# Copyright (c) Alpaca Core
# SPDX-License-Identifier: MIT
#
add_library(ac-c-test-data-dummy INTERFACE)
target_compile_definitions(ac-c-test-data-dummy INTERFACE DUMMY_MODEL="${CMAKE_CURRENT_SOURCE_DIR}/dummy-small.txt")

add_unity_lib_test(dummy ac-c-local t-dummy.c
    LIBRARIES
        ac-c-test-data-dummy
        aclp::dummy-plib
)

add_unity_lib_test(dummy-plugin ac-c-local t-dummy-plugin.c
    LIBRARIES
        aclp::dummy-info
)