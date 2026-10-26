#pragma once
#include <pl/Mod.hpp>
#include <nmdk/NMDK.hpp>
#include <nmdk/EventBus.hpp>
#include <nmdk/events/ClientInstance.hpp>
#include <nmdk/events/ResourcePackManager.hpp>

class ModulLevi {
public:
    static ModulLevi& instance();
    ModulLevi();
    [[nodiscard]] ll::mod::NativeMod& getSelf() const { return mSelf; }

    bool load();
    bool enable();
    bool disable();
    bool unload();

private:
    ll::mod::NativeMod& mSelf;
    uint64_t mClientInitListenerId = 0;
    uint64_t mResourcePackListenerId = 0;

    void onClientInstanceInit(nmdk::ClientInstanceInitFinishedEvent& event);
    void onResourcePackCompose(nmdk::ResourcePackManagerBeforeComposeFullStackEvent& event);
};
