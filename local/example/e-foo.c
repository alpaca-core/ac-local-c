// Copyright (c) Alpaca Core
// SPDX-License-Identifier: MIT
//
#include <ac/local.h>
#include <ac/dict.h>

#include <jalogc.h>

#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

#include "ac-test-data-foo-models.h"
#include "aclp-out-dir.h"

int main(void) {
    int ret = 0;
    jalogc_init((jalogc_init_params) {.add_default_sink = true});

    ac_add_plugin_dir(ACLP_OUT_DIR);
    ac_load_all_plugins();

    ac_local_model_desc_asset model_asset = {
        .path = AC_FOO_MODEL_SMALL,
        .tag = "x"
    };

    printf("Loading model...\n");
    ac_local_instance* instance = NULL;
    ac_local_model* model = ac_create_local_model(
        "foo", &model_asset, 1,
        ac_dict_arg_null(), NULL, NULL
    );
    if (!model) {
        ret = 1;
        goto cleanup;
    }

    printf("Creating instance...\n");
    instance = ac_create_local_instance(model, "general", ac_dict_arg_null());
    if (!instance) {
        ret = 1;
        goto cleanup;
    }

    printf("Generation:\n");
    ac_dict_root* params_root = ac_dict_new_root_from_json("{\"input\": [\"Xuxa\", \"sang:\"], \"splice\": false}", NULL);
    if (!params_root) {
        ret = 1;
        goto cleanup;
    }

    ac_dict_root* result_root = ac_dict_new_root();
    ac_dict_ref result = ac_run_local_op(ac_dict_make_ref(result_root), instance, "run",
        ac_dict_arg_take(ac_dict_make_ref(params_root)),
        NULL, NULL);

    ac_dict_free_root(params_root);
    if (!result) {
        ac_dict_free_root(result_root);
        ret = 1;
        goto cleanup;
    }

    char* dump = ac_dict_dump(result , 2);
    printf("%s\n", dump);
    free(dump);
    ac_dict_free_root(result_root);

cleanup:
    if (ret != 0) {
        fprintf(stderr, "Error: %s\n", ac_local_get_last_error());
    }
    ac_free_local_instance(instance);
    ac_free_local_model(model);
    return ret;
}
