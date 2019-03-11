#include <string.h>
#include "esp_log.h"
#include "soc/io_mux_reg.h"
#include "soc/gpio_reg.h"
#include "soc/gpio_sig_map.h"
#include "soc/spi_reg.h"
#include "rom/gpio.h"
#include "driver/periph_ctrl.h"
#include "esp_heap_caps.h"

#include "mcp23s17.h"

#define PIN_NUM_MOSI 23
#define PIN_NUM_MISO 19
#define PIN_NUM_CLK  18
#define PIN_NUM_CS   5
#define SPI_NUM      0x3

static const char* TAG = "MCP23S17";

uint8_t *spi_buf;

/**
 * spi_master_init.
 *
 * @see https://github.com/espressif/esp32-nesemu/blob/master/components/nofrendo-esp32/spi_lcd.c
 */
void spi_master_init()
{
    periph_module_enable(PERIPH_VSPI_MODULE);
    periph_module_enable(PERIPH_SPI_DMA_MODULE);

    PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO19_U, 2);
    PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO23_U, 2);
    PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO22_U, 2);
    PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO25_U, 2);
    WRITE_PERI_REG(GPIO_ENABLE_W1TS_REG, BIT19 | BIT23 | BIT22);

    gpio_matrix_in(PIN_NUM_MISO, VSPIQ_IN_IDX, 0);
    gpio_matrix_out(PIN_NUM_MOSI, VSPID_OUT_IDX, 0, 0);
    gpio_matrix_out(PIN_NUM_CLK, VSPICLK_OUT_IDX, 0, 0);
    gpio_matrix_out(PIN_NUM_CS, VSPICS0_OUT_IDX, 0, 0);

    CLEAR_PERI_REG_MASK(SPI_SLAVE_REG(SPI_NUM), SPI_TRANS_DONE << 5);
    SET_PERI_REG_MASK(SPI_USER_REG(SPI_NUM), SPI_CS_SETUP);
    CLEAR_PERI_REG_MASK(SPI_PIN_REG(SPI_NUM), SPI_CK_IDLE_EDGE);
    CLEAR_PERI_REG_MASK(SPI_USER_REG(SPI_NUM),  SPI_CK_OUT_EDGE);
    CLEAR_PERI_REG_MASK(SPI_CTRL_REG(SPI_NUM), SPI_WR_BIT_ORDER);
    CLEAR_PERI_REG_MASK(SPI_CTRL_REG(SPI_NUM), SPI_RD_BIT_ORDER);
    CLEAR_PERI_REG_MASK(SPI_USER_REG(SPI_NUM), SPI_DOUTDIN);
    WRITE_PERI_REG(SPI_USER1_REG(SPI_NUM), 0);
    SET_PERI_REG_BITS(SPI_CTRL2_REG(SPI_NUM), SPI_MISO_DELAY_MODE, 0, SPI_MISO_DELAY_MODE_S);
    CLEAR_PERI_REG_MASK(SPI_SLAVE_REG(SPI_NUM), SPI_SLAVE_MODE);

    // set clock 10MHz (80MHz / (7 + 1))
    WRITE_PERI_REG(SPI_CLOCK_REG(SPI_NUM), (7 << SPI_CLKCNT_N_S) | (7 << SPI_CLKCNT_L_S));

    SET_PERI_REG_MASK(SPI_USER_REG(SPI_NUM), SPI_CS_SETUP | SPI_CS_HOLD | SPI_USR_MOSI);
    SET_PERI_REG_MASK(SPI_CTRL2_REG(SPI_NUM), ((0x4 & SPI_MISO_DELAY_NUM) << SPI_MISO_DELAY_NUM_S));
    CLEAR_PERI_REG_MASK(SPI_USER_REG(SPI_NUM), SPI_USR_COMMAND);
    SET_PERI_REG_BITS(SPI_USER2_REG(SPI_NUM), SPI_USR_COMMAND_BITLEN, 0, SPI_USR_COMMAND_BITLEN_S);
    CLEAR_PERI_REG_MASK(SPI_USER_REG(SPI_NUM), SPI_USR_ADDR);
    SET_PERI_REG_BITS(SPI_USER1_REG(SPI_NUM), SPI_USR_ADDR_BITLEN, 0, SPI_USR_ADDR_BITLEN_S);
    CLEAR_PERI_REG_MASK(SPI_USER_REG(SPI_NUM), SPI_USR_MISO);
    SET_PERI_REG_MASK(SPI_USER_REG(SPI_NUM), SPI_USR_MOSI);
    char i;
    for (i = 0; i < 16; ++i) {
        WRITE_PERI_REG((SPI_W0_REG(SPI_NUM) + (i << 2)), 0);
    }
}

mcp23s17_err_t mcp23s17_init()
{
    ESP_LOGI(TAG, "spi init start.");
    spi_master_init();
    spi_buf = heap_caps_malloc(64, MALLOC_CAP_8BIT);
    ESP_LOGI(TAG, "spi init sccuess.");

    return MCP23S17_ERR_OK;
}

uint8_t mcp23s17_register(mcp23s17_reg_t reg, mcp23s17_gpio_t group)
{
   return (group == GPIOA)?(reg << 1): (reg << 1) | 1;
}

/**
 * spi_write_byte.
 *
 * @see https://github.com/espressif/esp32-nesemu/blob/master/components/nofrendo-esp32/spi_lcd.c
 */
mcp23s17_err_t mcp23s17_write_register(uint8_t addr, mcp23s17_reg_t reg, mcp23s17_gpio_t group, uint8_t data)
{
    uint8_t regpos = mcp23s17_register(reg, group);

    SET_PERI_REG_BITS(SPI_MOSI_DLEN_REG(SPI_NUM), SPI_USR_MOSI_DBITLEN, 0x17, SPI_USR_MOSI_DBITLEN_S);
    WRITE_PERI_REG((SPI_W0_REG(SPI_NUM)), (data << 16) | (regpos << 8) | addr);

    SET_PERI_REG_MASK(SPI_CMD_REG(SPI_NUM), SPI_USR);
    while (READ_PERI_REG(SPI_CMD_REG(SPI_NUM))&SPI_USR);

    return MCP23S17_ERR_OK;
}

mcp23s17_err_t mcp23s17_write_register_seq(uint8_t addr, mcp23s17_reg_t reg, mcp23s17_gpio_t group, uint8_t *data, size_t size)
{
    // address + register + size <= 64 (4byte * 16)
    // assert(size <= 62);

    uint8_t regpos = mcp23s17_register(reg, group);

    spi_buf[0] = addr;
    spi_buf[1] = regpos;
    for(uint8_t i = 0; i < size; i++) {
        spi_buf[i + 2] = data[i];
    }

    SET_PERI_REG_BITS(SPI_MOSI_DLEN_REG(SPI_NUM), SPI_USR_MOSI_DBITLEN, (size + 2) * 8 - 1, SPI_USR_MOSI_DBITLEN_S);
    uint8_t spipos = 0;
    for(uint8_t i = 0; i < size + 2; i += 4) {
        WRITE_PERI_REG((SPI_W0_REG(SPI_NUM) + (spipos << 2)), (spi_buf[i + 3] << 24) | (spi_buf[i + 2] << 16) | (spi_buf[i + 1] << 8) | spi_buf[i]);
        spipos++;
    }
    SET_PERI_REG_MASK(SPI_CMD_REG(SPI_NUM), SPI_USR);
    while (READ_PERI_REG(SPI_CMD_REG(SPI_NUM))&SPI_USR);

    return MCP23S17_ERR_OK;
}
