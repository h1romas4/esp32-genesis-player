#include <math.h>
#include "driver/i2c.h"
#include "soc/gpio_sig_map.h"
#include "soc/i2c_struct.h"
#include "soc/i2c_reg.h"
#include "esp_system.h"
#include "esp_log.h"
#include "ltc6904.h"

#define ACK_CHECK_EN  0x1 /*!< I2C master will check ack from slave*/
#define ACK_CHECK_DIS 0x0 /*!< I2C master will not check ack from slave */

static const char* TAG = "ltc6904.c";

ltc6904_err_t ltc6904_init()
{
    ESP_LOGI(TAG, "ltc6904 init start.");

	esp_err_t ret;
	i2c_config_t conf = {
		.mode = I2C_MODE_MASTER,
		.sda_io_num = GPIO_NUM_21,
		.scl_io_num = GPIO_NUM_22,
		.sda_pullup_en = GPIO_PULLUP_DISABLE,
		.scl_pullup_en = GPIO_PULLUP_DISABLE,
		.master.clk_speed = 100000
	};
	ret = i2c_param_config(I2C_NUM_0, &conf);

	if( ret != ESP_OK ) {
		ESP_LOGE(TAG,"PARAM CONFIG FAILED");
		return LTC6904_ERR_CONFIG;
	}

	// install the driver
	ret = i2c_driver_install(I2C_NUM_0, I2C_MODE_MASTER, 0, 0, 0);
	if(ret != ESP_OK) {
		ESP_LOGE(TAG,"I2C driver install failed");
		return LTC6904_ERR_INSTALL;
	}

    ESP_LOGI(TAG, "ltc6904 init end.");

    return LTC6904_ERR_OK;
}

ltc6904_err_t ltc6904_write(uint8_t addr, uint16_t code)
{
    uint8_t high = (uint8_t)((code & 0xff00) >> 8);
    uint8_t low = (uint8_t)(code & 0x00ff);

    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, addr << 1 | I2C_MASTER_WRITE, ACK_CHECK_EN);
    i2c_master_write_byte(cmd, high, ACK_CHECK_EN);
    i2c_master_write_byte(cmd, low, ACK_CHECK_EN);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    if (ret == ESP_FAIL) {
        ESP_LOGE(TAG,"ERROR: unable to write to register");
        return LTC6904_ERR_FAIL;
    }
    return LTC6904_ERR_OK;
}

ltc6904_err_t ltc6904_set_clock(uint8_t addr, float freq_khz)
{
    uint16_t oct = (int16_t)(3.322 * log10(freq_khz * 1000.0 / 1039.0));
    uint16_t dac = (int16_t)(2048.0 - 2078.0 * pow(2, 10 + oct) / (freq_khz * 1000.0) + 0.5);
    ltc6904_write(addr, (uint16_t)((oct<<12) | (dac<<2)));

    return LTC6904_ERR_OK;
}
