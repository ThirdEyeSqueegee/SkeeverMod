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

        if (const auto actor = a_event->actor->As<RE::Actor>(); !actor->IsPlayerRef() && actor->Is3DLoaded()) {
            if (const auto item = RE::TESForm::LookupByID<RE::TESObjectARMO>(a_event->baseObject);
                item->GetSlotMask() == RE::BGSBipedObjectForm::BipedObjectSlot::kBody) {
                logger::info("Stripped body slot for {}", actor->GetName());
                actor->AddObjectToContainer(Utility::red, nullptr, 1, nullptr);
                logger::info("Added underwear to {}'s inventory", actor->GetName());
                const auto manager = RE::ActorEquipManager::GetSingleton();
                manager->EquipObject(actor, Utility::red, nullptr, 1, nullptr,
                                     true, true, false, true);
                logger::info("Equipped underwear to {}", actor->GetName());
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
