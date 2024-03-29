/*!
 * @section LICENSE
 *
 * @copyright
 * Copyright (c) 2016-2017 Intel Corporation
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
 * @section DESCRIPTION
 */

#pragma once

#include "agent-framework/generic/singleton.hpp"
#include "agent-framework/module/enum/common.hpp"
#include "table_interface.hpp"
#include <list>
#include <functional>


namespace agent_framework {
namespace module {

class GenericManagerRegistry final : public generic::Singleton<GenericManagerRegistry> {
public:
    virtual ~GenericManagerRegistry();
    template <typename T>
    void register_table(T* table);

    using TableClientFunc = std::function<bool(TableInterface*)>;
    void for_each_table(TableClientFunc func);
protected:
    std::list<TableInterface*> tables {};
};

template <typename T>
void GenericManagerRegistry::register_table(T* table) {
    tables.push_back(table);
}

}
}
