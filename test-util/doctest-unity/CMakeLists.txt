# Copyright (c) Alpaca Core
# SPDX-License-Identifier: MIT
#
CPMAddPackage(gh:iboB/doctest-lib@2.4.11)

add_library(doctest-unity STATIC
    doctest-unity-main.cpp
)
add_library(ac-c-dev::doctest-unity ALIAS doctest-unity)
set_target_properties(doctest-unity PROPERTIES FOLDER test)
target_link_libraries(doctest-unity PUBLIC
    unity::framework
    doctest::headers
)

function(add_doctest_unity_lib_test test lib)
    cmake_parse_arguments(ARG "" "" "SOURCES;LIBRARIES;LABELS" ${ARGN})
    icm_add_test(
        NAME ${lib}-${test}
        TARGET test-${lib}-${test}
        LIBRARIES
            ac-c-dev::doctest-unity
            ac-c-dev::test-util
            ${lib}
            ${ARG_LIBRARIES}
        SOURCES
            ${ARG_UNPARSED_ARGUMENTS}
            ${ARG_SOURCES}
        LABELS ${ARG_LABELS}
        FOLDER test
    )
endfunction()
