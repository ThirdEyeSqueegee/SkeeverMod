#pragma once

class Settings : public Singleton<Settings> {
public:
    static void LoadSettings();

    inline static bool debug_logging{};

    inline static std::vector<std::pair<std::uint32_t, std::string_view>> underwear;
};
