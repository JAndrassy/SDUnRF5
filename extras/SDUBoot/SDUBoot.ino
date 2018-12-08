/*
  Copyright (c) 2017 Arduino LLC.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

 SAMD version Feb 2017
 by Sandeep Mistry (Arduino)
 modified for nRF5 Dec 2018
 by Juraj Andrassy
*/

#include <SD.h>
#include <Flash.h>

#if defined(NRF51_S110)
#define SDU_START        (0x18000)
#elif defined(NRF51_S130)
#define SDU_START        (0x1b000)
#elif defined(NRF51)
#define SDU_START        (0x0000)
#else
  #error "Unsupported board!"
#endif
#define SDU_SIZE     0x4000

#define SKETCH_START (uint32_t*)(SDU_START + SDU_SIZE)

#ifndef SDCARD_SS_PIN
#if defined(ARDUINO_ARCHLINK)
#define SDCARD_SS_PIN 4
#else
#define SDCARD_SS_PIN 4
#endif
#endif

#define UPDATE_FILE "UPDATE.BIN"

FlashClass flash;

void setup() {

  if (SD.begin(SDCARD_SS_PIN) && SD.exists(UPDATE_FILE)) {

    File updateFile = SD.open(UPDATE_FILE);
    uint32_t updateSize = updateFile.size();
    bool updateFlashed = false;

    if (updateSize > SDU_SIZE) {
      // skip the SDU section
      updateFile.seek(SDU_SIZE);
      updateSize -= SDU_SIZE;

      uint32_t* flashAddress = (uint32_t*)SKETCH_START;

      // erase the pages
      flash.erase(flashAddress, updateSize);

      uint32_t buffer[512];

      // write the pages
      for (uint32_t i = 0; i < updateSize; i += sizeof(buffer)) {
        updateFile.read(buffer, sizeof(buffer));

        uint16_t size = (uint16_t)(sizeof(buffer) / sizeof(uint32_t));

        flash.write_block(flashAddress, buffer, size);

        flashAddress += size;
      }

      updateFlashed = true;
    }
    
    updateFile.close();

    if (updateFlashed) {
      SD.remove(UPDATE_FILE);
    }
  }

  // jump to the sketch
  __set_MSP(*SKETCH_START);

  // address of Reset_Handler is written by the linker at the beginning of the .text section (see linker script)
  uint32_t resetHandlerAddress = (uint32_t) * (SKETCH_START + 1);
  // jump to reset handler
  asm("bx %0"::"r"(resetHandlerAddress));
}


void loop() {

}
