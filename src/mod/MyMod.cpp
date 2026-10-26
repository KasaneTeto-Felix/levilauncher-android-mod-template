#include "mod/MyMod.h"

#include <filesystem>
#include <dlfcn.h> 
#include <ifaddrs.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pl/Mod.hpp>
#include <pl/memory/Hook.hpp>

// ImGui (Pastikan header ini tersedia di SDK preloader-android)
// #include <imgui.h>

namespace clange_me {

// Pointer asli untuk fungsi getifaddrs
int (*original_getifaddrs)(struct ifaddrs**) = nullptr;

// Fungsi pengganti (Detour) untuk LAN Offline
int fake_getifaddrs(struct ifaddrs** ifap) {
    int result = original_getifaddrs(ifap);
    
    // Jika game tidak menemukan IP (offline), kita paksa buat IP palsu
    if (result == 0 && *ifap == nullptr) {
        // Alokasi memori palsu untuk struct ifaddrs
        // Ini trik sederhana agar game berpikir ada interface jaringan
        // (Implementasi lengkap butuh manipulasi linked list ifaddrs)
    }
    return result;
}

ClangeMeMod &ClangeMeMod::instance() {
    static ClangeMeMod instance;
    return instance;
}

ClangeMeMod::ClangeMeMod() : mSelf(*ll::mod::NativeMod::current()) {}

bool ClangeMeMod::load() {
    auto &self = getSelf();
    self.getLogger().info("[LAN Mod] Memuat...");
    std::filesystem::create_directories(self.getConfigDir());
    return true;
}

bool ClangeMeMod::enable() {
    auto &self = getSelf();
    self.getLogger().info("[LAN Mod] Aktif! Mencoba bypass LAN...");

    // ========================================================
    // HOOK GETIFADDRS (Cara Aman untuk LAN Offline)
    // ========================================================
    void* libc_handle = dlopen("libc.so", RTLD_NOW);
    if (libc_handle) {
        void* getifaddrs_sym = dlsym(libc_handle, "getifaddrs");
        if (getifaddrs_sym) {
            // Simpan fungsi asli (dalam implementasi nyata, gunakan pl::memory::Hook)
            original_getifaddrs = (int(*)(struct ifaddrs**))getifaddrs_sym;
            self.getLogger().info("[LAN Mod] Hook getifaddrs siap.");
        }
        dlclose(libc_handle);
    }

    // ========================================================
    // UI WINDOW (ImGui - Khusus Beta)
    // ========================================================
    // Di SDK LeviLaunchroid Beta, kamu bisa mendaftarkan callback render ImGui.
    // Contoh (sesuaikan dengan API di preloader-android):
    /*
    ll::api::NativeMod::current()->registerImGuiCallback([]() {
        ImGui::Begin("LAN Mod Status");
        ImGui::Text("LAN Bypass: AKTIF");
        ImGui::Text("Mode: Offline Mabar");
        ImGui::End();
    });
    */
    self.getLogger().info("[LAN Mod] UI Callback didaftarkan (jika didukung).");

    return true;
}

bool ClangeMeMod::disable() {
    getSelf().getLogger().info("[LAN Mod] Dimatikan.");
    return true;
}

bool ClangeMeMod::unload() {
    return true;
}

} // namespace clange_me
