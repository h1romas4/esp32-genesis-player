# esp32-genesis-player

## Work in progress..

![progress02](https://github.com/h1romas4/esp32-genesis-player/raw/master/docs/images/progress02.jpg)

## Upload .vgm file to esp32 flash

```
mv example.vgz example.vgm.gz
gzip -d example.vgm.gz
./flashrom.sh example.vgm
```

## Schematic

### MCP23S17

```
IC( 9) Vdd             3.3V
IC(10) Vss             GND
IC(15) A0              GND
IC(16) A1              GND
IC(17) A2              GND
IC(18) RESET           3.3V
IC(19) INTA            (NC)
IC(29) INTB            (NC)
```

### LTC6904 #1,#2

```
IC( 1) GND             GND
IC( 4) ADR             3.3V (0b0010111) LTC6904 #1
IC( 4) ADR             GND  (0b0010110) LTC6904 #2
IC( 5) ~CLK            (NC)
IC( 7) OE              MCP23S17(28) GPA7
IC( 8) V+              3.3V - 1uF bypass capacitor
```

### ESP32

```
GPIO(23) VSPI MOSI     MCP23S17(13) SI
GPIO(19) VSPI MISO     MCP23S17(14) SO
GPIO(18) VSPI SCK      MCP23S17(12) SCK
GPIO( 5) VSPI SS       MCP23S17(11) CS
GPIO(22) SCL           LTC6904( 3)  SCL - 4.7K external pullup
GPIO(21) SDA           LTC6904( 2)  SDI - 4.7K external pullup
```

### YM2612

```
IC( 1) GND            GND
IC( 2) D0             MCP23S17( 1)  GPB0
IC( 3) D1             MCP23S17( 2)  GPB1
IC( 4) D2             MCP23S17( 3)  GPB2
IC( 5) D3             MCP23S17( 4)  GPB3
IC( 6) D4             MCP23S17( 5)  GPB4
IC( 7) D5             MCP23S17( 6)  GPB5
IC( 8) D6             MCP23S17( 7)  GPB6
IC( 9) D7             MCP23S17( 8)  GPB7
IC(10) NC             (NC)
IC(11) ~IC            MCP23S17(26)  GPA5
IC(12) GND            GND
IC(13) ~IRQ           (NC)
IC(14) ~CS            MCP23S17(23)  GPA2
IC(15) ~WR            MCP23S17(24)  GPA3
IC(16) ~RD            MCP23S17(25)  GPA4
IC(17) A0             MCP23S17(21)  GPA0
IC(18) A1             MCP23S17(22)  GPA1
IC(19) AGND           GND
IC(20) MOR            (audio R)
IC(21) MOL            (audio L)
IC(22) AVcc           5V - 100uf bypass capacitor
IC(23) Vcc            5V - 0.1uF bypass capacitor
IC(24) CLK            LTC6904#2( 6) CLK
```

### SN76489

```
IC( 1) D5             MCP23S17( 6)  GPB5
IC( 2) D6             MCP23S17( 7)  GPB6
IC( 3) D7             MCP23S17( 8)  GPB7
IC( 4) READY          (NC)
IC( 5) ~WE            MCP23S17(27)  GPA6
IC( 6) ~CE            GND
IC( 7) SNDOUT         (audio L/R)
IC( 8) GND            GND
IC( 9) NC             (NC)
IC(10) D0             MCP23S17( 1)  GPB0
IC(11) D1             MCP23S17( 2)  GPB1
IC(12) D2             MCP23S17( 3)  GPB2
IC(13) D3             MCP23S17( 4)  GPB3
IC(14) CLK            LTC6904#1( 6) CLK
IC(15) D4             MCP23S17( 5)  GPB4
IC(16) Vcc            5V
```
