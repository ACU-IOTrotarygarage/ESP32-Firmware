/*
  xsns_18_pms5003.ino - PMS3003, PMS5003, PMS7003 particle concentration sensor support for Firmware

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

#ifdef USE_PMS5003
/*********************************************************************************************\
 * PlanTower PMS3003, PMS5003, PMS7003 particle concentration sensor
 * For background information see http://aqicn.org/sensor/pms5003-7003/ or
 * http://aqicn.org/sensor/pms3003/
 *
 * Hardware Serial will be selected if GPIO3 = [PMS5003]
 * You can either support PMS3003 or PMS5003-7003 at one time. To enable the PMS3003 support
 * you must enable the define PMS_MODEL_PMS3003 on your configuration file.
 * For PMSx003T models that report temperature and humidity define PMS_MODEL_PMS5003T
\*********************************************************************************************/

#define XSNS_18             18

#include <FirmwareSerial.h>

#ifndef WARMUP_PERIOD
#define WARMUP_PERIOD 30          // Turn on PMSX003 XX-seconds before read in passive mode
#endif

#ifndef MIN_INTERVAL_PERIOD
#define MIN_INTERVAL_PERIOD 60    // minimum interval period in seconds required for passive mode
#endif

FirmwareSerial *PmsSerial;

struct PMS5003 {
  uint16_t time = 0;
  uint8_t type = 1;
  uint8_t valid = 0;
  uint8_t wake_mode = 1;
  uint8_t ready = 1;
  bool discovery_triggered = false;
} Pms;

enum PmsCommands
{
  CMD_MODE_ACTIVE,
  CMD_SLEEP,
  CMD_WAKEUP,
  CMD_MODE_PASSIVE,
  CMD_READ_DATA
};

const uint8_t kPmsCommands[][7] PROGMEM = {
    //  0     1    2    3     4     5     6
    {0x42, 0x4D, 0xE1, 0x00, 0x01, 0x01, 0x71},  // pms_set_active_mode
    {0x42, 0x4D, 0xE4, 0x00, 0x00, 0x01, 0x73},  // pms_sleep
    {0x42, 0x4D, 0xE4, 0x00, 0x01, 0x01, 0x74},  // pms_wake
    {0x42, 0x4D, 0xE1, 0x00, 0x00, 0x01, 0x70},  // pms_set_passive_mode
    {0x42, 0x4D, 0xE2, 0x00, 0x00, 0x01, 0x71}}; // pms_passive_mode_read

struct pmsX003data {
  uint16_t framelen;
  uint16_t pm10_standard, pm25_standard, pm100_standard;
  uint16_t pm10_env, pm25_env, pm100_env;
#ifdef PMS_MODEL_PMS3003
  uint16_t reserved1, reserved2, reserved3;
#else
  uint16_t particles_03um, particles_05um, particles_10um, particles_25um;
#ifdef PMS_MODEL_PMS5003T
  uint16_t temperature10x, humidity10x;
#else
  uint16_t particles_50um, particles_100um;
#endif // PMS_MODEL_PMS5003T
  uint16_t unused;
#endif  // PMS_MODEL_PMS3003
  uint16_t checksum;
} pms_data;

/*********************************************************************************************/

size_t PmsSendCmd(uint8_t command_id)
{
  return PmsSerial->write(kPmsCommands[command_id], sizeof(kPmsCommands[command_id]));
}

/*********************************************************************************************/

