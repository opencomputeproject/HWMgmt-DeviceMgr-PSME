/*
* Edgecore DeviceManager
* Copyright 2020-2021 Edgecore Networks, Inc.
 *
* Licensed to the Apache Software Foundation (ASF) under one
* or more contributor license agreements.  See the NOTICE file
* distributed with this work for additional information
* regarding copyright ownership.  The ASF licenses this file
* to you under the Apache License, Version 2.0 (the
* "License"); you may not use this file except in compliance
* with the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
* Unless required by applicable law or agreed to in writing,
* software distributed under the License is distributed on an
* "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
* KIND, either express or implied.  See the License for the
* specific language governing permissions and limitations
* under the License.
 */

#ifndef EC_RF_PAL_HELPER_API_HPP
#define EC_RF_PAL_HELPER_API_HPP

#include "ecrf_pal_com.hpp"
using namespace ecrf_pal_com_s;
namespace ecrf_pal_com_api
{
int rfpal_sys_info_get(rfpal_sys_info *p_sys_info);

int rfpal_fan_num_get();
int rfpal_fan_info_get(unsigned id, rfpal_fan_info *p_fan_info);
int rfpal_fan_speed_thresholds_set(unsigned id, rfpal_thresholds_t threshhold_info);
int rfpal_fan_speed_thresholds_get(unsigned id, rfpal_thresholds_t *threshhold_info);

int rfpal_port_num_get();
int rfpal_port_trans_is_present(unsigned id);
int rfpal_sfp_tx_disable_set(unsigned id, int value);
int rfpal_sfp_tx_disable_get(unsigned id);
int rfpal_sfp_tx_eeprom_get(unsigned int id, char *p_eeprom, int size);

int rfpal_thermal_num_get();
int rfpal_thermal_info_get(unsigned id, rfpal_thermal_info *p_thermal_info);
int rfpal_thermal_thresholds_set(unsigned id, rfpal_thresholds_t threshhold_info);
int rfpal_thermal_thresholds_get(unsigned id, rfpal_thresholds_t *threshhold_info);

int rfpal_sys_resource_thresholds_get(ecrf_sys_threshold_info_t type, rfpal_thresholds *threshold_info);
int rfpal_sys_resource_thresholds_set(ecrf_sys_threshold_info_t type, rfpal_thresholds threshold_info);

int rfpal_psu_num_get();
int rfpal_psu_info_get(unsigned id, rfpal_psu_info *p_psu_info);
} // namespace ecrf_pal_com_api
#endif
