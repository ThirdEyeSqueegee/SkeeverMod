#include "Events.h"

#include "Utility.h"

namespace Events {
    OnEquipEventHandler* OnEquipEventHandler::GetSingleton() {
        static OnEquipEventHandler singleton;
        return std::addressof(singleton);
    }

    RE::BSEventNotifyControl OnEquipEventHandler::ProcessEvent(const RE::TESEquipEvent* a_event,
                                                               RE::BSTEventSource<RE::TESEquipEvent>* a_eventSource) {
        if (!a_event)
            return RE::BSEventNotifyControl::kContinue;

        if (const auto actor = a_event->actor->As<RE::Actor>()) {
            if (actor->HasKeywordString("ActorTypeNPC"sv) && actor->Is3DLoaded() && !actor->IsPlayerRef()) {
                if (const auto item = RE::TESForm::LookupByID<RE::TESObjectARMO>(a_event->baseObject)) {
                    if (item->GetSlotMask() == RE::BGSBipedObjectForm::BipedObjectSlot::kBody) {
                        if (std::ranges::find(Utility::underwear_formids, item->GetFormID()) == Utility::underwear_formids.end()) {
                            if (!a_event->equipped) {
                                logger::debug("Unequipping slot 32 item {} from {}", item->GetName(), actor->GetName());
                                logger::debug("Spawning worker thread...");
                                std::jthread([=] {
                                    logger::debug("Worker thread sleeping for 5ms");
                                    std::this_thread::sleep_for(5ms);
                                    SKSE::GetTaskInterface()->AddTask([=] {
                                        for (const auto inv = actor->GetInventory();
                                             const auto& obj : inv | std::views::keys) { // Remove duplicate underwear
                                            if (const auto armo = obj->As<RE::TESObjectARMO>()) {
                                                if (std::ranges::find(Utility::underwear_formids, armo->GetFormID())
                                                    != Utility::underwear_formids.end()) {
                                                    logger::debug("Removing duplicate {} from {}'s inventory", armo->GetName(), actor->GetName());
                                                    actor->RemoveItem(armo, 1, RE::ITEM_REMOVE_REASON::kRemove, nullptr, nullptr);
                                                }
                                            }
                                        }
                                        std::random_device rd;
                                        std::mt19937 rng(rd());
                                        const auto size = static_cast<int>(Utility::underwear.size() - 1);
                                        std::uniform_int_distribution dist(0, size);
                                        const auto idx = dist(rng);
                                        const auto undie = Utility::underwear[idx];
                                        actor->AddObjectToContainer(undie, nullptr, 1, nullptr);
                                        logger::debug("Added {} to {}'s inventory", undie->GetName(), actor->GetName());
                                        const auto manager = RE::ActorEquipManager::GetSingleton();
                                        manager->EquipObject(actor, undie, nullptr, 1, nullptr, true, true, false, true);
                                        actor->Update3DModel();
                                        logger::debug("Equipped underwear to {}", actor->GetName());
                                    });
                                }).detach();
                            } else { // Item equipped to body slot, check if underwear is present and remove
                                logger::debug("Equipping slot 32 item {}", item->GetName());
                                for (const auto inv = actor->GetInventory(); const auto& obj : inv | std::views::keys) {
                                    if (const auto armo = obj->As<RE::TESObjectARMO>()) {
                                        if (std::ranges::find(Utility::underwear_formids, armo->GetFormID()) != Utility::underwear_formids.end()) {
                                            logger::debug("Removing {} from {} while equipping {}", armo->GetName(), actor->GetName(),
                                                          item->GetName());
                                            actor->RemoveItem(armo, 1, RE::ITEM_REMOVE_REASON::kRemove, nullptr, nullptr);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        return RE::BSEventNotifyControl::kContinue;
    }

    void OnEquipEventHandler::Register() {
        const auto holder = RE::ScriptEventSourceHolder::GetSingleton();
        holder->AddEventSink(GetSingleton());
        logger::info("Registered equip event handler");
    }
}
