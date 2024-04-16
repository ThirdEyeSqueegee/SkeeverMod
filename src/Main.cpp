#include "Hooks.h"
#include "Logging.h"
#include "Settings.h"
#include "Utility.h"

void Listener(SKSE::MessagingInterface::Message* message) noexcept
{
    if (message->type <=> SKSE::MessagingInterface::kDataLoaded == 0) {
        Settings::LoadSettings();
        Utility::InitUnderwear();
        Hooks::Install();
    }
}

extern "C" __declspec(dllexport) constinit auto SKSEPlugin_Version = []() {
    SKSE::PluginVersionData v;
    v.PluginVersion(Version::MAJOR);
    v.PluginName(Version::PROJECT);
    v.AuthorName("ThirdEyeSqueegee");
    v.UsesAddressLibrary(true);
    v.UsesNoStructs(true);
    v.CompatibleVersions({ SKSE::RUNTIME_SSE_LATEST_AE });
    return v;
}();


extern "C" __declspec(dllexport) bool SKSEAPI SKSEPlugin_Query(const SKSE::QueryInterface*, SKSE::PluginInfo* aInfo)
{
    aInfo->infoVersion = SKSE::PluginInfo::kVersion;
    aInfo->name        = Version::PROJECT.data();
    aInfo->version     = Version::MAJOR;
    return true;
}

extern "C" [[maybe_unused]] __declspec(dllexport) bool SKSEPlugin_Load(const SKSE::LoadInterface* aSkse)
{
    InitializeLogging();

    const auto plugin{ SKSE::PluginDeclaration::GetSingleton() };
    const auto version{ plugin->GetVersion() };

    logger::info("{} {} is loading...", plugin->GetName(), version);

    SKSE::Init(aSkse);

    if (const auto messaging{ SKSE::GetMessagingInterface() }; !messaging->RegisterListener(Listener))
        return false;

    logger::info("{} has finished loading.", plugin->GetName());

    return true;
}
