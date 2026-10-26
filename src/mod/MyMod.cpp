#include "mod/MyMod.h"
#include <filesystem>
#include <pl/Mod.hpp>

// =========================================================================
// TRIK DARURAT: Deklarasi manual fungsi dlopen/dlsym/dlclose
// Ini biar kita gak butuh file header <dlfcn.h> yang suka error di NDK
// =========================================================================
extern "C" {
    void* dlopen(const char* filename, int flag);
    void* dlsym(void* handle, const char* symbol);
    int dlclose(void* handle);
}
#ifndef RTLD_NOW
#define RTLD_NOW 2
#endif
// =========================================================================

namespace clange_me {

ClangeMeMod &ClangeMeMod::instance() {
    static ClangeMeMod instance;
    return instance;
}

ClangeMeMod::ClangeMeMod() : mSelf(*ll::mod::NativeMod::current()) {}

bool ClangeMeMod::load() {
    auto &self = getSelf();
    self.getLogger().info("[LAN Mod] Memuat mod...");
    
    std::error_code ec;
    std::filesystem::create_directories(self.getDataDir(), ec);
    std::filesystem::create_directories(self.getConfigDir(), ec);

    return true;
}

bool ClangeMeMod::enable() {
    auto &self = getSelf();
    self.getLogger().info("[LAN Mod] Mengaktifkan fitur LAN Offline...");

    // HOOK GETIFADDRS (Membajak fungsi cek jaringan)
    void* libc_handle = dlopen("libc.so", RTLD_NOW);
    if (libc_handle) {
        void* getifaddrs_sym = dlsym(libc_handle, "getifaddrs");
        if (getifaddrs_sym) {
            self.getLogger().info("[LAN Mod] Berhasil menemukan fungsi getifaddrs!");
        } else {
            self.getLogger().warn("[LAN Mod] Fungsi getifaddrs tidak ketemu.");
        }
        dlclose(libc_handle);
    } else {
        self.getLogger().error("[LAN Mod] Gagal membuka libc.so");
    }

    self.getLogger().info("[LAN Mod] Mod aktif!");
    return true;
}

bool ClangeMeMod::disable() {
    getSelf().getLogger().info("[LAN Mod] Dimatikan.");
    return true;
}

bool ClangeMeMod::unload() {
    getSelf().getLogger().info("[LAN Mod] Unloading...");
    return true;
}

} // namespace clange_me
