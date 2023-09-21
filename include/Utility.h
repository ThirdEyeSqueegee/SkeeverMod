#pragma once

#include "Settings.h"

constexpr auto body_slot{ RE::BGSBipedObjectForm::BipedObjectSlot::kBody };
constexpr auto head_slot{ RE::BGSBipedObjectForm::BipedObjectSlot::kHead };
constexpr auto hands_slot{ RE::BGSBipedObjectForm::BipedObjectSlot::kHands };
constexpr auto feet_slot{ RE::BGSBipedObjectForm::BipedObjectSlot::kFeet };
constexpr auto remove_reason{ RE::ITEM_REMOVE_REASON::kRemove };

class Utility : public Singleton<Utility>
{
public:
    static void InitUnderwear() noexcept
    {
        const auto handler{ RE::TESDataHandler::GetSingleton() };

        std::unordered_set<std::string_view> not_found;
        for (const auto& [form_id, mod] : Settings::underwear)
        {
            if (!handler->LookupModByName(mod))
            {
                logger::error("ERROR: {} not found", mod);
                not_found.emplace(mod);
            }
            if (const auto undie{ handler->LookupForm<RE::TESObjectARMO>(form_id, mod) }; !not_found.contains(mod))
            {
                underwear.emplace_back(undie);
                underwear_formids.emplace(undie->GetFormID());
                logger::info("Cached {} (0x{:x}) from {}", undie->GetName(), undie->GetFormID(), mod);
            }
            else
                logger::error("ERROR: {:x} not found in {}", form_id, mod);
        }
        if (underwear.empty())
        {
            logger::error("ERROR: No underwear found in Underwear.ini. Please define at least one pair.");
            const auto plugin{ SKSE::PluginDeclaration::GetSingleton() };
            stl::report_and_fail(fmt::format("{}: Malformed configuration file (Underwear.ini). See Underwear.log.", plugin->GetName()));
        }
    }

    inline static std::vector<RE::TESObjectARMO*> underwear{};
    inline static std::unordered_set<RE::FormID>  underwear_formids{};
};
