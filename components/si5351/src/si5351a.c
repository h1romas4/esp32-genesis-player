// Reference Library
//  @see https://github.com/adafruit/Adafruit_Si5351_Library
// for AE-Si5351A-B
//  http://akizukidenshi.com/catalog/g/gK-10679/

#include <math.h>
#include "freertos/FreeRTOS.h"
#include "freertos/FreeRTOSConfig.h"
#include "freertos/portmacro.h"
#include "esp_log.h"
#include "driver/i2c.h"
#include "si5351a.h"

// for vscode c_cpp_extention
#ifdef __INTELLISENSE__
#include "build/include/sdkconfig.h"
#endif

static const char *TAG = "si5351a.c";

esp_err_t st5351a_write(uint8_t reg, uint8_t dat)
{
    esp_err_t ret;

    uint8_t addr = SI5351A_ADDRESS;

    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, addr << 1 | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, reg, true);
    i2c_master_write_byte(cmd, dat, true);
    i2c_master_stop(cmd);
    ret = i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);

    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "ERROR: unable to write to register");
    }

    return ret;
}

st5351a_err_t si5351a_init()
{
    ESP_LOGI(TAG, "si5351a init start.");

    esp_err_t ret;

    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = GPIO_NUM_21,
        .scl_io_num = GPIO_NUM_22,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = 100000};
    ret = i2c_param_config(I2C_NUM_0, &conf);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "PARAM CONFIG FAILED");
        return SI5351A_ERR_CONFIG;
    }

    // install the driver
    ret = i2c_driver_install(I2C_NUM_0, I2C_MODE_MASTER, 0, 0, 0);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "I2C driver install failed");
        return SI5351A_ERR_INSTALL;
    }

    /* Disable all outputs setting CLKx_DIS high */
    ESP_ERROR_CHECK(st5351a_write(SI5351_REGISTER_3_OUTPUT_ENABLE_CONTROL, 0xFF));
    /* Power down all output drivers */
    ESP_ERROR_CHECK(st5351a_write(SI5351_REGISTER_16_CLK0_CONTROL, 0x80));
    ESP_ERROR_CHECK(st5351a_write(SI5351_REGISTER_17_CLK1_CONTROL, 0x80));
    ESP_ERROR_CHECK(st5351a_write(SI5351_REGISTER_18_CLK2_CONTROL, 0x80));
    ESP_ERROR_CHECK(st5351a_write(SI5351_REGISTER_19_CLK3_CONTROL, 0x80));
    ESP_ERROR_CHECK(st5351a_write(SI5351_REGISTER_20_CLK4_CONTROL, 0x80));
    ESP_ERROR_CHECK(st5351a_write(SI5351_REGISTER_21_CLK5_CONTROL, 0x80));
    ESP_ERROR_CHECK(st5351a_write(SI5351_REGISTER_22_CLK6_CONTROL, 0x80));
    ESP_ERROR_CHECK(st5351a_write(SI5351_REGISTER_23_CLK7_CONTROL, 0x80));
    /* Set the load capacitance for the XTAL */
    ESP_ERROR_CHECK(st5351a_write(SI5351_REGISTER_183_CRYSTAL_INTERNAL_LOAD_CAPACITANCE, SI5351_CRYSTAL_LOAD_8PF));

    ESP_LOGI(TAG, "si5351a init end.");

    return SI5351A_ERR_OK;
}

