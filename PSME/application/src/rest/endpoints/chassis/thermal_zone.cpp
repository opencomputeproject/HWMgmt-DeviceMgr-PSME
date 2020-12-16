/*!
 * @copyright
 * Copyright (c) 2015-2017 Intel Corporation
 *
 * @copyright
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * @copyright
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * @copyright
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Edgecore DeviceManager
 * Copyright 2020-2021 Edgecore Networks, Inc.
 *
 * This product includes software developed at
 * Edgecore Networks Inc. (http://www.edge-core.com/).
 *
 * */

#include "psme/rest/endpoints/chassis/thermal_zone.hpp"
#include "psme/rest/constants/constants.hpp"
#include "psme/rest/endpoints/utils.hpp"

using namespace psme::rest;
using namespace psme::rest::constants;

namespace
{
json::Value make_prototype()
{
    json::Value r(json::Value::Type::OBJECT);
    json::Value rs;
    rs[Common::ODATA_TYPE] = "#Thermal.v1_0_0.Thermal";
    rs[ThermalZone::PHYSICAL_CONTEXT] = json::Value::Type::NIL;
	rs[constants::ThermalZoneCollection::UPPER_THRESHOLD_CRITICAL] = 45; //Hard code//
    rs[ThermalZone::READINGCELSIUS] = "0";	
	r[ChassisThreshold::TEMPERATURES] = std::move(rs);
    return r;
}
} // namespace

endpoint::ThermalZone::ThermalZone(const std::string& path) : EndpointBase(path) {}

endpoint::ThermalZone::~ThermalZone() {}

#define THERMAL_CRITICAL 55000
#define THERMAL_WARNING 45000

void endpoint::ThermalZone::get(const server::Request &req, server::Response &res)
{
	auto json = ::make_prototype();

	json[Common::ODATA_ID] = PathBuilder(req).build();

	/*Fill real time data into JSON format*/

	auto req_tz_id =  psme::rest::model::Find<agent_framework::model::ThermalZone>(req.params[PathParam::THERMAL_ID]).get().get_id();

	auto& tz_manager = agent_framework::module::ChassisComponents::get_instance()->get_thermal_zone_manager();
	auto tz_uuids = tz_manager.get_keys();
	for (const auto &tz_uuid : tz_uuids)
	{
		auto tz_ = tz_manager.get_entry(tz_uuid);  //Get TZ object by fan_uuid//

		if (tz_.get_tz_id() == req_tz_id)
		{
			json[ChassisThreshold::TEMPERATURES][Common::MEMBER_ID] = std::to_string(req_tz_id);
			json[ChassisThreshold::TEMPERATURES][constants::ThermalZoneCollection::READING_CELSIUS] = (tz_.get_temperature() * 0.001);

			if(tz_.get_temperature() == -1)
			{
				json[ChassisThreshold::TEMPERATURES][Common::STATUS][Common::HEALTH] = json::Value::Type::NIL;
				json[ChassisThreshold::TEMPERATURES][Common::STATUS][Common::STATE] = "Absent";

				json[ChassisThreshold::TEMPERATURES][constants::ThermalZoneCollection::READING_CELSIUS] = "0";
			}
			else
			{
				if((tz_.get_temperature() < 0) || ((tz_.get_temperature() < THERMAL_CRITICAL) && (tz_.get_temperature() >= THERMAL_WARNING)))
				{
					json[ChassisThreshold::TEMPERATURES][Common::STATUS][Common::HEALTH] = "Warning";
					json[ChassisThreshold::TEMPERATURES][Common::STATUS][Common::STATE] = "Enabled";
				}
				else if((tz_.get_temperature() < 60000) && ((tz_.get_temperature() >= THERMAL_CRITICAL)))
				{
					json[ChassisThreshold::TEMPERATURES][Common::STATUS][Common::HEALTH] = "Warning";
					json[ChassisThreshold::TEMPERATURES][Common::STATUS][Common::STATE] = "Enabled";
				}
				else if((tz_.get_temperature() >= 60000))
				{
					json[ChassisThreshold::TEMPERATURES][Common::STATUS][Common::HEALTH] = "Critical";
					json[ChassisThreshold::TEMPERATURES][Common::STATUS][Common::STATE] = "Enabled";
				}
				else
				{
					json[ChassisThreshold::TEMPERATURES][Common::STATUS][Common::HEALTH] = "OK";
					json[ChassisThreshold::TEMPERATURES][Common::STATUS][Common::STATE] = "Enabled";
				}
			}
		}
	}

	set_response(res, json);
}

