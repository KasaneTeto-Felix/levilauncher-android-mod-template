#include "mod/MyMod.h"

#include <filesystem>
#include <pl/Mod.hpp>

namespace clange_me {

ClangeMeMod &ClangeMeMod::instance() {
    static ClangeMeMod instance;
    return instance;
}

ClangeMeMod::ClangeMeMod() : mSelf(*ll::mod::NativeMod::current()) {}

bool ClangeMeMod::load() {
    auto &self = getSelf();
    self.getLogger().info("[CalmMod] Memuat mod Kunang-kunang & LAN Offline...");

    std::error_code ec;
    std::filesystem::create_directories(self.getDataDir(), ec);
    std::filesystem::create_directories(self.getConfigDir(), ec);

    self.getLogger().info("[CalmMod] Berhasil dimuat tanpa crash!");
    return true;
}

bool ClangeMeMod::enable() {
    auto &self = getSelf();
    self.getLogger().info("[CalmMod] Mod aktif! Selamat menikmati dunia yang tenang.");
    
    // Nanti logika hook yang aman kita taruh di sini
    return true;
}

bool ClangeMeMod::disable() {
    getSelf().getLogger().info("[CalmMod] Mod dimatikan.");
    return true;
}

bool ClangeMeMod::unload() {
    getSelf().getLogger().info("[CalmMod] Unloading...");
    return true;
}

} // namespace clange_me