bool PmsReadData(void)
{
  if (! PmsSerial->available()) {
    return false;
  }
  while ((PmsSerial->peek() != 0x42) && PmsSerial->available()) {
    PmsSerial->read();
  }
#ifdef PMS_MODEL_PMS3003
  if (PmsSerial->available() < 24) {
#else
  if (PmsSerial->available() < 32) {
#endif  // PMS_MODEL_PMS3003
    return false;
  }

#ifdef PMS_MODEL_PMS3003
  uint8_t buffer[24];
  PmsSerial->readBytes(buffer, 24);
#else
  uint8_t buffer[32];
  PmsSerial->readBytes(buffer, 32);
#endif  // PMS_MODEL_PMS3003
  uint16_t sum = 0;
  PmsSerial->flush();  // Make room for another burst

#ifdef PMS_MODEL_PMS3003
  AddLogBuffer(LOG_LEVEL_DEBUG_MORE, buffer, 24);
#else
  AddLogBuffer(LOG_LEVEL_DEBUG_MORE, buffer, 32);
#endif  // PMS_MODEL_PMS3003

  // get checksum ready
#ifdef PMS_MODEL_PMS3003
  for (uint32_t i = 0; i < 22; i++) {
#else
  for (uint32_t i = 0; i < 30; i++) {
#endif  // PMS_MODEL_PMS3003
    sum += buffer[i];
  }
  // The data comes in endian'd, this solves it so it works on all platforms
#ifdef PMS_MODEL_PMS3003
  uint16_t buffer_u16[12];
  for (uint32_t i = 0; i < 12; i++) {
#else
  uint16_t buffer_u16[15];
  for (uint32_t i = 0; i < 15; i++) {
#endif  // PMS_MODEL_PMS3003
    buffer_u16[i] = buffer[2 + i*2 + 1];
    buffer_u16[i] += (buffer[2 + i*2] << 8);
  }
#ifdef PMS_MODEL_PMS3003
  if (sum != buffer_u16[10]) {
#else
  if (sum != buffer_u16[14]) {
#endif  // PMS_MODEL_PMS3003
    AddLog(LOG_LEVEL_DEBUG, PSTR("PMS: " D_CHECKSUM_FAILURE));
    return false;
  }

#ifdef PMS_MODEL_PMS3003
  memcpy((void *)&pms_data, (void *)buffer_u16, 22);
#else
  memcpy((void *)&pms_data, (void *)buffer_u16, 30);
#endif  // PMS_MODEL_PMS3003
  Pms.valid = 10;

  if (!Pms.discovery_triggered) {
    FirmwareGlobal.discovery_counter = 1;      // Force discovery
    Pms.discovery_triggered = true;
  }

  return true;
}

/*********************************************************************************************\
 * Command Sensor18
 *
 * Warmup time for sensor is 30 seconds, therfore setting interval time to less than 60
 * seconds doesn't really make sense.
 *
 * 0 - 59   -   Active Mode (continuous sensor readings)
 * 60 .. 65535 -  Passive Mode (read sensor every x seconds)
\*********************************************************************************************/

bool PmsCommandSensor(void)
{
  if (PinUsed(GPIO_PMS5003_TX) && (XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 32001)) {
    if (XdrvMailbox.payload < MIN_INTERVAL_PERIOD) {
      // Set Active Mode if interval is less than 60 seconds
      Settings->pms_wake_interval = 0;
      Pms.wake_mode = 1;
      Pms.ready = 1;
      PmsSendCmd(CMD_MODE_ACTIVE);
      PmsSendCmd(CMD_WAKEUP);
    } else {
      // Set Passive Mode and schedule read once per interval time
      Settings->pms_wake_interval = XdrvMailbox.payload;
      PmsSendCmd(CMD_MODE_PASSIVE);
      PmsSendCmd(CMD_SLEEP);
      Pms.wake_mode = 0;
      Pms.ready = 0;
    }
  }

  Response_P(S_JSON_SENSOR_INDEX_NVALUE, XSNS_18, Settings->pms_wake_interval);

  return true;
}

/*********************************************************************************************/

void PmsSecond(void)                 // Every second
{
  if (Settings->pms_wake_interval >= MIN_INTERVAL_PERIOD) {
    // Passive Mode
    Pms.time++;
    if ((Settings->pms_wake_interval - Pms.time <= WARMUP_PERIOD) && !Pms.wake_mode) {
      // wakeup sensor WARMUP_PERIOD before read interval
      Pms.wake_mode = 1;
      PmsSendCmd(CMD_WAKEUP);
    }
    if (Pms.time >= Settings->pms_wake_interval) {
      // sensor is awake and warmed up, set up for reading
      PmsSendCmd(CMD_READ_DATA);
      Pms.ready = 1;
      Pms.time = 0;
    }
  }

  if (Pms.ready) {
    if (PmsReadData()) {
      Pms.valid = 10;
      if (Settings->pms_wake_interval >= MIN_INTERVAL_PERIOD) {
        PmsSendCmd(CMD_SLEEP);
        Pms.wake_mode = 0;
        Pms.ready = 0;
      }
    } else {
      if (Pms.valid) {
        Pms.valid--;
        if (Settings->pms_wake_interval >= MIN_INTERVAL_PERIOD) {
          PmsSendCmd(CMD_READ_DATA);
          Pms.ready = 1;
        }
      }
    }
  }
}

/*********************************************************************************************/

void PmsInit(void) {
  Pms.type = 0;
  if (PinUsed(GPIO_PMS5003_RX)) {
    PmsSerial = new FirmwareSerial(Pin(GPIO_PMS5003_RX), (PinUsed(GPIO_PMS5003_TX)) ? Pin(GPIO_PMS5003_TX) : -1, 1);
    if (PmsSerial->begin(9600)) {
      if (PmsSerial->hardwareSerial()) { ClaimSerial(); }

      if (!PinUsed(GPIO_PMS5003_TX)) {  // setting interval not supported if TX pin not connected
        Settings->pms_wake_interval = 0;
        Pms.ready = 1;
      } else {
        if (Settings->pms_wake_interval >= MIN_INTERVAL_PERIOD) {
          // Passive Mode
          PmsSendCmd(CMD_MODE_PASSIVE);
          Pms.wake_mode = 0;
          Pms.ready = 0;
          Pms.time = Settings->pms_wake_interval - WARMUP_PERIOD; // Let it wake up in the next second
        }
      }

      Pms.type = 1;
    }
  }
}

void PmsShow(bool json) {
  if (Pms.valid) {
    char types[10];
#ifdef PMS_MODEL_PMS3003
    strcpy_P(types, PSTR("PMS3003"));
#elif defined(PMS_MODEL_PMS5003T)
    strcpy_P(types, PSTR("PMS5003T"));
#else
    strcpy_P(types, PSTR("PMS5003"));
#endif

#ifdef PMS_MODEL_PMS5003T
    float temperature = ConvertTemp(pms_data.temperature10x/10.0);
    float humidity = ConvertHumidity(pms_data.humidity10x/10.0);
#endif // PMS_MODEL_PMS5003T

    if (json) {
      ResponseAppend_P(PSTR(",\"%s\":{\"CF1\":%d,\"CF2.5\":%d,\"CF10\":%d,\"PM1\":%d,\"PM2.5\":%d,\"PM10\":%d"),
        types,
        pms_data.pm10_standard, pms_data.pm25_standard, pms_data.pm100_standard,
        pms_data.pm10_env, pms_data.pm25_env, pms_data.pm100_env);
#ifndef PMS_MODEL_PMS3003
      ResponseAppend_P(PSTR(",\"PB0.3\":%d,\"PB0.5\":%d,\"PB1\":%d,\"PB2.5\":%d,"),
        pms_data.particles_03um, pms_data.particles_05um, pms_data.particles_10um, pms_data.particles_25um);
#ifdef PMS_MODEL_PMS5003T
      ResponseAppendTHD(temperature, humidity);
#else
      ResponseAppend_P(PSTR("\"PB5\":%d,\"PB10\":%d"),
        pms_data.particles_50um, pms_data.particles_100um);
#endif  // PMS_MODEL_PMS5003T
#endif  // No PMS_MODEL_PMS3003
      ResponseJsonEnd();

#ifdef USE_DOMOTICZ
      if (0 == FirmwareGlobal.tele_period) {
        DomoticzSensor(DZ_COUNT, pms_data.pm10_env);     // PM1
        DomoticzSensor(DZ_VOLTAGE, pms_data.pm25_env);   // PM2.5
        DomoticzSensor(DZ_CURRENT, pms_data.pm100_env);  // PM10
      }
#endif  // USE_DOMOTICZ
#ifdef USE_WEBSERVER
    } else {
//      WSContentSend_PD(HTTP_SNS_STANDARD_CONCENTRATION, types, "1", pms_data.pm10_standard);
//      WSContentSend_PD(HTTP_SNS_STANDARD_CONCENTRATION, types, "2.5", pms_data.pm25_standard);
//      WSContentSend_PD(HTTP_SNS_STANDARD_CONCENTRATION, types, "10", pms_data.pm100_standard);
      WSContentSend_PD(HTTP_SNS_ENVIRONMENTAL_CONCENTRATION, types, "1", pms_data.pm10_env);
      WSContentSend_PD(HTTP_SNS_ENVIRONMENTAL_CONCENTRATION, types, "2.5", pms_data.pm25_env);
      WSContentSend_PD(HTTP_SNS_ENVIRONMENTAL_CONCENTRATION, types, "10", pms_data.pm100_env);
#ifndef PMS_MODEL_PMS3003
      WSContentSend_PD(HTTP_SNS_PARTICALS_BEYOND, types, "0.3", pms_data.particles_03um);
      WSContentSend_PD(HTTP_SNS_PARTICALS_BEYOND, types, "0.5", pms_data.particles_05um);
      WSContentSend_PD(HTTP_SNS_PARTICALS_BEYOND, types, "1", pms_data.particles_10um);
      WSContentSend_PD(HTTP_SNS_PARTICALS_BEYOND, types, "2.5", pms_data.particles_25um);
#ifdef PMS_MODEL_PMS5003T
      WSContentSend_THD(types, temperature, humidity);
#else
      WSContentSend_PD(HTTP_SNS_PARTICALS_BEYOND, types, "5", pms_data.particles_50um);
      WSContentSend_PD(HTTP_SNS_PARTICALS_BEYOND, types, "10", pms_data.particles_100um);
#endif  // PMS_MODEL_PMS5003T
#endif  // No PMS_MODEL_PMS3003
#endif  // USE_WEBSERVER
    }
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns18(uint32_t function)
{
  bool result = false;

  if (Pms.type) {
    switch (function) {
      case FUNC_INIT:
        PmsInit();
        break;
      case FUNC_EVERY_SECOND:
        PmsSecond();
        break;
      case FUNC_COMMAND_SENSOR:
        if (XSNS_18 == XdrvMailbox.index) {
          result = PmsCommandSensor();
        }
        break;
      case FUNC_JSON_APPEND:
        PmsShow(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        PmsShow(0);
        break;
#endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_PMS5003
