#include "Settings.h"

void Settings::LoadSettings() {
    logger::info("Loading settings");

    CSimpleIniA ini;

    ini.SetUnicode();
    ini.SetMultiKey();
    ini.LoadFile(R"(.\Data\SKSE\Plugins\Underwear.ini)");

    debug_logging = ini.GetBoolValue("Log", "Debug");

    if (debug_logging) {
        spdlog::get("Global")->set_level(spdlog::level::level_enum::debug);
        logger::debug("Debug logging enabled");
    }

    CSimpleIniA::TNamesDepend underwear_ini_values;
    ini.GetAllValues("General", "Underwear", underwear_ini_values);

    for (const auto& val : underwear_ini_values) {
        const auto view{ std::string_view(val.pItem) };
        const auto delim{ view.find('~') };
        const auto form_id{ std::strtol(view.substr(0, delim).data(), nullptr, 0) };
        const auto mod_name{ view.substr(delim + 1) };
        underwear.emplace_back(form_id, mod_name);
    }

    logger::info("Loaded settings");
}
