/*
 * Copyright (c) 2017, George Oikonomou - http://www.spd.gr
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/*---------------------------------------------------------------------------*/
#include "contiki.h"
#include "batmon-sensor.h"
#include "board-peripherals.h"
#include "sisp.h"
#include "mqtt-client.h"

#include <string.h>
#include <stdio.h>
/*---------------------------------------------------------------------------*/
// Define parameters for sisp

#define START_DELAY 20
#define SEND_INTERVAL 1280
#define DELTA 128
/*---------------------------------------------------------------------------*/
#define TMP_BUF_SZ 32
/*---------------------------------------------------------------------------*/
char tmp_buf[TMP_BUF_SZ];
/*---------------------------------------------------------------------------*/
static char *
temp_reading(void)
{
  memset(tmp_buf, 0, TMP_BUF_SZ);
  snprintf(tmp_buf, TMP_BUF_SZ, "\"On-Chip Temp (mC)\":%d",
           batmon_sensor.value(BATMON_SENSOR_TYPE_TEMP));
  return tmp_buf;
}
/*---------------------------------------------------------------------------*/
static void
temp_init(void)
{
  SENSORS_ACTIVATE(batmon_sensor);
}
/*---------------------------------------------------------------------------*/
const mqtt_client_extension_t builtin_sensors_batmon_temp = {
  temp_init,
  temp_reading,
};
/*---------------------------------------------------------------------------*/
static char *
volt_reading(void)
{
  memset(tmp_buf, 0, TMP_BUF_SZ);
  snprintf(tmp_buf, TMP_BUF_SZ, "\"Volt (mV)\":%d",
           (batmon_sensor.value(BATMON_SENSOR_TYPE_VOLT) * 125) >> 5);
  return tmp_buf;
}
/*---------------------------------------------------------------------------*/
static void
volt_init(void)
{
  SENSORS_ACTIVATE(batmon_sensor);
}
/*---------------------------------------------------------------------------*/
const mqtt_client_extension_t builtin_sensors_batmon_volt = {
  volt_init,
  volt_reading,
};
/*---------------------------------------------------------------------------*/
static void
ext_temp_init(void)
{
  SENSORS_ACTIVATE(hdc_1000_sensor);
}
/*---------------------------------------------------------------------------*/
static char *
ext_temp_reading(void)
{
  memset(tmp_buf, 0, TMP_BUF_SZ);
  if (hdc_1000_sensor.status(SENSORS_READY) ==  3)
  {
    snprintf(tmp_buf, TMP_BUF_SZ, "\"ext Temp (mC)\":%d",
           hdc_1000_sensor.value(HDC_1000_SENSOR_TYPE_TEMP));
    ext_temp_init();
  } else {
    snprintf(tmp_buf, TMP_BUF_SZ, "\"ext Temp (mC)\":not ready");
  }
  return tmp_buf;
}
/*---------------------------------------------------------------------------*/
const mqtt_client_extension_t builtin_sensors_hdc_1000_temp = {
  ext_temp_init,
  ext_temp_reading,
};
/*---------------------------------------------------------------------------*/
static void
shared_clk_init(void)
{
  sisp_init(START_DELAY,SEND_INTERVAL,DELTA);
}
/*---------------------------------------------------------------------------*/
static char *
shared_clock_reading(void)
{
  clock_time_t s_clk;
  sisp_get_sCLK(&s_clk);
  memset(tmp_buf, 0, TMP_BUF_SZ);
  snprintf(tmp_buf, TMP_BUF_SZ, "\"shared CLK \":%lu",
           s_clk);
  return tmp_buf;
}
/*---------------------------------------------------------------------------*/
const mqtt_client_extension_t builtin_sensors_sisp_shared_clk = {
  shared_clk_init,
  shared_clock_reading,
};
/*---------------------------------------------------------------------------*/
