// Copyright (c) Alpaca Core
// SPDX-License-Identifier: MIT
//
#pragma once
#include "export.h"

#include <ac/dict_ref.h>
#include <ac/dict_arg.h>
#include <ac/sv.h>

#if defined(__cplusplus)
extern "C" {
#endif

/// @defgroup c-local Local inference API
/// C API for local inference.

/// @addtogroup c-local
/// @{

/// Get the last local inference error message or `NULL` if no error.
/// The function returns a thread-local string. Ownership of the string is not transferred.
/// @note Every `local_` function invalidates the last error.
AC_C_LOCAL_EXPORT const char* ac_local_get_last_error(void);

/// Opaque structure representing an instance.
typedef struct ac_local_instance ac_local_instance;

/// Free a local instance.
/// `NULL` is a no-op.
AC_C_LOCAL_EXPORT void ac_free_local_instance(ac_local_instance* i);

/// Run an op on a local instance
/// `target` is where the result of the operation is stored.
/// @return `target` or `NULL` on error.
AC_C_LOCAL_EXPORT ac_dict_ref ac_run_local_op(
    ac_dict_ref target,
    ac_local_instance* i,
    const char* op,
    ac_dict_arg params,
    bool (*progress_cb)(ac_sv tag, float progress, void* user_data),
    void* cb_user_data
);

/// Opaque structure representing a model.
typedef struct ac_local_model ac_local_model;

/// Free a local model.
/// `NULL` is a no-op.
AC_C_LOCAL_EXPORT void ac_free_local_model(ac_local_model* m);

/// Create a local instance of a model.
/// Returns `NULL` on error.
AC_C_LOCAL_EXPORT ac_local_instance* ac_create_local_instance(
    ac_local_model* m,
    const char* instance_type,
    ac_dict_arg params
);

/// Asset info for creating a local model.
typedef struct ac_local_model_asset_info {
    const char* path; ///< Path to the asset.
    const char* tag;  ///< Tag of the asset (may be `NULL`).
} ac_local_model_asset_info;

/// Asset description for creating a local model.
typedef struct ac_local_model_asset_desc {
    const char* type; ///< Type of the asset.
    ac_local_model_asset_info* assets; ///< Array of asset infos.
    size_t assets_count; ///< Number of assets.
    const char* name; ///< Optional name of the model (only used for logging)
} ac_local_model_asset_desc;

/// Create a local model.
/// Returns `NULL` on error.
AC_C_LOCAL_EXPORT ac_local_model* ac_load_local_model(
    ac_local_model_asset_desc desc,
    ac_dict_arg params,
    bool (*progress_cb)(ac_sv tag, float progress, void* user_data),
    void* cb_user_data
);

AC_C_LOCAL_EXPORT void ac_load_plugin(const char* path);
AC_C_LOCAL_EXPORT void ac_add_plugin_dir(const char* path);
AC_C_LOCAL_EXPORT void ac_add_plugin_dirs_from_env(const char* env_var);
AC_C_LOCAL_EXPORT void ac_load_all_plugins(void);

/// @}

#if defined(__cplusplus)
} // extern "C"
#endif
