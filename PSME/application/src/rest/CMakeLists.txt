# <license_header>
#
# Copyright (c) 2015-2017 Intel Corporation
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# Edgecore DeviceManager
# Copyright 2020-2021 Edgecore Networks, Inc.
#
# This product includes software developed at
# Edgecore Networks Inc. (http://www.edge-core.com/).
#
# </license_header>

add_subdirectory(metadata)

set(SOURCES
    server/mux/empty_matcher.cpp
    server/mux/segment_matcher.cpp
    server/mux/regex_matcher.cpp
    server/mux/static_matcher.cpp
    server/mux/variable_matcher.cpp
    server/mux/utils.cpp

    server/status.cpp
    server/response.cpp
    server/request.cpp
    server/parameters.cpp
    server/multiplexer.cpp
    server/server.cpp
    server/methods_handler.cpp
    server/content_types.cpp
    server/utils.cpp

    server/error/error_factory.cpp
    server/error/server_error.cpp
    server/error/server_exception.cpp
    server/error/message_object.cpp

    server/connector/connector.cpp
    server/connector/connector_factory.cpp
    server/connector/connector_options.cpp
    server/connector/microhttpd/mhd_connector_options.cpp
    server/connector/microhttpd/mhd_connector.cpp
    rest_server.cpp

    constants/constants.cpp
    constants/chassis.cpp
    constants/ethernet.cpp
    constants/storage.cpp
    constants/system.cpp
    constants/fabric.cpp
    constants/routes.cpp

    endpoints/endpoint_base.cpp
    endpoints/redfish.cpp
    endpoints/root.cpp
    endpoints/odata_service_document.cpp
    endpoints/rpvfoobar.cpp
    endpoints/metadata_root.cpp
    endpoints/metadata.cpp
    endpoints/utils.cpp
    endpoints/task_service_utils.cpp
    endpoints/path_builder.cpp
    endpoints/manager_collection.cpp
    endpoints/manager.cpp
    endpoints/network_protocol.cpp
    endpoints/manager_network_interface_collection.cpp
    endpoints/manager_serial_interface_collection.cpp
    endpoints/manager_serial_interface.cpp
    endpoints/manager_log_services_collection.cpp
    endpoints/manager_log_services.cpp
    endpoints/manager_log_services_entries.cpp
    endpoints/manager_log_services_entries_collection.cpp
    endpoints/manager_network_interface.cpp
    endpoints/test_event_subscription.cpp
    endpoints/event_service.cpp
    
    endpoints/account_service.cpp
    endpoints/account_accounts_collection.cpp
    endpoints/account_role_collection.cpp
    endpoints/account.cpp
    endpoints/role.cpp
    
    endpoints/event_subscriptions_collection.cpp
    endpoints/event_subscription.cpp
    endpoints/task_service.cpp
    endpoints/task_collection.cpp
    endpoints/session_collection.cpp    
    endpoints/session_service.cpp
    endpoints/sessions.cpp    
    endpoints/members.cpp    
    endpoints/task.cpp
    endpoints/message_registry_file_collection.cpp
    endpoints/message_privilege_registry_file.cpp
    endpoints/message_Accton_Firmware_Update_Registry_file.cpp
    endpoints/message_registry_file.cpp
    endpoints/monitor.cpp
    endpoints/monitor_content_builder.cpp

    endpoints/system/systems_collection.cpp
    endpoints/system/system.cpp
    endpoints/system/system_reset.cpp
    endpoints/system/log_reset.cpp
    endpoints/system/processors_collection.cpp
    endpoints/system/processor.cpp
    endpoints/system/storage_subsystems_collection.cpp
    endpoints/system/storage_subsystem.cpp
    endpoints/system/memory_collection.cpp
    endpoints/system/memory.cpp
    endpoints/system/system_network_interface_collection.cpp
    endpoints/system/system_network_interface.cpp
    endpoints/system/volumes_collection.cpp
    endpoints/system/network_interface.cpp
    endpoints/system/network_interfaces_collection.cpp
    endpoints/system/network_device_functions_collection.cpp
    endpoints/system/network_device_function.cpp

    endpoints/storage/physical_drives_collection.cpp
    endpoints/storage/physical_drive.cpp
    endpoints/storage/logical_drive_collection.cpp
    endpoints/storage/logical_drive.cpp
    endpoints/storage/storage_services.cpp
    endpoints/storage/storage_services_collection.cpp
    endpoints/storage/remote_target.cpp
    endpoints/storage/remote_target_collection.cpp

    endpoints/ethernet/vlan_network_interface_collection.cpp
    endpoints/ethernet/mgmt_vlan_network_interface_collection.cpp
    endpoints/ethernet/vlan_network_interface.cpp
    endpoints/ethernet/mgmt_vlan_network_interface.cpp
    endpoints/ethernet/ethernet_switch_collection.cpp
    endpoints/ethernet/ethernet_switch.cpp
    endpoints/ethernet/ethernet_switch_port_collection.cpp
    endpoints/ethernet/ethernet_switch_port_onus_collection.cpp
    endpoints/ethernet/ethernet_switch_port_onus.cpp
    endpoints/ethernet/ethernet_switch_port_onus_omci.cpp
    endpoints/ethernet/ethernet_switch_port.cpp
    endpoints/ethernet/olt_flow.cpp
    endpoints/ethernet/olt_flow_id.cpp
    endpoints/ethernet/acl.cpp
    endpoints/ethernet/acl_bind.cpp
    endpoints/ethernet/acl_collection.cpp
    endpoints/ethernet/rule.cpp
    endpoints/ethernet/rule_collection.cpp
    endpoints/ethernet/static_mac.cpp
    endpoints/ethernet/static_mac_collection.cpp

    endpoints/chassis/chassis_collection.cpp
    endpoints/chassis/chassis.cpp
    endpoints/chassis/thermal_zone.cpp
    endpoints/chassis/thermal_zones_collection.cpp
    endpoints/chassis/thermal_collection.cpp
    endpoints/chassis/temperature_collection.cpp
    endpoints/chassis/psu_collection.cpp
    endpoints/chassis/power_zone.cpp
    endpoints/chassis/power_zones_collection.cpp
    endpoints/chassis/drives_collection.cpp
    endpoints/chassis/drive.cpp
    endpoints/chassis/drive_secure_erase.cpp
    endpoints/chassis/pcie_device.cpp
    endpoints/chassis/pcie_devices_collection.cpp
    endpoints/chassis/pcie_function.cpp
    endpoints/chassis/pcie_functions_collection.cpp
    endpoints/chassis/chassis_reset.cpp
    endpoints/chassis/update_service.cpp
    endpoints/chassis/update_service_nos.cpp
    endpoints/chassis/update_service_mu.cpp

    endpoints/fabric/fabric.cpp
    endpoints/fabric/fabrics_collection.cpp
    endpoints/fabric/endpoint.cpp
    endpoints/fabric/endpoints_collection.cpp
    endpoints/fabric/switch.cpp
    endpoints/fabric/switches_collection.cpp
    endpoints/fabric/switch_reset.cpp
    endpoints/fabric/port.cpp
    endpoints/fabric/port_reset.cpp
    endpoints/fabric/ports_collection.cpp
    endpoints/fabric/zone.cpp
    endpoints/fabric/zones_collection.cpp

    endpoints/endpoint_builder.cpp

    endpoints/olt.cpp

    eventing/event.cpp
    eventing/event_service.cpp
    eventing/event_queue.cpp
    eventing/rest_client.cpp
    eventing/config/subscription_config.cpp
    eventing/model/subscription.cpp
    eventing/manager/subscription_manager.cpp
    http/http_defs.cpp

    account/config/account_config.cpp
    account/config/role_config.cpp
    account/model/accountservice.cpp
    account/model/account.cpp
    account/model/role.cpp
    account/manager/account_manager.cpp
    
    session/config/session_config.cpp
    session/model/sessionservice.cpp
    session/model/session.cpp
    session/manager/session_manager.cpp
    
    log/config/log_config.cpp
    log/model/logservice.cpp
    log/model/log.cpp
    log/manager/log_manager.cpp
    
    cache/cache.cpp

    model/watcher.cpp
    model/handlers/generic_handler.cpp
    model/handlers/handler_manager.cpp
    model/handlers/root_handler.cpp
    model/handlers/id_memoizer.cpp
    model/handlers/database.cpp
    model/handlers/file_database.cpp

    registries/config/registry_configurator.cpp
    registries/managers/message_registry_file_manager.cpp
    registries/managers/message_registry_manager.cpp
    registries/model/attributes/location.cpp
    registries/model/message_registry_file.cpp
    registries/model/message_registry.cpp

    utils/network_interface_info.cpp
    utils/time_utils.cpp
    utils/lag_utils.cpp
    utils/zone_utils.cpp
    utils/mapper.cpp
    utils/ec_common_utils.cpp

    validators/json_validator.cpp
    validators/schemas/common.cpp
    validators/schemas/accountservice.cpp
    validators/schemas/account_collection.cpp
    validators/schemas/account.cpp
    validators/schemas/session_collection.cpp
    validators/schemas/subscription_collection.cpp
    validators/schemas/subscription.cpp
    validators/schemas/endpoints_collection.cpp
    validators/schemas/ethernet_switch_port.cpp
    validators/schemas/ethernet_interface.cpp
    validators/schemas/ethernet_switch_port_collection.cpp
    validators/schemas/ethernet_switch_port_onus_collection.cpp
    validators/schemas/rule.cpp
    validators/schemas/static_mac.cpp
    validators/schemas/vlan_network_interface_collection.cpp
    validators/schemas/remote_target.cpp
    validators/schemas/remote_targets_collection.cpp
    validators/schemas/zone.cpp
    validators/schemas/zones_collection.cpp
    validators/schemas/drive.cpp
    validators/schemas/switch.cpp
    validators/schemas/storage_subsystem.cpp
    validators/schemas/chassis.cpp
    validators/schemas/olt.cpp
    validators/schemas/omci.cpp
    validators/schemas/flow.cpp
    validators/schemas/pcie_device.cpp
    validators/schemas/system.cpp
    validators/schemas/reset.cpp
    validators/schemas/logical_drive.cpp
    validators/schemas/network_device_function.cpp
    validators/schemas/sessionservice.cpp
    )

