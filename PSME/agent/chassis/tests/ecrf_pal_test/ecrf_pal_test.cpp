/*!
 * @brief Unit tests for edge core redfish PAL library and related system tools 
 *
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
 * @file chassis_tree_stabilizer_test.cpp
 * */

#include "gtest/gtest.h"
#include <spdlog/spdlog.h>
#include <ecrf_pal_helper/api/ecrf_pal_helper.hpp>
#include <ecsys_helper/ecsys_helper.hpp>
#include <ecnet_helper/ecnet_helper.hpp>
#include "spdlog/sinks/rotating_file_sink.h"

using namespace ecrf_pal_helper;
using namespace ecsys_helper;
using namespace std;

#include <string>
#include <iostream>
#include <chrono>
#include <ctime>  
#include <thread>

class TestClass1 : public ::testing::Test
{
public:
    virtual ~TestClass1();

    virtual void SetUp();

    virtual void TearDown();
};

TestClass1::~TestClass1() 
{ 
    printf("TestClass1-deconstructor\r\n");
}

void TestClass1::SetUp()
{
    //while(1)
    {
        DiskInfo disk_info;
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        printf("disk used percentage [%d]\r\n", (int)disk_info.get_sdx_used_percentage());
    }

    Json::Value AA;
    AA["AA"]="BB";
    AA["CC"]="DD";
    Json::FastWriter writer;

    // Create a file rotating logger with 5mb size max and 3 rotated files
    auto max_size = 1048576 * 5;
    auto max_files = 3;
    auto logger = spdlog::rotating_logger_mt("rf_log", "logs/rotating.txt", max_size, max_files);

    logger->info("this message should not appear in the console, only in the file");
    logger->info("Welcome to spdlog version {}.{}.{}  !", SPDLOG_VER_MAJOR, SPDLOG_VER_MINOR, SPDLOG_VER_PATCH);
    logger->warn("Easy padding in numbers like {:08d}", 12);
    logger->critical("Support for int: {0:d};  hex: {0:x};  oct: {0:o}; bin: {0:b}", 42);
    logger->info("Support for floats {:03.2f}", 1.23456);
    logger->info("Positional args are {1} {0}..", "too", "supported");
    logger->info("{:>8} aligned, {:<8} aligned", "right", "left");

    // Customize msg format for all loggers
    //logger->set_pattern("[%H:%M:%S %z] [%^%L%$] [thread %t] %v");
    logger->set_pattern("%v");
    logger->info("This an info message with custom format");
    //logger->set_pattern("%+"); // back to default format
    //logger->set_level(spdlog::level::info);
    // Backtrace support
    // Loggers can store in a ring buffer all messages (including debug/trace) for later inspection.
    // When needed, call dump_backtrace() to see what happened:

    for (int i = 0; i < 100; i++)
    {
        logger->debug(writer.write(AA));
    }
    Json::Value NewLog;
    NewLog["EntryType"] = "1";
    NewLog["OemRecordFormat"] = "NULL";
    NewLog["Severity"] = "3";
    NewLog["Created"] = "4";
    NewLog["EntryCode"] = "Assert";
    NewLog["SensorType"] = "5";
    NewLog["SensorNumber"] = "6";
    NewLog["Message"] = "67";
    logger->debug(writer.write(NewLog));

    int iii = 1;
    while (iii != 0)
    {
        CPU s1;
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        CPU s2;
        CPUStatsPrinter printer(s1, s2);

        printer.PrintActivePercentageTotal();

        printer.PrintActivePercentageAll();

        printer.PrintFullStatePercentageTotal();

        printer.PrintFullStatePercentageAll();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        iii--;
    }

    InterfaceConfig intf_ip;
    intf_ip.Restart();

    auto &Entry = RFLogEntry::get_instance();

    Entry.clean_log();
    Entry.get_current_time();
    Entry.get_zone_offset();
    Entry.get_max_entries_num();
    Entry.get_log_status();
    Entry.set_log_status(true);
    std::string event("Event");
    std::string servrity("Warning");
    std::string sensor_type("Temperature");
    std::string message;
    message = std::string("CPU Thermal is ") + std::to_string(1000) + std::string(" degrees.Over error temperature.");

    Entry.set_log_entry(event, sensor_type, servrity, message, 1);
    Entry.set_log_status(false);

    //Switch TESTs;
    auto &secrf_pal = Switch::Switch::get_instance();
    int ii = 1;
    int stress = 100;

    
    while (ii != 0)
    {
        auto start = std::chrono::system_clock::now();

        printf("////////////////Switch get port info. test/////////////////\r\n");
        ProcMemoryUsage MEM_PORT("GET PORT INFO. ");
        while (stress != 0)
        {
            MEM_PORT.mark_first();
        secrf_pal.get_port_present_info();
            MEM_PORT.check_diff();
            stress--;
            std::cout << '.' << std::flush;
        }
        stress = 100;

        printf("\r\n////////////////Switch get port oom info. test/////////////////\r\n");
        ProcMemoryUsage MEM_OOM("GET OOM INFO. ");
        while (stress != 0)
        {
            MEM_OOM.mark_first();
        secrf_pal.get_port_oom_info();
            MEM_OOM.check_diff();
            stress--;
            std::cout << '.' << std::flush;
        }
        stress = 30;
        auto end = std::chrono::system_clock::now();

        std::chrono::duration<double> elapsed_seconds = end - start;
        std::time_t end_time = std::chrono::system_clock::to_time_t(end);
        std::cout << "finished computation at getting get_port_info info.. " << std::ctime(&end_time)
                  << "elapsed time: " << elapsed_seconds.count() << "s\n";

        int port_max = secrf_pal.get_port_num();

        if (port_max == 0)
        {
            printf("You need check you have done HW_NODE_ASXXX Porting first!!!!\r\n");
            return;
        }
        printf("Port MAX [%d]\r\n", port_max);

        for (int i = 1; i <= port_max; i++)
        {
            printf("Port present %d [%d] \r\n", i, secrf_pal.get_port_info_by_(i, ecrf_pal_helper::Switch::Port_Present));
            secrf_pal.get_port_trans_info_by_(i);
        }
        start = std::chrono::system_clock::now();

        printf("\r\n////////////////Switch get psu info. test/////////////////\r\n");
        while (stress != 0)
        {
            ProcMemoryUsage MEM_PSU("GET PSU INFO. ");
            MEM_PSU.mark_first();
        secrf_pal.get_psu_info();
            MEM_PSU.check_diff();
            stress--;
            std::cout << '.'<< std::flush;
        }
        stress = 100;
        end = std::chrono::system_clock::now();
        elapsed_seconds = end - start;
        end_time = std::chrono::system_clock::to_time_t(end);
        std::cout << "finished computation at getting get_psu_info info.. " << std::ctime(&end_time)
                  << "elapsed time: " << elapsed_seconds.count() << "s\n";

        int psu_max = secrf_pal.get_psu_num();

        for (int i = 1; i <= psu_max; i++)
        {
            printf("PSU Model %d [%s] \r\n", i, secrf_pal.get_psu_info_by_(i, "Model").c_str());
            printf("PSU S/N     %d [%s] \r\n", i, secrf_pal.get_psu_info_by_(i, "SN").c_str());
            printf("PSU present %d [%d] \r\n", i, secrf_pal.get_psu_info_by_(i, ecrf_pal_helper::Switch::Psu_Present));
            printf("PSU Vin %d       [%d] \r\n", i, secrf_pal.get_psu_info_by_(i, ecrf_pal_helper::Switch::Vin));
            printf("PSU Vout %d       [%d] \r\n", i, secrf_pal.get_psu_info_by_(i, ecrf_pal_helper::Switch::Vout));
            printf("PSU Iin %d       [%d] \r\n", i, secrf_pal.get_psu_info_by_(i, ecrf_pal_helper::Switch::Iin));
            printf("PSU Iout %d       [%d] \r\n", i, secrf_pal.get_psu_info_by_(i, ecrf_pal_helper::Switch::Iout));
            printf("PSU Pin %d       [%d] \r\n", i, secrf_pal.get_psu_info_by_(i, ecrf_pal_helper::Switch::Pin));
            printf("PSU Pout %d       [%d] \r\n", i, secrf_pal.get_psu_info_by_(i, ecrf_pal_helper::Switch::Pout));
            printf("PSU Psu_type %d       [%d] \r\n", i, secrf_pal.get_psu_info_by_(i, ecrf_pal_helper::Switch::Psu_type));
            printf("PSU health %d [%s] \r\n", i, secrf_pal.get_psu_info_by_(i, "Status_Health").c_str());
            printf("PSU state %d [%s] \r\n", i, secrf_pal.get_psu_info_by_(i, "Status_State").c_str());
        }

        start = std::chrono::system_clock::now();

        auto &secrf_pal2 = ecrf_pal_helper::Switch::get_instance();
        printf("\r\n////////////////Switch get thermal info. test/////////////////\r\n");
        while (stress != 0)
        {
            ProcMemoryUsage MEM_THERMAL("GET THERMAL INFO.");
            MEM_THERMAL.mark_first();
        secrf_pal2.get_thermal_info();
            MEM_THERMAL.check_diff();
            stress--;
            std::cout << '.'<< std::flush;
        }
        stress = 100;

        end = std::chrono::system_clock::now();
        elapsed_seconds = end - start;
        end_time = std::chrono::system_clock::to_time_t(end);
        std::cout << "finished computation at getting get_thermal_info info.. " << std::ctime(&end_time)
                  << "elapsed time: " << elapsed_seconds.count() << "s\n";

        int thermal_max = secrf_pal2.get_thermal_num();

        for (int i = 1; i <= thermal_max; i++)
        {
            printf("Thermal type %d [%d] \r\n", i, (int)secrf_pal2.get_thermal_info_by_(i, ecrf_pal_helper::Switch::Thermal_Type));
            printf("Thermal health %d [%s] \r\n", i, secrf_pal2.get_thermal_info_by_(i, "Status_Health").c_str());
            printf("Thermal state %d [%s] \r\n", i, secrf_pal2.get_thermal_info_by_(i, "Status_State").c_str());
        }

        start = std::chrono::system_clock::now();
        auto &secrf_pal3 = ecrf_pal_helper::Switch::get_instance();
        printf("\r\n////////////////Switch get fan info. test/////////////////\r\n");

        stress = 100;

        while (stress != 0)
        {
            ProcMemoryUsage MEM_FAN("GET FAN INFO.");
            MEM_FAN.mark_first();
        secrf_pal3.get_fan_info();
            MEM_FAN.check_diff();
            stress--;
            std::cout << '.'<< std::flush;
        }
        std::cout << std::endl;
        stress = 100;

        end = std::chrono::system_clock::now();
        elapsed_seconds = end - start;
        end_time = std::chrono::system_clock::to_time_t(end);
        std::cout << "finished computation at getting get_fan_info info.. " << std::ctime(&end_time)
                  << "elapsed time: " << elapsed_seconds.count() << "s\n";

        int fan_max = secrf_pal2.get_fan_num();

        for (int i = 1; i <= fan_max; i++)
        {
            printf("fan type %d [%d] \r\n", i, secrf_pal3.get_fan_info_by_(i, ecrf_pal_helper::Switch::Type));
            printf("fan health %d [%s] \r\n", i, secrf_pal3.get_fan_info_by_(i, "Status_Health").c_str());
            printf("fan state %d [%s] \r\n", i, secrf_pal3.get_fan_info_by_(i, "Status_State").c_str());
        }
        ii--;
    }
}

void TestClass1::TearDown() 
{
    printf("TestClass1-TearDown\r\n");
}

TEST_F(TestClass1, Test_Memo_1) 
{
    // TEST 1 Content here //
    printf("TestClass1-content\r\n");
}
