#pragma once

#include "Settings.h"

class Utility {
protected:
    Utility() = default;
    ~Utility() = default;

public:
    Utility(const Utility&) = delete;            // Prevent copy construction
    Utility(Utility&&) = delete;                 // Prevent move construction
    Utility& operator=(const Utility&) = delete; // Prevent copy assignment
    Utility& operator=(Utility&&) = delete;      // Prevent move assignment

    static Utility* GetSingleton() {
        static Utility singleton;
        return std::addressof(singleton);
    }

    inline static std::vector<RE::TESObjectARMO*> underwear;
    inline static std::vector<RE::FormID> underwear_formids;

    static void InitUnderwear() {
        const auto handler = RE::TESDataHandler::GetSingleton();
        for (const auto& [form_id, mod] : Settings::underwear) {
            if (!handler->LookupModByName(mod))
                logger::error("ERROR: {} not found", mod);
            const auto undie = handler->LookupForm<RE::TESObjectARMO>(form_id, mod);
            underwear.emplace_back(undie);
            underwear_formids.emplace_back(undie->GetFormID());
            logger::info("Cached {} (0x{:x}) from {}", undie->GetName(), undie->GetFormID(), mod);
        }
    }
};
