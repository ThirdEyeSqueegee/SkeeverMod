#include "Events.h"

#include "Utility.h"

namespace Events {
    EquipEventHandler* EquipEventHandler::GetSingleton() {
        static EquipEventHandler singleton;
        return std::addressof(singleton);
    }

    RE::BSEventNotifyControl EquipEventHandler::ProcessEvent(const RE::TESEquipEvent* a_event, RE::BSTEventSource<RE::TESEquipEvent>* a_eventSource) {
        if (!a_event) return RE::BSEventNotifyControl::kContinue;

        if (a_event->equipped) return RE::BSEventNotifyControl::kContinue;

        if (const auto actor = a_event->actor->As<RE::Actor>()) {
            if (!actor->IsPlayerRef() && actor->Is3DLoaded()) {
                if (const auto item = RE::TESForm::LookupByID<RE::TESObjectARMO>(a_event->baseObject)) {
                    logger::info("Item: {}, EDID: {}", item->GetName(), item->GetFormEditorID());
                    if (std::ranges::find(Utility::underwear, item) == Utility::underwear.end()) {
                        if (item->GetSlotMask() == RE::BGSBipedObjectForm::BipedObjectSlot::kBody) {
                            logger::info("Unequipped {} from {}", item->GetName(), actor->GetName());
                            std::random_device rd;
                            std::mt19937 rng(rd());
                            std::uniform_int_distribution dist(0, 3);
                            const auto idx = dist(rng);
                            const auto undie = Utility::underwear[idx];
                            actor->AddObjectToContainer(undie, nullptr, 1, nullptr);
                            logger::info("Added {} to {}'s inventory", undie->GetName(), actor->GetName());
                            if (const auto inv = actor->GetInventory([](RE::TESBoundObject& obj) { return obj.IsArmor(); }, true);
                                inv.contains(undie)) {
                                logger::info("{} has underwear in inventory", actor->GetName());
                                const auto manager = RE::ActorEquipManager::GetSingleton();
                                SKSE::GetTaskInterface()->AddTask([manager, actor, undie] {
                                    manager->EquipObject(actor, undie, nullptr, 1, nullptr, true, true, false, true);
                                    actor->Update3DModel();
                                });
                                logger::info("Equipped underwear to {}", actor->GetName());
                            }
                        }
                    }
                }
            }
        }

        return RE::BSEventNotifyControl::kContinue;
    }

    void EquipEventHandler::Register() {
        const auto holder = RE::ScriptEventSourceHolder::GetSingleton();
        holder->AddEventSink(GetSingleton());
        logger::info("Registered equip event handler");
    }

    OnCellAttachEventHandler* OnCellAttachEventHandler::GetSingleton() {
        static OnCellAttachEventHandler singleton;
        return std::addressof(singleton);
    }

    RE::BSEventNotifyControl OnCellAttachEventHandler::ProcessEvent(const RE::TESCellAttachDetachEvent* a_event,
                                                                    RE::BSTEventSource<RE::TESCellAttachDetachEvent>* a_eventSource) {
        if (!a_event) return RE::BSEventNotifyControl::kContinue;

        if (!a_event->attached) return RE::BSEventNotifyControl::kContinue;

        if (const auto actor = a_event->reference->As<RE::Actor>(); !actor->IsPlayerRef()) {
            logger::info("Actor {} loaded", actor->GetName());
            const auto underwear = actor->GetInventory([](RE::TESBoundObject& item) { return "Underwear"sv.compare(item.GetName()); }, true);
            if (underwear.size()) {
                logger::info("{} has underwear in inventory on load", actor->GetName());
                for (const auto& undie : underwear) {
                    actor->RemoveItem(undie.first, 1, RE::ITEM_REMOVE_REASON::kRemove, nullptr, nullptr);
                    logger::info("Removed underwear from {} on load", actor->GetName());
                }
            }
        }

        return RE::BSEventNotifyControl::kContinue;
    }

    void OnCellAttachEventHandler::Register() {
        const auto holder = RE::ScriptEventSourceHolder::GetSingleton();
        holder->AddEventSink(GetSingleton());
        logger::info("Registered cell attach event handler");
    }
}
