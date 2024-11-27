# Copyright (c) Alpaca Core
# SPDX-License-Identifier: MIT
#
cmake_minimum_required(VERSION 3.24 FATAL_ERROR)

project(ac-local-c
    VERSION 1.0.0
    DESCRIPTION "C wrapper of the Alpaca Core Local SDK"
    LANGUAGES C CXX
)

include(./get-ac-build.cmake)
include(init_ac_prj)

#################
# cmake lib
CPMAddPackage(gh:iboB/icm@1.5.4)
list(APPEND CMAKE_MODULE_PATH
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake"
    "${icm_SOURCE_DIR}"
)

include(ac_build_prj_util)
include(icm_add_lib)
include(icm_testing)

#################
# options

option(AC_LOCAL_C_BUILD_TESTS "${PROJECT_NAME}: build tests" ${testsDefault})
option(AC_LOCAL_C_BUILD_EXAMPLES "${PROJECT_NAME}: build examples" ${examplesDefault})
mark_as_advanced(AC_LOCAL_C_BUILD_TESTS AC_LOCAL_C_BUILD_EXAMPLES)

#######################################
# packages

add_ac_local(0.1.3)
CPMAddPackage(gh:iboB/splat@1.3.3)

#######################################
# subdirs

if(AC_LOCAL_C_BUILD_TESTS)
    CPMAddPackage(gh:ThrowTheSwitch/Unity@2.6.0)
    set_target_properties(unity PROPERTIES FOLDER test)
    add_subdirectory(test-util)
    enable_testing()
endif()

add_subdirectory(cutil)
add_subdirectory(jalogc)

add_subdirectory(dict)

add_subdirectory(local)