if (CMAKE_CXX_COMPILER_ID MATCHES GNU)
    set_source_files_properties(
        rest_server.cpp
        PROPERTIES COMPILE_FLAGS "-Wno-unsafe-loop-optimizations"
    )

    set_source_files_properties(
        server/error/error_factory.cpp
        PROPERTIES COMPILE_FLAGS "-Wno-suggest-attribute=format"
    )
endif()

if (CMAKE_CXX_COMPILER_ID MATCHES Clang)
    set_source_files_properties(
        server/error/error_factory.cpp
        PROPERTIES COMPILE_FLAGS "-Wno-format-nonliteral"
    )
endif()

if (CMAKE_CXX_COMPILER_ID MATCHES Clang|GNU)
    set_source_files_properties(
        eventing/rest_client.cpp
        PROPERTIES COMPILE_FLAGS "-Wno-disabled-macro-expansion -Wno-old-style-cast"
    )
    set_source_files_properties(
        eventing/event_service.cpp
        model/handlers/handler_manager.cpp
        PROPERTIES COMPILE_FLAGS "-Wno-exit-time-destructors"
    )
    set_source_files_properties(
        endpoints/metadata_root.cpp
        endpoints/metadata.cpp
        PROPERTIES COMPILE_FLAGS "-Wno-exit-time-destructors -Wno-global-constructors"
    )
endif()

if(ENABLE_HTTPS)
    set(EXTRA_SOURCES ${EXTRA_SOURCES}
        server/certs/cert_loader.cpp
        server/certs/file_cert_loader.cpp
        server/certs/rmm_cert_loader.cpp
        server/certs/cert_manager.cpp
        server/connector/microhttpd/mhd_ssl_connector.cpp
    )

    add_definitions(-DENABLE_HTTPS)

    set_source_files_properties(
        server/cert_manager.cpp
        model/handlers/handler_manager.cpp
        model/handlers/root_handler.cpp
        PROPERTIES COMPILE_FLAGS "-Wno-exit-time-destructors"
    )
endif()

set_source_files_properties(
        utils/mapper.cpp
        PROPERTIES COMPILE_FLAGS "-Wno-exit-time-destructors -Wno-global-constructors"
)

add_library(application-rest ${SOURCES} ${EXTRA_SOURCES})

# Since app also uses generated metadata, it should wait for the headers
# to be generated
add_dependencies(application-rest metadata_generate_headers)

# Add directory for storing database files (during install)
install(DIRECTORY DESTINATION /var/opt/psme)
