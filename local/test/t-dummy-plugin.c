// Copyright (c) Alpaca Core
// SPDX-License-Identifier: MIT
//
#include <ac/local.h>
#include <ac/dict.h>
#include <ac-unity.h>
#include "aclp-dummy-info.h"

ac_dict_arg make_params(ac_dict_ref temp, const char* json) {
    ac_dict_parse_json(temp, json, NULL);
    return ac_dict_arg_take(temp);
}

void local_dummy(void) {
    ac_load_plugin(ACLP_dummy_PLUGIN_FILE);

    ac_local_model* synthetic_model = ac_create_local_model(
        "dummy", NULL, 0, ac_dict_arg_null(), NULL, NULL);
    CHECK_NOT_NULL(synthetic_model);
    CHECK_NULL(ac_local_get_last_error());

    ac_local_instance* synthetic_instance = ac_create_local_instance(synthetic_model, "general", ac_dict_arg_null());
    CHECK_NOT_NULL(synthetic_instance);
    CHECK_NULL(ac_local_get_last_error());

    ac_dict_root* params_root = ac_dict_new_root();
    ac_dict_ref params = ac_dict_make_ref(params_root);
    ac_dict_root* result_root = ac_dict_new_root();
    ac_dict_ref result = ac_dict_make_ref(result_root);
    result = ac_run_local_op(ac_dict_make_ref(result_root), synthetic_instance, "run",
        make_params(params, "{\"input\": [\"x\", \"y\"]}"), NULL, NULL);
    CHECK_NOT_NULL(result);
    CHECK_NULL(ac_local_get_last_error());
    CHECK_EQ_STR("x one y two", ac_dict_get_string_value(ac_dict_at_key(result, "result")));


    ac_dict_free_root(params_root);
    ac_dict_free_root(result_root);
    ac_free_local_instance(synthetic_instance);
    ac_free_local_model(synthetic_model);
}

void setUp(void) {}
void tearDown(void) {}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(local_dummy);
    return UNITY_END();
}
