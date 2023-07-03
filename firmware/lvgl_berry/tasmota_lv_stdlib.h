/*
  Firmware_lv_stdlib.h - internationalization for Firmware
  Copyright (C) 2021  Theo Arends and Mahmoud Shams El-Din
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

// replacement for <stdlib.h> used for PSRAM malloc in LVGL

extern void *lvbe_malloc(size_t size);
extern void  lvbe_free(void *ptr);
extern void *lvbe_realloc(void *ptr, size_t size); 
