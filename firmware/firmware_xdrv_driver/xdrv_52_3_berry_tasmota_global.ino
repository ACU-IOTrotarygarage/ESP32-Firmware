/*
  xdrv_52_3_berry_Firmware_global.ino - Berry scripting language, mapping to FirmwareGlobal

  Copyright (C) 2021 Stephan Hadinger, Berry language by Guan Wenliang https://github.com/Skiars/berry

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
/********************************************************************
 * Firmware LVGL ctypes mapping
 *******************************************************************/

#ifdef USE_BERRY

#include <berry.h>
#include "be_ctypes.h"

/********************************************************************
 * Generated code, don't edit
 *******************************************************************/

extern "C" {

  extern const be_ctypes_structure_t be_Firmware_global_struct = {
    sizeof(FirmwareGlobal),  /* size in bytes */
    8,  /* number of elements */
    nullptr,
    (const be_ctypes_structure_item_t[8]) {
      // Warning: fields below need to be in alphabetical order
      { "devices_present", offsetof(FirmwareGlobal_t, devices_present), 0, 0, ctypes_u8, 0 },
      { "fast_loop_enabled", offsetof(FirmwareGlobal_t, berry_fast_loop_enabled), 0, 0, ctypes_u8, 0 },
      { "masterlog_level", offsetof(FirmwareGlobal_t, masterlog_level), 0, 0, ctypes_u8, 0 },
      { "restart_flag", offsetof(FirmwareGlobal_t, restart_flag), 0, 0, ctypes_u8, 0 },
      { "seriallog_level", offsetof(FirmwareGlobal_t, seriallog_level), 0, 0, ctypes_u8, 0 },
      { "sleep", offsetof(FirmwareGlobal_t, sleep), 0, 0, ctypes_u8, 0 },
      { "syslog_level", offsetof(FirmwareGlobal_t, syslog_level), 0, 0, ctypes_u8, 0 },
      { "templog_level", offsetof(FirmwareGlobal_t, templog_level), 0, 0, ctypes_u8, 0 },
  }};

  extern const be_ctypes_structure_t be_Firmware_settings_struct = {
    sizeof(TSettings),  /* size in bytes */
    6,  /* number of elements */
    nullptr,
    (const be_ctypes_structure_item_t[6]) {
      // Warning: fields below need to be in alphabetical order
      { "bootcount", offsetof(TSettings, bootcount), 0, 0, ctypes_u16, 0 },
      { "mqttlog_level", offsetof(TSettings, mqttlog_level), 0, 0, ctypes_u8, 0 },
      { "seriallog_level", offsetof(TSettings, seriallog_level), 0, 0, ctypes_u8, 0 },
      { "sleep", offsetof(TSettings, sleep), 0, 0, ctypes_u8, 0 },
      { "syslog_level", offsetof(TSettings, syslog_level), 0, 0, ctypes_u8, 0 },
      { "weblog_level", offsetof(TSettings, weblog_level), 0, 0, ctypes_u8, 0 },
  }};

}

#endif // USE_BERRY
