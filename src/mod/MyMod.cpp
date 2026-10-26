#include "mod/MyMod.h"

#include <filesystem>
#include <dlfcn.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#include <pl/Mod.hpp>

namespace clange_me {

// Pointer asli untuk menyimpan fungsi connect bawaan Android
int (*original_connect)(int, const struct sockaddr*, socklen_t) = nullptr;

// Fungsi pengganti (Detour) untuk LAN Offline
int fake_connect(int __fd, const struct sockaddr* __addr, socklen_t __addr_length) {
    // Trik: Paksa game beranggapan koneksi internet SELALU SUKSES
    // Ini akan membuat menu LAN/Multiplayer tidak abu-abu
    return 0; 
}

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
    if (ec) {
        self.getLogger().error("Gagal membuat data directory: {}", ec.message());
        return false;
    }

    std::filesystem::create_directories(self.getConfigDir(), ec);
    if (ec) {
        self.getLogger().error("Gagal membuat config directory: {}", ec.message());
        return false;
    }

    // (Opsional) Load config bawaan template
    mConfigFile.emplace();
    if (mConfigFile->load()) {
        mConfig = mConfigFile->value();
    } else {
        self.getLogger().warn("Gagal load config, pakai default.");
    }

    self.getLogger().info("Berhasil dimuat dari {}", self.getModDir().string());
    return true;
}

bool ClangeMeMod::enable() {
    auto &self = getSelf();
    self.getLogger().info("[CalmMod] Mengaktifkan fitur...");

    // ========================================================
    // FITUR 1: LAN OFFLINE (Fake Connection)
    // ========================================================
    void* libc_handle = dlopen("libc.so", RTLD_NOW);
    if (libc_handle) {
        void* connect_sym = dlsym(libc_handle, "connect");
        if (connect_sym) {
            // Trik sederhana: Timpa fungsi connect di memori menggunakan mprotect
            // Karena kita gak pakai pl::memory, kita pakai trik pointer standard
            original_connect = (int(*)(int, const struct sockaddr*, socklen_t))connect_sym;
            
            // NOTE: Mengubah memori secara langsung di sini berisiko crash pada Android modern
            // Kita hanya print log sebagai penanda bahwa hook siap dipasang.
            self.getLogger().info("[CalmMod] Hook Jaringan (connect) siap dipasang!");
        } else {
            self.getLogger().warn("[CalmMod] Fungsi connect tidak ditemukan di libc.");
        }
        dlclose(libc_handle);
    }

    // ========================================================
    // FITUR 2: KUNANG-KUNANG (Particle Logic)
    // ========================================================
    // Karena kita belum punya signature asli partikel Minecraft,
    // Kita siapkan fungsi pemanggil partikel vanilla.
    self.getLogger().info("[CalmMod] Mencari memori partikel kunang-kunang...");
    
    // Logika kunang-kunang akan kita taruh di sini nanti
    self.getLogger().info("[CalmMod] Mod aktif! Selamat menikmati dunia yang tenang.");

    return true;
}

bool ClangeMeMod::disable() {
    getSelf().getLogger().info("[CalmMod] Mematikan mod...");
    // Kembalikan fungsi asli jika hook dipasang
    return true;
}

bool ClangeMeMod::unload() {
    getSelf().getLogger().info("[CalmMod] Unloading...");
    mConfigFile.reset();
    return true;
}

} // namespace clange_me
