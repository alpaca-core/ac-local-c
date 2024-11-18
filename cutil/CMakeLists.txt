# Copyright (c) Alpaca Core
# SPDX-License-Identifier: MIT
#
add_library(ac-cutil INTERFACE)
add_library(ac-c::util ALIAS ac-cutil)
target_include_directories(ac-cutil INTERFACE include)
target_link_libraries(ac-cutil INTERFACE splat::splat)

if(AC_LOCAL_C_BUILD_TESTS)
    add_subdirectory(test)
endif()
