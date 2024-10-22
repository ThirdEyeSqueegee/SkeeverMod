#include "Settings.h"

void Settings::LoadSettings() noexcept
{
    logger::info("Loading settings");

    CSimpleIniA ini;

    ini.SetUnicode();
    ini.SetMultiKey();
    ini.LoadFile(R"(.\Data\SKSE\Plugins\Underwear.ini)");

    debug_logging = ini.GetBoolValue("Log", "Debug");

    if (debug_logging) {
        spdlog::set_level(spdlog::level::debug);
        logger::debug("Debug logging enabled");
    }

    CSimpleIniA::TNamesDepend underwear_ini_values{};
    ini.GetAllValues("General", "Underwear", underwear_ini_values);

    for (const auto& val : underwear_ini_values) {
        const std::string val_str{ val.pItem };
        const auto        delim{ val_str.find('~') };
        const auto        form_id{ static_cast<RE::FormID>(std::stoul(val_str.substr(0, delim), nullptr, 16)) };
        const auto        plugin_name{ val_str.substr(delim + 1) };
        underwear_vec.emplace_back(form_id, plugin_name);
    }

    CSimpleIniA::TNamesDepend blacklist_ini_values{};
    ini.GetAllValues("Blacklist", "Ignore", blacklist_ini_values);

    for (const auto& val : blacklist_ini_values) {
        const std::string val_str{ val.pItem };
        const auto        delim{ val_str.find('~') };
        const auto        form_id{ static_cast<RE::FormID>(std::stoul(val_str.substr(0, delim), nullptr, 16)) };
        const auto        plugin_name{ val_str.substr(delim + 1) };
        blacklist_vec.emplace_back(form_id, plugin_name);
    }

    logger::info("Loaded settings");
    logger::info("");
}
