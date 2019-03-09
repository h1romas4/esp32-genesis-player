#define LTC6904_ADDR_0 0b0010110
#define LTC6904_ADDR_1 0b0010111

typedef enum {
   LTC6904_ERR_OK      = 0x00,
   LTC6904_ERR_CONFIG  = 0x01,
   LTC6904_ERR_INSTALL = 0x02,
   LTC6904_ERR_FAIL    = 0x03
} ltc6904_err_t;

ltc6904_err_t ltc6904_init();
ltc6904_err_t ltc6904_write(uint8_t addr, uint16_t code);
ltc6904_err_t ltc6904_set_clock(uint8_t addr, float freq);
