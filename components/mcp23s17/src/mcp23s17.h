#define MCP23S17_IODIRA		0x00
#define MCP23S17_IPOLA 		0x02
#define MCP23S17_GPINTENA 	0x04
#define MCP23S17_DEFVALA 	0x06
#define MCP23S17_INTCONA 	0x08
#define MCP23S17_IOCONA 	0x0A
#define MCP23S17_GPPUA 		0x0C
#define MCP23S17_INTFA 		0x0E
#define MCP23S17_INTCAPA 	0x10
#define MCP23S17_GPIOA 		0x12
#define MCP23S17_OLATA 		0x14

#define MCP23S17_IODIRB 	0x01
#define MCP23S17_IPOLB 		0x03
#define MCP23S17_GPINTENB 	0x05
#define MCP23S17_DEFVALB 	0x07
#define MCP23S17_INTCONB 	0x09
#define MCP23S17_IOCONB 	0x0B
#define MCP23S17_GPPUB 		0x0D
#define MCP23S17_INTFB 		0x0F
#define MCP23S17_INTCAPB 	0x11
#define MCP23S17_GPIOB 		0x13
#define MCP23S17_OLATB 		0x15

// 0 1 0 0 A2 A1 A0 RW
#define MCP23S17_DEFAULT_ADDR 0x40
#define MCP23S17_WRITE_REGISTER 0x00
#define MCP23S17_READ_REGISTER 0x01

typedef enum {
   MCP23S17_ERR_OK      = 0x00,
   MCP23S17_ERR_CONFIG  = 0x01,
   MCP23S17_ERR_INSTALL = 0x02,
   MCP23S17_ERR_FAIL    = 0x03
} mcp23s17_err_t;

typedef enum {
	MCP23S17_IODIR		= 0x00,
	MCP23S17_IPOL		= 0x01,
	MCP23S17_GPINTEN	= 0x02,
	MCP23S17_DEFVAL	    = 0x03,
	MCP23S17_INTCON	    = 0x04,
	MCP23S17_IOCON		= 0x05,
	MCP23S17_GPPU		= 0x06,
	MCP23S17_INTF		= 0x07,
	MCP23S17_INTCAP	    = 0x08,
	MCP23S17_GPIO		= 0x09,
	MCP23S17_OLAT		= 0x0A
} mcp23s17_reg_t;

typedef enum {
	GPIOA = 0x00,
	GPIOB = 0x01
} mcp23s17_gpio_t;

mcp23s17_err_t mcp23s17_init();
mcp23s17_err_t mcp23s17_write_register(uint8_t addr, mcp23s17_reg_t reg, mcp23s17_gpio_t group, uint8_t data);
mcp23s17_err_t mcp23s17_write_register_seq(uint8_t addr, mcp23s17_reg_t reg, mcp23s17_gpio_t group, uint8_t *data, size_t size);
