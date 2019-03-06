#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs_flash.h"
#include "esp_system.h"
#include "esp_log.h"
#include "esp_partition.h"
#include "esp_heap_caps.h"
#include "esp_timer.h"
#include "ltc6904.h"
#include "chips.h"

#define SAMPLE_RATE 44100.0

static const char *TAG = "vgmplay";

// uint8_t *vgm;
uint8_t *vgm;
uint32_t vgmpos = 0x40;
bool vgmend = false;
uint32_t vgmloopoffset;
uint32_t datpos;
uint32_t pcmpos;
uint32_t pcmoffset;

int64_t startTime;
int64_t duration;
uint32_t delay8n[16];
uint32_t delay7n[16];

void load_vgmdata()
{
    uint8_t *data;
    const esp_partition_t *part;
    spi_flash_mmap_handle_t hrom;
    esp_err_t err;

    nvs_flash_init();

    part = esp_partition_find_first(ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_DATA_PHY, NULL);
    if (part == 0) {
        ESP_LOGE(TAG, "Couldn't find vgm part!");
    }

    err = esp_partition_mmap(part, 0, 0x1EF000, SPI_FLASH_MMAP_DATA, (const void **)&data, &hrom);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Couldn't map vgm part!");
    }
    ESP_LOGI(TAG, "read vgm data @%p", data);

    // // for testing..
    // vgm = (uint8_t *)heap_caps_malloc(100000 * sizeof(uint8_t), MALLOC_CAP_8BIT);
    // if(vgm == NULL) {
    //     ESP_LOGE(TAG, "Memory alloc error");
    // }
    // memcpy(vgm, data, 100000);
    // // vgm = (uint8_t *)data;

    vgm = (uint8_t *)data;
}

uint8_t get_vgm_ui8()
{
    return vgm[vgmpos++];
}

uint16_t get_vgm_ui16()
{
    return get_vgm_ui8() + (get_vgm_ui8() << 8);
}

uint32_t get_vgm_ui32()
{
    return get_vgm_ui8() + (get_vgm_ui8() << 8) + (get_vgm_ui8() << 16) + (get_vgm_ui8() << 24);
}

void init_vgm()
{
    uint32_t clock_sn76489;
    uint32_t clock_ym2612;

    // load vgm data
    load_vgmdata();

    // read vgm header
    vgmpos = 0x0C; clock_sn76489 = get_vgm_ui32();
    vgmpos = 0x2C; clock_ym2612 = get_vgm_ui32();
    vgmpos = 0x1c; vgmloopoffset = get_vgm_ui32();
    vgmpos = 0x34; vgmpos = 0x34 + get_vgm_ui32();

    if (clock_ym2612 == 0) clock_ym2612 = 7670453;
    if (clock_sn76489 == 0) clock_sn76489 = 3579545;

    // init clock
    ltc6904_init();
    ltc6904_set_clock(LTC6904_ADDR_0, clock_ym2612 / 1000);
    ltc6904_set_clock(LTC6904_ADDR_1, clock_sn76489 / 1000);

    ESP_LOGI(TAG, "clock_sn76489 : %d", clock_sn76489);
    ESP_LOGI(TAG, "clock_ym2612 : %d", clock_ym2612);
    ESP_LOGI(TAG, "vgmpos : 0x%x", vgmpos);

    // pre calc delay sample.
    for (int i = 0; i < 16; i++) {
        if (i == 0) {
            delay8n[i] = 0;
            delay7n[i] = 1;
        } else {
            delay8n[i] = ((1000.0 / (SAMPLE_RATE / (float)i)) * 1000);
            // +1 is spec-accurate; however, on this hardware, it sounds better without it.
            delay7n[i] = ((1000.0 / (SAMPLE_RATE / (float)i + 1)) * 1000);
        }
    }
}

void pause(uint32_t samples)
{
    duration = ((1000.0 / (SAMPLE_RATE / (float)samples)) * 1000);
    startTime = esp_timer_get_time();
}

void pause_ms(uint32_t ms)
{
    duration = ms;
    startTime = esp_timer_get_time();
}

void play_vgm()
{
    uint8_t command;
    uint8_t reg;
    uint8_t dat;

    if (esp_timer_get_time() - startTime <= duration) {
        return;
    }

    command = get_vgm_ui8();
    switch (command) {
        case 0x50:
            dat = get_vgm_ui8();
            SN76489_Write(dat);
            break;
        case 0x52:
        case 0x53:
            reg = get_vgm_ui8();
            dat = get_vgm_ui8();
            YM2612_Write(0 + ((command & 1) << 1), reg);
            YM2612_Write(1 + ((command & 1) << 1), dat);
            break;
        case 0x61:
            pause(get_vgm_ui16());
            break;
        case 0x62:
            pause(735);
            break;
        case 0x63:
            pause(882);
            break;
        case 0x66:
            if(vgmloopoffset == 0) {
                vgmend = true;
            } else {
                vgmpos = vgmloopoffset;
            }
            break;
        case 0x67:
            get_vgm_ui8(); // 0x66
            get_vgm_ui8(); // 0x00 data type
            datpos = vgmpos + 4;
            vgmpos += get_vgm_ui32(); // size of data, in bytes
            break;
        case 0x70:
        case 0x71:
        case 0x72:
        case 0x73:
        case 0x74:
        case 0x75:
        case 0x76:
        case 0x77:
        case 0x78:
        case 0x79:
        case 0x7a:
        case 0x7b:
        case 0x7c:
        case 0x7d:
        case 0x7e:
        case 0x7f:
            pause_ms(delay7n[command & 0x0f]);
            break;
        case 0x80:
        case 0x81:
        case 0x82:
        case 0x83:
        case 0x84:
        case 0x85:
        case 0x86:
        case 0x87:
        case 0x88:
        case 0x89:
        case 0x8a:
        case 0x8b:
        case 0x8c:
        case 0x8d:
        case 0x8e:
        case 0x8f:
            pause_ms(delay8n[command & 0x0f]);
            YM2612_Write(0, 0x2a);
            YM2612_Write(1, vgm[datpos + pcmpos + pcmoffset]);
            pcmoffset++;
            break;
        case 0xe0:
            pcmpos = get_vgm_ui32();
            pcmoffset = 0;
            break;
        default:
            ESP_LOGE(TAG, "unknown cmd at 0x%x: 0x%x", vgmpos, vgm[vgmpos]);
            vgmpos++;
            break;
    }
}

// The setup routine runs once when M5Stack starts up
void setup()
{
    // Initialize VGM data
    init_vgm();

    // Initialize chips
    init_chips();

    ESP_LOGI(TAG, "Start VGM! (0x%x)", vgmpos);
    while (!vgmend) {
        play_vgm();
    }
    ESP_LOGI(TAG, "End VGM! (0x%x)", vgmpos);
}

// The loop routine runs over and over again forever
void loop()
{
}

// The arduino task
void loopTask(void *pvParameters)
{
    setup();
    for (;;) {
        esp_timer_get_time(); // update overflow
        loop();
    }
}

void app_main()
{
    xTaskCreatePinnedToCore(loopTask, "loopTask", 8192, NULL, 1, NULL, 1);
}
