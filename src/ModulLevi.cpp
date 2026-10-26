#include "ModulLevi.hpp"
#include "NexLog.hpp"
#include <filesystem>

ModulLevi& ModulLevi::instance() {
    static ModulLevi instance;
    return instance;
}

ModulLevi::ModulLevi() : mSelf(*ll::mod::NativeMod::current()) {}

bool ModulLevi::load() {
    auto& self = getSelf();
    log::info("Memuat fondasi Modul Levi...");
    std::error_code ec;
    std::filesystem::create_directories(self.getConfigDir(), ec);
    if (ec) {
        log::error("Gagal membuat direktori config: " + ec.message());
        return false;
    }
    return true;
}

bool ModulLevi::enable() {
    auto& bus = nmdk::getEventBus();
    
    // Daftarkan listener ClientInstance
    mClientInitListenerId = bus.subscribe<nmdk::ClientInstanceInitFinishedEvent>(
        [this](nmdk::ClientInstanceInitFinishedEvent& event) {
            this->onClientInstanceInit(event);
        }, 0);

    // Daftarkan listener ResourcePackManager
    mResourcePackListenerId = bus.subscribe<nmdk::ResourcePackManagerBeforeComposeFullStackEvent>(
        [this](nmdk::ResourcePackManagerBeforeComposeFullStackEvent& event) {
            this->onResourcePackCompose(event);
        }, 0);

    if (mClientInitListenerId == 0 || mResourcePackListenerId == 0) {
        log::error("Gagal mendaftarkan listener event NMDK.");
        return false;
    }
    log::info("Modul Levi aktif. Listener NMDK terdaftar.");
    return true;
}

bool ModulLevi::disable() {
    auto& bus = nmdk::getEventBus();
    if (mClientInitListenerId != 0) {
        bus.unsubscribe(mClientInitListenerId);
        mClientInitListenerId = 0;
    }
    if (mResourcePackListenerId != 0) {
        bus.unsubscribe(mResourcePackListenerId);
        mResourcePackListenerId = 0;
    }
    log::info("Modul Levi dinonaktifkan. Listener dilepas.");
    return true;
}

bool ModulLevi::unload() {
    log::info("Modul Levi di-unload.");
    return true;
}

void ModulLevi::onClientInstanceInit(nmdk::ClientInstanceInitFinishedEvent& event) {
    if (!event.mClient) {
        log::warn("ClientInstanceInitFinishedEvent: mClient null.");
        return;
    }
    log::info("ClientInstance berhasil diinisialisasi.");
    // Tempatkan logika yang membutuhkan ClientInstance di sini
}

void ModulLevi::onResourcePackCompose(nmdk::ResourcePackManagerBeforeComposeFullStackEvent& event) {
    log::info("ResourcePackManagerBeforeComposeFullStackEvent dipicu.");
    // Tempatkan logika resource pack di sini
}

PL_REGISTER_MOD(ModulLevi, ModulLevi::instance())
