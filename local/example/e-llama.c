// Copyright (c) Alpaca Core
// SPDX-License-Identifier: MIT
//
#include <ac/local.h>
#include <ac/dict.h>

#include <jalogc.h>

#include <ac/unused.h>

#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

#include "ac-test-data-llama-dir.h"
#include "aclp-out-dir.h"

bool on_progress(ac_sv tag, float progress, void* user_data) {
    UNUSED(user_data);
    printf(PRacsv " progress: %f\n", AC_PRINTF_SV(tag), progress);
    return true;
}

int main(void) {
    int ret = 0;
    jalogc_init((jalogc_init_params) {.add_default_sink = true});

    ac_add_plugin_dir(ACLP_OUT_DIR);
    ac_load_all_plugins();

    ac_local_instance* instance = NULL;

    ac_local_model_desc_asset llama_gguf = {
        .path = AC_TEST_DATA_LLAMA_DIR "/gpt2-117m-q6_k.gguf",
    };

    printf("Loading model...\n");
    ac_local_model* model = ac_create_local_model(
        "llama", &llama_gguf, 1,
        ac_dict_arg_null(), on_progress, NULL
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

#define PROMPT "He was slow to"
    printf("Running op. Prompt: %s\n", PROMPT);
    printf("Generation: ");

    ac_dict_root* params_root = ac_dict_new_root_from_json("{\"prompt\": \"" PROMPT "\", \"max_tokens\": 30}", NULL);
    if (!params_root) {
        ret = 1;
        goto cleanup;
    }

    ac_dict_root* result_root = ac_dict_new_root();
    ac_dict_ref result = ac_run_local_op(ac_dict_make_ref(result_root), instance, "run",
        ac_dict_arg_take(ac_dict_make_ref(params_root)),
        NULL, NULL
    );

    ac_dict_free_root(params_root);
    if (!result) {
        ac_dict_free_root(result_root);
        ret = 1;
        goto cleanup;
    }

    ac_dict_ref result_val = ac_dict_at_key(result, "result");
    if (result_val) {
        printf("%s\n", ac_dict_get_string_value(result_val));
    }
    else {
        char* dump = ac_dict_dump(result, 2);
        fprintf(stderr, "Error bad dict:\n%s\n", dump);
        free(dump);
        ret = 1;
    }
    ac_dict_free_root(result_root);

cleanup:
    if (ret != 0) {
        fprintf(stderr, "Error: %s\n", ac_local_get_last_error());
    }
    ac_free_local_instance(instance);
    ac_free_local_model(model);
    return ret;
}
