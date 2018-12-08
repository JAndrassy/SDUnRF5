# SD Update library for nRF5 Arduino

This library is nRF51 version of [Arduino SAMD architecture core SDU library](https://github.com/arduino/ArduinoCore-samd/blob/master/libraries/SDU/examples/Usage/Usage.ino).

The library works with hex versions of SDUBoot.ino, which are prepended by linker before your compiled sketch. Currently only hex files of SDUBoot.ino for 'Seeed Arch Link' are generated in this library. 'Seeed Arch Link' has on board SD card holder and Ethernet chip.

The SDU library can be used with remote upload library [ArduinoOTA](https://github.com/jandrassy/ArduinoOTA).

To support additional nRF51 board, add #ifdefs to SDUBoot.ino and SDU.cpp and add lines to build.sh in extra/SDUBoot folder. Then run build.sh.  

SDU library requires support in linker script. To scripts nrf51_common.ld, and nrf5x_common.ld in sandeepmistry/hardware/nRF5/0.6.0/cores/nRF5/SDK/components/toolchain/gcc add lines

```
        KEEP(*(.sketch_boot))
```
after `.text : {`