st5351a_err_t si5351a_setup_PLL(si5351PLL_t pll, uint8_t mult, uint32_t num, uint32_t denom)
{
    assert((mult > 14) && (mult < 91)); /* mult = 15..90 */
    assert(denom > 0);                  /* Avoid divide by zero */
    assert(num <= 0xFFFFF);             /* 20-bit limit */
    assert(denom <= 0xFFFFF);           /* 20-bit limit */

    ESP_LOGI(TAG, "si5351a si5351a_setup_PLL start.");

    /* PLL config register P1 */
    uint32_t P1;
    /* PLL config register P2 */
    uint32_t P2;
    /* PLL config register P3 */
    uint32_t P3;

    /* Set the main PLL config registers */
    if (num == 0) {
        /* Integer mode */
        P1 = 128 * mult - 512;
        P2 = num;
        P3 = denom;
    }
    else {
        /* Fractional mode */
        P1 = (uint32_t)(128 * mult + floor(128 * ((float)num / (float)denom)) - 512);
        P2 = (uint32_t)(128 * num - denom * floor(128 * ((float)num / (float)denom)));
        P3 = denom;
    }

    /* Get the appropriate starting point for the PLL registers */
    uint8_t baseaddr = (pll == SI5351_PLL_A ? 26 : 34);

    /* The datasheet is a nightmare of typos and inconsistencies here! */
    ESP_ERROR_CHECK(st5351a_write(baseaddr    , (P3 & 0x0000FF00) >> 8));
    ESP_ERROR_CHECK(st5351a_write(baseaddr + 1, (P3 & 0x000000FF)));
    ESP_ERROR_CHECK(st5351a_write(baseaddr + 2, (P1 & 0x00030000) >> 16));
    ESP_ERROR_CHECK(st5351a_write(baseaddr + 3, (P1 & 0x0000FF00) >> 8));
    ESP_ERROR_CHECK(st5351a_write(baseaddr + 4, (P1 & 0x000000FF)));
    ESP_ERROR_CHECK(st5351a_write(baseaddr + 5, ((P3 & 0x000F0000) >> 12) | ((P2 & 0x000F0000) >> 16)));
    ESP_ERROR_CHECK(st5351a_write(baseaddr + 6, (P2 & 0x0000FF00) >> 8));
    ESP_ERROR_CHECK(st5351a_write(baseaddr + 7, (P2 & 0x000000FF)));

    /* Reset both PLLs */
    ESP_ERROR_CHECK(st5351a_write(SI5351_REGISTER_177_PLL_RESET, (1 << 7) | (1 << 5)));

    ESP_LOGI(TAG, "si5351a si5351a_setup_PLL end.");

    return SI5351A_ERR_OK;
}

