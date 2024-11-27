// Copyright (c) Alpaca Core
// SPDX-License-Identifier: MIT
//
#include "local.h"

#include <ac/local/Instance.hpp>
#include <ac/local/Model.hpp>
#include <ac/local/ModelAssetDesc.hpp>
#include <ac/local/Lib.hpp>
#include <ac/local/PluginManager.hpp>

#include <ac/DictCUtil.hpp>

#include <astl/move.hpp>

#include <string>

using namespace ac::cutil;
using namespace ac::local;

struct ac_local_model {
    ModelPtr model;
};

namespace {
thread_local std::string local_last_error;

template <typename F>
auto local_try_catch(F&& f) noexcept -> decltype(f()) {
    local_last_error.clear();
    try {
        return f();
    }
    catch (const std::exception& e) {
        local_last_error = e.what();
    }
    catch (...) {
        local_last_error = "Unknown error";
    }
    return {};
}

inline Instance* Instance_toCpp(ac_local_instance* p) {
    return reinterpret_cast<Instance*>(p);
}
inline ac_local_instance* Instance_fromCpp(Instance* p) {
    return reinterpret_cast<ac_local_instance*>(p);
}
} // namespace

extern "C" {

const char* ac_local_get_last_error() {
    if (local_last_error.empty()) return nullptr;
    return local_last_error.c_str();
}

void ac_free_local_instance(ac_local_instance* i) {
    delete Instance_toCpp(i);
}

ac_dict_ref ac_run_local_op(
    ac_dict_ref target,
    ac_local_instance* i,
    const char* op,
    ac_dict_arg cparams,
    bool (*progress_cb)(ac_sv tag, float progress, void* user_data),
    void* cb_user_data
) {
    return local_try_catch([&] {
        auto& instance = *Instance_toCpp(i);
        auto params = Dict_from_dict_arg(cparams);

        ProgressCb pcb;
        if (progress_cb) {
            pcb = [=](std::string_view tag, float progress) {
                return progress_cb(ac_sv::from_std(tag), progress, cb_user_data);
            };
        }

        Dict_from_dict_ref(target) = instance.runOp(op, astl::move(params), astl::move(pcb));
        return target;
    });
}

void ac_free_local_model(ac_local_model* m) {
    delete m;
}

ac_local_instance* ac_create_local_instance(
    ac_local_model* m,
    const char* instance_type,
    ac_dict_arg cparams
) {
    return local_try_catch([&] {
        auto instance = m->model->createInstance(instance_type, Dict_from_dict_arg(cparams));
        return Instance_fromCpp(instance.release());
    });
}

ac_local_model* ac_load_local_model(
    ac_local_model_asset_desc cdesc,
    ac_dict_arg cparams,
    bool (*progress_cb)(ac_sv tag, float progress, void* user_data),
    void* cb_user_data
) {
    return local_try_catch([&] {
        ModelAssetDesc desc = {.type = cdesc.type};
        desc.assets.reserve(cdesc.assets_count);
        for (size_t i = 0; i < cdesc.assets_count; ++i) {
            auto& info = desc.assets.emplace_back();
            assert(cdesc.assets[i].path);
            info.path = cdesc.assets[i].path;
            if (cdesc.assets[i].tag) {
                info.tag = cdesc.assets[i].tag;
            }
        }
        if (cdesc.name) {
            desc.name = cdesc.name;
        }

        ProgressCb pcb;
        if (progress_cb) {
            pcb = [=](std::string_view tag, float progress) {
                return progress_cb(ac_sv::from_std(tag), progress, cb_user_data);
            };
        }

        auto model = ac::local::Lib::loadModel(
            astl::move(desc), Dict_from_dict_arg(cparams), astl::move(pcb));
        return new ac_local_model{astl::move(model)};
    });
}

void ac_load_plugin(const char* path) {
    ac::local::Lib::loadPlugin(path);
}

void ac_add_plugin_dir(const char* path) {
    ac::local::Lib::addPluginDir(path);
}

void ac_add_plugin_dirs_from_env(const char* env_var) {
    ac::local::Lib::addPluginDirsFromEnvVar(env_var);
}

void ac_load_all_plugins(void) {
    ac::local::Lib::loadAllPlugins();
}

} // extern "C"