/*This content show up in Brower query*/

namespace
{
json::Value make_fan_prototype()
{
	
    json::Value r(json::Value::Type::OBJECT);
    json::Value rs;
    rs[Common::ODATA_TYPE] = "#Thermal.v1_0_0.Thermal";
    rs[ThermalZone::PHYSICAL_CONTEXT] = json::Value::Type::NIL;
    rs[ThermalZone::READING_UNITS] = "rpm";
    rs[ThermalZone::READING] = "0";	
	r[constants::Root::FANS] = std::move(rs);

    return r;
}
} // namespace

endpoint::Fan::Fan(const std::string& path) : EndpointBase(path) {}

endpoint::Fan::~Fan() {}

void endpoint::Fan::get(const server::Request &req, server::Response &res)
{

    auto json = ::make_fan_prototype();

    json[Common::ODATA_ID] = PathBuilder(req).build();

/*Fill real time data into JSON format */	

    auto req_fan_id =  psme::rest::model::Find<agent_framework::model::Fan>(req.params[PathParam::FAN_ID]).get().get_id();

    auto& fan_manager = agent_framework::module::ChassisComponents::get_instance()->get_fan_manager();
    auto fan_uuids = fan_manager.get_keys();
	for (const auto &fan_uuid : fan_uuids)
	{

      auto fan_ = fan_manager.get_entry(fan_uuid);  //Get Fan object by fan_uuid//

		if (fan_.get_fan_id() == req_fan_id)
		{
			json[constants::Root::FANS][Common::ID] = req_fan_id;
			json[constants::Root::FANS][constants::ThermalZoneCollection::READING] = fan_.get_current_speed();

  		  if(fan_.get_current_speed() < 0)
  		  {
				json[constants::Root::FANS][Common::STATUS][Common::STATE] = "Absent";
				json[constants::Root::FANS][Common::STATUS][Common::HEALTH] = json::Value::Type::NIL;
				json[constants::Root::FANS][constants::ThermalZoneCollection::READING] = "0";
  		  }
  		  else
  		  {
				json[constants::Root::FANS][Common::STATUS][Common::STATE] = "Enabled";
				json[constants::Root::FANS][Common::STATUS][Common::HEALTH] = "OK";
  		  }
      }
    }
    set_response(res, json);
}

namespace
{
json::Value make_psu_prototype()
{
	
    json::Value r(json::Value::Type::OBJECT);
    json::Value rs;
    rs[Common::ODATA_TYPE] = "Power.v1_0_0.PowerSupply";
    rs[PowerZone::POWER_CONSUMED] = json::Value::Type::NIL;
	r[PowerZone::POWER_SUPPLY] = std::move(rs);
    return r;
}
} // namespace

endpoint::Psu::Psu(const std::string& path) : EndpointBase(path) {}

endpoint::Psu::~Psu() {}

void endpoint::Psu::get(const server::Request &req, server::Response &res)
{
    auto json = ::make_psu_prototype();
    json[Common::ODATA_ID] = PathBuilder(req).build();

/*Fill real time data into JSON format*/
    auto req_psu_id =  psme::rest::model::Find<agent_framework::model::Psu>(req.params[PathParam::PSU_ID]).get().get_id();

    auto& psu_manager = agent_framework::module::ChassisComponents::get_instance()->get_psu_manager();
    auto psu_uuids = psu_manager.get_keys();
	for (const auto &psu_uuid : psu_uuids)
	{
    auto psu_ = psu_manager.get_entry(psu_uuid);  //Get Fan object by fan_uuid//

		if (psu_.get_psu_id() == req_psu_id)
		{
			json[PowerZone::POWER_SUPPLY][Common::MEMBER_ID] = std::to_string(req_psu_id);
			json[PowerZone::POWER_SUPPLY][PowerZone::POWER_CONSUMED] = (psu_.get_power_output() * 0.001);
        }
    }
    set_response(res, json);
}
