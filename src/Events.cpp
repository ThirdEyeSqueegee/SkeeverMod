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
                    if (item->GetSlotMask() == RE::BGSBipedObjectForm::BipedObjectSlot::kBody && "Underwear"sv.compare(item->GetName())) {
                        std::random_device rd;
                        std::mt19937 rng(rd());
                        std::uniform_int_distribution dist(0, 3);
                        const auto idx = dist(rng);
                        const auto undie = Utility::underwear[idx];
                        actor->AddObjectToContainer(undie, nullptr, 1, nullptr);
                        const auto inv = actor->GetInventory([](RE::TESBoundObject& obj) { return obj.IsArmor(); });
                        if (inv.contains(undie)) {
                            const auto manager = RE::ActorEquipManager::GetSingleton();
                            manager->EquipObject(actor, undie, nullptr, 1, nullptr,
                                                 true, true, false, true);
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
}
