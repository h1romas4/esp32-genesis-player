# esp32-genesis-player

Work in progress..

## Demo

[![demo](https://github.com/h1romas4/esp32-genesis-player/raw/master/docs/images/demo01.png)](https://youtu.be/mzKiQgvpdTA)

## Require

* ESP32 (ESP32-DevKitC, M5Stack...)
* [esp32-idf toolchain setup](https://docs.espressif.com/projects/esp-idf/en/stable/get-started/index.html#setup-toolchain)

```
$ xtensa-esp32-elf-gcc -v
gcc version 5.2.0 (crosstool-NG crosstool-ng-1.22.0-80-g6c4433a)
```

## Build

![](https://github.com/h1romas4/esp32-genesis-player/workflows/ESP32%20CI/badge.svg)

```
git clone  --recursive https://github.com/h1romas4/esp32-genesis-player.git
cd esp32-genesis-player
# This repository includes eps-idf v3.3.3
export IDF_PATH=$(pwd)/esp-idf
# Serial flasher config
make menuconfig
# compile & flash
make flash
```

## Upload .vgm file to esp32 flash

```
# mv example.vgz example.vgm.gz
# gzip -d example.vgm.gz
./flashrom.sh vgm/kanon2612.vgm
```

## Schematic

![fg05-mdp01-rev-a](https://github.com/h1romas4/esp32-genesis-player/raw/master/docs/schematic/fg05-mdp01-rev-a.png)

### ESP32 (MBUS)

```
ESP32 GPIO(23) VSPI MOSI     MCP23S17(13) SI
ESP32 GPIO(19) VSPI MISO     MCP23S17(14) SO
ESP32 GPIO(18) VSPI SCK      MCP23S17(12) SCK
ESP32 GPIO( 5) VSPI SS       MCP23S17(11) CS
ESP32 GPIO(22) SCL           Si5351A(2)  SCL
ESP32 GPIO(21) SDA           Si5351A(3)  SDA
```

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

## Si5351A ([AE-Si5351A-B](http://akizukidenshi.com/catalog/g/gK-10679/))

```
DIP(1) VDD0            3.3V
DIP(4) GND             GND
DIP(7) CLK2            (NC)
DIP(8) VDD             3.3V
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
IC(24) CLK            Si5351A(5) CLK0
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
IC(14) CLK            Si5351A(6) CLK1
IC(15) D4             MCP23S17( 5)  GPB4
IC(16) Vcc            5V
```
