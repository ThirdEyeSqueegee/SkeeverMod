#include "Hooks.h"
#include "Logging.h"
#include "Settings.h"
#include "Utility.h"

void Listener(SKSE::MessagingInterface::Message* message) noexcept
{
    if (message->type == SKSE::MessagingInterface::kDataLoaded) {
        Settings::LoadSettings();
        Utility::InitUnderwear();
        Hooks::Install();
    }
}

SKSEPluginLoad(const SKSE::LoadInterface* skse)
{
    InitLogging();

    const auto plugin{ SKSE::PluginDeclaration::GetSingleton() };
    const auto name{ plugin->GetName() };
    const auto version{ plugin->GetVersion() };

    logger::info("{} {} is loading...", name, version);

    Init(skse);

    if (const auto messaging{ SKSE::GetMessagingInterface() }; !messaging->RegisterListener(Listener)) {
        return false;
    }

    logger::info("{} has finished loading.", name);
    logger::info("");

    return true;
}
