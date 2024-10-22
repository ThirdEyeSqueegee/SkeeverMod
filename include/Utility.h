#pragma once

#include <ankerl/unordered_dense.h>

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

        ankerl::unordered_dense::set<std::string_view> not_found{};
        for (const auto& [form_id, mod] : Settings::underwear_vec) {
            if (!handler->LookupModByName(mod)) {
                logger::error("ERROR: {} not found", mod);
                not_found.emplace(mod);
            }
            if (const auto undie{ handler->LookupForm<RE::TESObjectARMO>(form_id, mod) }; !not_found.contains(mod)) {
                underwear.emplace_back(undie);
                underwear_formids.emplace(undie->GetFormID());
                logger::info("Cached {} ({:#x}) from {}", undie->GetName(), undie->GetFormID(), mod);
            }
            else {
                logger::error("ERROR: {:#x} not found in {}", form_id, mod);
            }
        }
        if (underwear.empty()) {
            logger::error("ERROR: No underwear found in Underwear.ini. Please define at least one pair.");
            const auto plugin{ SKSE::PluginDeclaration::GetSingleton() };
            stl::report_and_fail(std::format("{}: Malformed configuration file (Underwear.ini). See Underwear.log.", plugin->GetName()));
        }

        for (const auto& [form_id, mod] : Settings::blacklist_vec) {
            if (const auto npc{ handler->LookupForm<RE::TESNPC>(form_id, mod) }; npc) {
                blacklist.emplace(npc);
                logger::info("Added {} ({:#x}) to blacklist", npc->GetName(), npc->GetFormID());
            }
            else {
                logger::error("ERROR: {:#x} not found in {}", form_id, mod);
            }
        }
    }

    static RE::TESObjectARMO* GetRandomUnderwear() noexcept
    {
        static std::random_device            rd;
        static std::mt19937                  rng(rd());
        static const auto                    size{ static_cast<int>(underwear.size() - 1) };
        static std::uniform_int_distribution dist(0, size);

        return underwear[dist(rng)];
    }

    inline static std::vector<RE::TESObjectARMO*> underwear{};

    inline static ankerl::unordered_dense::set<RE::FormID> underwear_formids{};

    inline static ankerl::unordered_dense::set<RE::TESNPC*> blacklist{};
};
