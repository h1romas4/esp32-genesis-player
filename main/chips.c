#include "freertos/FreeRTOS.h"
#include "freertos/FreeRTOSConfig.h"
#include "freertos/portmacro.h"
#include "esp_system.h"
#include "esp_heap_caps.h"
#include "esp_log.h"
#include "freertos/mpu_wrappers.h"
#include "freertos/task.h"
#include "chips.h"
#include "mcp23s17.h"
#include "si5351a.h"

// for vscode c_cpp_extention
#ifdef __INTELLISENSE__
#include "build/include/sdkconfig.h"
#endif

#define CL_OE 0b10000000
#define SN_WR 0b01000000
#define YM_IC 0b00100000
#define YM_RD 0b00010000
#define YM_WR 0b00001000
#define YM_CS 0b00000100
#define YM_A1 0b00000010
#define YM_A0 0b00000001

#define YM_AD_MASK 0b11110000
#define SN_WR_MASK 0b10111111

#define GPIO_HIGH 0
#define GPIO_LOW 1

static const char* TAG = "chips.cpp";

uint8_t mcp_state_gpa = 0b00000000;

void write_sound_control(uint8_t dat, uint8_t cnt)
{
    if(cnt == GPIO_HIGH) {
        mcp_state_gpa = mcp_state_gpa | dat;
    } else {
        mcp_state_gpa = mcp_state_gpa & ~dat;
    }
    mcp23s17_write_register(MCP23S17_DEFAULT_ADDR, MCP23S17_GPIO, GPIOA, mcp_state_gpa);
}

void init_chips(chips_t *chips)
{
    ESP_LOGI(TAG, "chips init start.");

    // mcp23s17 init
    mcp23s17_err_t ret = mcp23s17_init();
    ESP_ERROR_CHECK(ret);

	// make all I/O's output
	mcp23s17_write_register(MCP23S17_DEFAULT_ADDR, MCP23S17_IODIR, GPIOA, 0x00);
	mcp23s17_write_register(MCP23S17_DEFAULT_ADDR, MCP23S17_IODIR, GPIOB, 0x00);
    // set byte mode and seq mode
	mcp23s17_write_register(MCP23S17_DEFAULT_ADDR, MCP23S17_IOCON, GPIOA, 0b00100000);
    // all bit HIGH
    mcp23s17_write_register(MCP23S17_DEFAULT_ADDR, MCP23S17_GPIO, GPIOA, 0xff);
    mcp23s17_write_register(MCP23S17_DEFAULT_ADDR, MCP23S17_GPIO, GPIOB, 0xff);

    // clock setting
    ESP_LOGI(TAG, "clock_sn76489 : %g", chips->clock_ym2612);
    ESP_LOGI(TAG, "clock_ym2612 : %g", chips->clock_sn76489);

    // set clock (TODO:)
    // clock_ym2612 = 7670453; clock_sn76489 = 3579545;
    st5351a_set_clock();

    // LED
    write_sound_control(CL_OE, GPIO_HIGH);

    // SN76489 reset
    write_sound_control(SN_WR, GPIO_HIGH);
    SN76489_Write(0x9f);
    SN76489_Write(0xbf);
    SN76489_Write(0xdf);
    SN76489_Write(0xff);

    // YM2612 reset
    write_sound_control(YM_A0 | YM_A1, GPIO_LOW);
    write_sound_control(YM_CS | YM_WR | YM_RD | YM_IC, GPIO_HIGH);
    vTaskDelay(100 / portTICK_RATE_MS);
    write_sound_control(YM_IC, GPIO_LOW);
    vTaskDelay(100 / portTICK_RATE_MS);
    write_sound_control(YM_IC, GPIO_HIGH);

    ESP_LOGI(TAG, "chips init end.");
}

void YM2612_Write(uint8_t addr, uint8_t data)
{
    uint8_t buf[6];
    // data set
    buf[0] = data;
    // WR(0) CS(0) YM_A1 YM_A0
    buf[1] = (mcp_state_gpa & YM_AD_MASK) | ( 0b0000 | (addr & 0x03));
    // data set (dummy)
    buf[2] = data;
    // (wait dummyy)
    buf[3] = (mcp_state_gpa & YM_AD_MASK) | ( 0b0000 | (addr & 0x03));
    // (wait dummyy)
    buf[4] = data;
    // WR(1) CS(1) YM_A1 YM_A0
    buf[5] = (mcp_state_gpa & YM_AD_MASK) | ( 0b1100 | (addr & 0x03));
    // send command
    mcp23s17_write_register_seq(MCP23S17_DEFAULT_ADDR, MCP23S17_GPIO, GPIOB, buf, 6);
    // update state
    mcp_state_gpa = (mcp_state_gpa & YM_AD_MASK) | ( 0b1100 | (addr & 0x03));
}

void SN76489_Write(uint8_t data)
{
    uint8_t wait = 8;
    uint8_t buf[(wait + 1) * 2 + 2];
    // data set
    buf[0] = data;
    // WR(0)
    buf[1] = mcp_state_gpa & SN_WR_MASK;
    // wait
    for(uint8_t i = 0; i < wait; i++) {
        // data set (dummy)
        buf[i * 2 + 2] = data;
        // WR(0) wait
        buf[i * 2 + 3] = mcp_state_gpa & SN_WR_MASK;
    }
    // data set (dummy)
    buf[(wait + 1) * 2] = data;
    // WR(1)
    buf[(wait + 1) * 2 + 1] = (mcp_state_gpa & SN_WR_MASK) | SN_WR;
    // send command
    mcp23s17_write_register_seq(MCP23S17_DEFAULT_ADDR, MCP23S17_GPIO, GPIOB, buf, (wait + 1) * 2 + 2);
    // update state
    mcp_state_gpa = (mcp_state_gpa & SN_WR_MASK) | SN_WR;
}
