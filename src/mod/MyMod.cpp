#include <dlfcn.h>         // <--- WAJIB DI PALING ATAS SEBELUM APAPUN
#include "mod/MyMod.h"

#include <filesystem>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pl/Mod.hpp>

// Jaga-jaga kalau macro RTLD_NOW nggak kebaca dari dlfcn.h
#ifndef RTLD_NOW
#define RTLD_NOW 2
#endif

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
            // Nanti kita timpa fungsi di sini
        } else {
            self.getLogger().warn("[LAN Mod] Fungsi getifaddrs tidak ketemu.");
        }
        dlclose(libc_handle);
    } else {
        self.getLogger().error("[LAN Mod] Gagal membuka libc.so");
    }

    self.getLogger().info("[LAN Mod] Mod aktif! Silakan cek menu Multiplayer.");
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
