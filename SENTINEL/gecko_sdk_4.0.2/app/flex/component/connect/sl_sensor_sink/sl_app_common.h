/***************************************************************************//**
 * @file
 * @brief sl_app_common.h
 *******************************************************************************
 * # License
 * <b>Copyright 2018 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Zlib
 *
 * The licensor of this software is Silicon Laboratories Inc.
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 ******************************************************************************/
#ifndef __APP_COMMON__
#define __APP_COMMON__

// -----------------------------------------------------------------------------
//                                   Includes
// -----------------------------------------------------------------------------
#include PLATFORM_HEADER
#include "stack/include/ember.h"
#include "hal/hal.h"
#include "em_chip.h"

// -----------------------------------------------------------------------------
//                              Macros and Typedefs
// -----------------------------------------------------------------------------
/// Default endpoint for Sensor and Sink apps
#define SENSOR_SINK_ENDPOINT    (1)
/// Default TX power level and IDs used for the network.
#define SENSOR_SINK_TX_POWER    0
#define SENSOR_SINK_PAN_ID      0x01FF
/// Relevant offsets and field length within a message.
#define SENSOR_SINK_DATA_OFFSET 0
#define SENSOR_SINK_DATA_LENGTH 8
/// The default security key to use for communications
#define SENSOR_SINK_SECURITY_KEY    { 0x68, 0xf0, 0x24, 0x4f, 0xf6, 0x27, 0x78, 0x67, 0xd2, 0x58, 0xb6, 0x7e, 0x15, 0xda, 0x34, 0x45}

#if defined(SINK_ROLE)
#define SENSOR_REPORT_PERIOD_MS      (1 * MILLISECOND_TICKS_PER_SECOND)
#define SENSOR_TIMEOUT_MS            (60 * MILLISECOND_TICKS_PER_SECOND)
#endif
// -----------------------------------------------------------------------------
//                                Global Variables
// -----------------------------------------------------------------------------
/// Connect security key set up by CLI (or the default key)
extern EmberKeyData security_key;
/// TX options set up for the network
extern EmberMessageOptions tx_options;

#if defined(SENSOR_ROLE)
/// report timing event control
extern uint16_t sensor_report_period_ms;
/// report timing event control
extern EmberEventControl *report_control;
#endif
// -----------------------------------------------------------------------------
//                          Public Function Declarations
// -----------------------------------------------------------------------------
#endif // __APP_COMMON__
