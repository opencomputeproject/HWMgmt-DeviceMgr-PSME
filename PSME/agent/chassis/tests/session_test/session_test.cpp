/*!
 * @brief Unit tests for generation of UUIDv5
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

#include "tree_stability/chassis_tree_stabilizer.hpp"
#include "agent-framework/module/chassis_components.hpp"
#include "agent-framework/module/common_components.hpp"
#include "agent-framework/service_uuid.hpp"

#include "gtest/gtest.h"

#include <string>

class TestClass1 : public ::testing::Test {
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
    printf("TestClass1-SetUp\r\n");
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