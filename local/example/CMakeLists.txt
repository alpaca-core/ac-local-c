# Copyright (c) Alpaca Core
# SPDX-License-Identifier: MIT
#
macro(add_local_example name)
    add_executable(example-ac-c-local-${name} e-${name}.c)
    target_link_libraries(example-ac-c-local-${name} PRIVATE
        ac-c::local
        ac-c::jalogc
        ac-test-data::${name}
        aclp::out-dir
    )
    set_target_properties(example-ac-c-local-${name} PROPERTIES FOLDER example)
endmacro()

make_ac_local_plugin_available(
    NAME ilib-foo
    VERSION 1.0.0
    GITHUB alpaca-core/ilib-foo
)
CPMAddPackage(gh:alpaca-core/test-data-foo@1.0.0)
add_local_example(foo)

CPMAddPackage(
    NAME ac-test-data-llama
    VERSION 1.0.0
    GIT_REPOSITORY https://huggingface.co/alpaca-core/ac-test-data-llama
    GIT_TAG e6860e89d869ea815a4107bce3bb41af9ad1c1f0
)
make_ac_local_plugin_available(
    NAME ilib-llama.cpp
    VERSION 1.0.0
    GITHUB alpaca-core/ilib-llama.cpp
)
add_local_example(llama)
