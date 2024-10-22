#pragma once

class Settings : public Singleton<Settings>
{
public:
    static void LoadSettings() noexcept;

    inline static bool debug_logging{};

    inline static std::vector<std::pair<RE::FormID, std::string>> underwear_vec{};

    inline static std::vector<std::pair<RE::FormID, std::string>> blacklist_vec{};
};
