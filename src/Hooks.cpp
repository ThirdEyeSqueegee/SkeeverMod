#include "Hooks.h"

#include "Utility.h"

namespace Hooks {
    void Install() {
        stl::write_vfunc<RE::Character, RemoveItem>();
        logger::info("Installed Character::RemoveItem hook");

        stl::write_vfunc<RE::Character, Load3D>();
        logger::info("Installed Character::Load3D hook");
    }

    RE::ObjectRefHandle* RemoveItem::Thunk(RE::Character* a_this, RE::ObjectRefHandle* a_hidden_return_argument, RE::TESBoundObject* a_item,
                                           std::int32_t a_count, RE::ITEM_REMOVE_REASON a_reason, RE::ExtraDataList* a_extraList,
                                           RE::TESObjectREFR* a_moveToRef, const RE::NiPoint3* a_dropLoc, const RE::NiPoint3* a_rotate) {
        const auto worn   = a_this->GetWornArmor(body_slot, true);
        const auto result = func(a_this, a_hidden_return_argument, a_item, a_count, a_reason, a_extraList, a_moveToRef, a_dropLoc, a_rotate);

        if (worn && !a_this->GetWornArmor(body_slot)) { // Previously worn body slot item found, body slot is currently empty
            if (!a_this->IsPlayerRef() && !a_this->IsPlayerTeammate() && a_this->HasKeywordString("ActorTypeNPC"sv) && a_moveToRef) {
                if (a_moveToRef->IsPlayerRef()) {
                    if (const auto item = a_item->As<RE::TESObjectARMO>()) {
                        if (item->GetSlotMask() == body_slot && item->GetFormID() == worn->GetFormID()) {
                            if (!Utility::underwear_formids.contains(item->GetFormID())) {
                                const auto inv     = a_this->GetInventory([](RE::TESBoundObject& obj) { return true; }, true);
                                const auto manager = RE::ActorEquipManager::GetSingleton();
                                for (const auto& obj : inv | std::views::keys) {
                                    if (Utility::underwear_formids.contains(obj->GetFormID())) {
                                        manager->EquipObject(a_this, obj, nullptr, 1, nullptr, true, false, false, false);
                                        return result;
                                    }
                                }
                                std::random_device            rd;
                                std::mt19937                  rng(rd());
                                const auto                    size = static_cast<int>(Utility::underwear.size() - 1);
                                std::uniform_int_distribution dist(0, size);
                                const auto                    undie = Utility::underwear[dist(rng)];
                                a_this->AddObjectToContainer(undie, nullptr, 1, nullptr);
                                manager->EquipObject(a_this, undie, nullptr, 1, undie->GetEquipSlot(), true, false, false, false);
                                logger::debug("Equipped {} to {}", undie->GetName(), a_this->GetName());
                            }
                        }
                    }
                }
            }
        }

        return result;
    }

    RE::NiAVObject* Load3D::Thunk(RE::Character* a_this, bool a_arg1) {
        const auto result = func(a_this, a_arg1);

        std::jthread([=] {
            std::this_thread::sleep_for(1s);
            SKSE::GetTaskInterface()->AddTask([=] {
                if (!a_this->IsPlayerRef() && a_this->HasKeywordString("ActorTypeNPC"sv) && !a_this->IsPlayerTeammate()) {
                    const auto body_worn  = a_this->GetWornArmor(body_slot);
                    const auto head_worn  = a_this->GetWornArmor(head_slot);
                    const auto hands_worn = a_this->GetWornArmor(hands_slot);
                    const auto feet_worn  = a_this->GetWornArmor(feet_slot);
                    if (!body_worn || !head_worn || !hands_worn || !feet_worn) {
                        const auto manager    = RE::ActorEquipManager::GetSingleton();
                        const auto inv        = a_this->GetInventory();
                        auto       body_found = false, head_found = false, hands_found = false, feet_found = false;
                        for (const auto& item : inv | std::views::keys) {
                            if (const auto armo = item->As<RE::TESObjectARMO>()) {
                                if (armo->GetSlotMask() == hands_slot && !hands_worn && !hands_found) {
                                    hands_found = true;
                                    manager->EquipObject(a_this, item, nullptr, 1, armo->GetEquipSlot(), true, false, false, false);
                                    logger::debug("{} spawned naked, equipped hands slot item {} found in inventory", a_this->GetName(),
                                                  item->GetName());
                                }
                                if (armo->GetSlotMask() == feet_slot && !feet_worn && !feet_found) {
                                    feet_found = true;
                                    manager->EquipObject(a_this, item, nullptr, 1, armo->GetEquipSlot(), true, false, false, false);
                                    logger::debug("{} spawned naked, equipped feet slot item {} found in inventory", a_this->GetName(),
                                                  item->GetName());
                                }
                                if (armo->GetSlotMask() == head_slot && !head_worn && !head_found) {
                                    head_found = true;
                                    manager->EquipObject(a_this, item, nullptr, 1, armo->GetEquipSlot(), true, false, false, false);
                                    logger::debug("{} spawned naked, equipped head slot item {} found in inventory", a_this->GetName(),
                                                  item->GetName());
                                }
                                if (armo->GetSlotMask() == body_slot && !body_worn && !body_found) {
                                    body_found = true;
                                    manager->EquipObject(a_this, item, nullptr, 1, armo->GetEquipSlot(), true, false, false, false);
                                    logger::debug("{} spawned naked, equipped body slot item {} found in inventory", a_this->GetName(),
                                                  item->GetName());
                                }
                            }
                        }
                        if (!body_worn && !body_found) {
                            std::random_device            rd;
                            std::mt19937                  rng(rd());
                            const auto                    size = static_cast<int>(Utility::underwear.size() - 1);
                            std::uniform_int_distribution dist(0, size);
                            const auto                    undie = Utility::underwear[dist(rng)];
                            a_this->AddObjectToContainer(undie, nullptr, 1, nullptr);
                            manager->EquipObject(a_this, undie, nullptr, 1, undie->GetEquipSlot(), true, false, false, false);
                            logger::debug("{} spawned naked, equipped {}", a_this->GetName(), undie->GetName());
                        }
                    }
                }
            });
        }).detach();

        return result;
    }
}
