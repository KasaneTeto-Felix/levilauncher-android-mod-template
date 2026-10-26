#pragma once
#include <pl/Mod.hpp>
#include <string>

namespace modul_levi::log {
    inline void info(const std::string& msg) {
        ll::mod::NativeMod::current()->getLogger().info("[ModulLevi] {}", msg);
    }
    inline void warn(const std::string& msg) {
        ll::mod::NativeMod::current()->getLogger().warn("[ModulLevi] {}", msg);
    }
    inline void error(const std::string& msg) {
        ll::mod::NativeMod::current()->getLogger().error("[ModulLevi] {}", msg);
    }
}