st5351a_err_t si5351a_setup_multisynth(uint8_t output, si5351PLL_t pllSource, uint32_t div, uint32_t num, uint32_t denom)
{
    assert(output < 3);       /* Channel range */
    assert(div > 3);          /* Divider integer value */
    assert(div < 901);        /* Divider integer value */
    assert(denom > 0);        /* Avoid divide by zero */
    assert(num <= 0xFFFFF);   /* 20-bit limit */
    assert(denom <= 0xFFFFF); /* 20-bit limit */

    ESP_LOGI(TAG, "si5351a si5351a_setup_multisynth start.");

    /* Multisynth config register P1 */
    uint32_t P1;
    /* Multisynth config register P2 */
    uint32_t P2;
    /* Multisynth config register P3 */
    uint32_t P3;

    /* Set the main PLL config registers */
    if (num == 0) {
        /* Integer mode */
        P1 = 128 * div - 512;
        P2 = num;
        P3 = denom;
    }
    else {
        /* Fractional mode */
        P1 = (uint32_t)(128 * div + floor(128 * ((float)num / (float)denom)) - 512);
        P2 = (uint32_t)(128 * num - denom * floor(128 * ((float)num / (float)denom)));
        P3 = denom;
    }

    /* Get the appropriate starting point for the PLL registers */
    uint8_t baseaddr = 0;
    switch (output) {
        case 0:
            baseaddr = SI5351_REGISTER_42_MULTISYNTH0_PARAMETERS_1;
            break;
        case 1:
            baseaddr = SI5351_REGISTER_50_MULTISYNTH1_PARAMETERS_1;
            break;
        case 2:
            baseaddr = SI5351_REGISTER_58_MULTISYNTH2_PARAMETERS_1;
            break;
    }

    /* Set the MSx config registers */
    ESP_ERROR_CHECK(st5351a_write(baseaddr    , (P3 & 0x0000FF00) >> 8));
    ESP_ERROR_CHECK(st5351a_write(baseaddr + 1, (P3 & 0x000000FF)));
    ESP_ERROR_CHECK(st5351a_write(baseaddr + 2, (P1 & 0x00030000) >> 16)); /* ToDo: Add DIVBY4 (>150MHz) and R0 support (<500kHz) later */
    ESP_ERROR_CHECK(st5351a_write(baseaddr + 3, (P1 & 0x0000FF00) >> 8));
    ESP_ERROR_CHECK(st5351a_write(baseaddr + 4, (P1 & 0x000000FF)));
    ESP_ERROR_CHECK(st5351a_write(baseaddr + 5, ((P3 & 0x000F0000) >> 12) | ((P2 & 0x000F0000) >> 16)));
    ESP_ERROR_CHECK(st5351a_write(baseaddr + 6, (P2 & 0x0000FF00) >> 8));
    ESP_ERROR_CHECK(st5351a_write(baseaddr + 7, (P2 & 0x000000FF)));

    /* Configure the clk control and enable the output */
    /* 8mA drive strength, MS0 as CLK0 source, Clock not inverted, powered up */
    uint8_t clkControlReg = 0x0F;
    if (pllSource == SI5351_PLL_B) {
        /* Uses PLLB */
        clkControlReg |= (1 << 5);
    }
    if (num == 0) {
        /* Integer mode */
        clkControlReg |= (1 << 6);
    }

    switch (output) {
        case 0:
            ESP_ERROR_CHECK(st5351a_write(SI5351_REGISTER_16_CLK0_CONTROL, clkControlReg));
            break;
        case 1:
            ESP_ERROR_CHECK(st5351a_write(SI5351_REGISTER_17_CLK1_CONTROL, clkControlReg));
            break;
        case 2:
            ESP_ERROR_CHECK(st5351a_write(SI5351_REGISTER_18_CLK2_CONTROL, clkControlReg));
            break;
    }

    ESP_LOGI(TAG, "si5351a si5351a_setup_multisynth end.");

    return SI5351A_ERR_OK;
}

st5351a_err_t st5351a_enable_outputs(bool enabled)
{
    ESP_LOGI(TAG, "si5351a st5351a_enable_outputs start.");

    /* Enabled desired outputs (see Register 3) */
    ESP_ERROR_CHECK(st5351a_write(SI5351_REGISTER_3_OUTPUT_ENABLE_CONTROL, enabled ? 0x00: 0xFF));

    ESP_LOGI(TAG, "si5351a st5351a_enable_outputs end.");

    return SI5351A_ERR_OK;
}

st5351a_err_t st5351a_set_clock(void)
{
    // init
    si5351a_init();
    // 630MHz / 82.13334MHz = 7.6704Hz
    // si5351a_setup_PLL(SI5351_PLL_A, 25, 1, 5);
    // si5351a_setup_multisynth(0, SI5351_PLL_A, 82, 13334, 100000);
    // 675MHz / 88MHz = 7.6704Hz
    si5351a_setup_PLL(SI5351_PLL_A, 27, 0, 1);
    si5351a_setup_multisynth(0, SI5351_PLL_A, 88, 0, 1);
    // 630MHz / 176MHz = 3.5795Hz
    si5351a_setup_PLL(SI5351_PLL_B, 25, 1, 5);
    si5351a_setup_multisynth(1, SI5351_PLL_B, 175, 1, 1);
    // si5351a_setup_PLL(SI5351_PLL_B, 25, 1, 5);
    // si5351a_setup_multisynth(1, SI5351_PLL_B, 175, 1, 1);
    // output clock
    st5351a_enable_outputs(true);

    return SI5351A_ERR_OK;
}
