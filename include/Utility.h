#pragma once

#include "Settings.h"

class Utility {
protected:
    Utility() = default;
    ~Utility() = default;

public:
    Utility(const Utility&) = delete;             // Prevent copy construction
    Utility(Utility&&) = delete;                  // Prevent move construction
    Utility& operator=(const Utility&) = delete;  // Prevent copy assignment
    Utility& operator=(Utility&&) = delete;       // Prevent move assignment

    static Utility* GetSingleton() {
        static Utility singleton;
        return std::addressof(singleton);
    }

    inline static std::vector<RE::TESObjectARMO*> underwear;

    static void InitUnderwear() {
        const auto handler = RE::TESDataHandler::GetSingleton();
        for (const auto& [k, v] : Settings::underwear) {
            if (!handler->LookupModByName(v))
                logger::error("ERROR: {} not found", v);
            const auto undie = handler->LookupForm<RE::TESObjectARMO>(k, v);
            underwear.emplace_back(undie);
            logger::info("Cached {} (0x{:x}) from {}", undie->GetName(), undie->GetFormID(), v);
        }
    }
};
