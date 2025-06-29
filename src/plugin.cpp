#include "Hooks.h"
#include "RE/Skyrim.h"
#include "SKSE/SKSE.h"
#include "logger.h"
#include "SKSE/API.h"


// Versioning information
extern "C" __declspec(dllexport) constinit SKSE::PluginVersionData SKSEPlugin_Version = [] {
    SKSE::PluginVersionData version{};

    version.PluginVersion({1, 0, 0});
    version.PluginName("dimt");
    version.AuthorName("cjw");
    version.UsesAddressLibrary();
    version.UsesUpdatedStructs();
    version.CompatibleVersions({SKSE::RUNTIME_SSE_LATEST});

    return version;
}();


// Menu Event Handler
class MenuEventHandler : public RE::BSTEventSink<RE::MenuOpenCloseEvent> {
public:
    RE::BSEventNotifyControl ProcessEvent(const RE::MenuOpenCloseEvent* a_event,
                                          RE::BSTEventSource<RE::MenuOpenCloseEvent>*) override {
        if (!a_event || !a_event->opening) return RE::BSEventNotifyControl::kContinue;

        if (a_event->menuName == RE::CraftingMenu::MENU_NAME) {
            logger::info("Crafting menu opened!");

            static bool hookInstalled = false;
            if (!hookInstalled) {
                logger::info("Installing hook after crafting menu opened...");
                InstallHooks();
                hookInstalled = true;
            } else {
                logger::info("Crafting menu opened again — hook already installed.");
            }
        }

        return RE::BSEventNotifyControl::kContinue;
    }
};

static MenuEventHandler g_menuEventHandler;

// SKSE Message Callback
void OnMessage(SKSE::MessagingInterface::Message* message) {
    switch (message->type) {
        case SKSE::MessagingInterface::kDataLoaded: {
            auto ui = RE::UI::GetSingleton();
            if (ui) {
                ui->AddEventSink<RE::MenuOpenCloseEvent>(&g_menuEventHandler);
                logger::info("Menu event sink registered 29 juni!");
            }
            
            break;
        }
        case SKSE::MessagingInterface::kNewGame:
        case SKSE::MessagingInterface::kPostLoadGame:
            // You can add post-load logic here
            break;
    }
};

// SKSE Plugin Entry Point
extern "C" __declspec(dllexport) bool SKSEPlugin_Load(const SKSE::LoadInterface* skse) {
    SetupLog();
    logger::info("Plugin loaded 29 juni!");

    SKSE::Init(skse);
    SKSE::AllocTrampoline(64);
    SKSE::GetMessagingInterface()->RegisterListener(OnMessage);

    MessageBoxA(nullptr, "DIMTPlugin loaded 29 June!", "SKSE", MB_OK);
    return true;
};



