# Copyright (c) Alpaca Core
# SPDX-License-Identifier: MIT
#
add_subdirectory(code)
if(AC_LOCAL_C_BUILD_TESTS)
    add_subdirectory(test)
endif()
if(AC_LOCAL_C_BUILD_EXAMPLES)
    add_subdirectory(example)
endif()

