#include "Events.h"

#include "Utility.h"

namespace Events {
    EquipEventHandler* EquipEventHandler::GetSingleton() {
        static EquipEventHandler singleton;
        return std::addressof(singleton);
    }

    RE::BSEventNotifyControl EquipEventHandler::ProcessEvent(const RE::TESEquipEvent* a_event, RE::BSTEventSource<RE::TESEquipEvent>* a_eventSource) {
        if (!a_event)
            return RE::BSEventNotifyControl::kContinue;

        if (a_event->equipped)
            return RE::BSEventNotifyControl::kContinue;

        if (const auto actor = a_event->actor->As<RE::Actor>()) {
            if (!actor->IsPlayerRef() && actor->Is3DLoaded()) {
                if (const auto item = RE::TESForm::LookupByID<RE::TESObjectARMO>(a_event->baseObject)) {
                    logger::debug("Item: {}", item->GetName());
                    if ("Underwear"sv.compare(item->GetName())) {
                        if (item->GetSlotMask() == RE::BGSBipedObjectForm::BipedObjectSlot::kBody) {
                            const auto inv = actor->GetInventory([](RE::TESBoundObject& obj) {return true;});
                            for (const auto& obj : inv | std::views::keys) {
                                if (const auto armo = obj->As<RE::TESObjectARMO>()) {
                                    logger::debug("{} still has slot 32 item {}, skipping underwear", actor->GetName(), item->GetName());
                                    if (armo == item) return RE::BSEventNotifyControl::kContinue;
                                }
                            }
                            logger::debug("Unequipped {} from {}", item->GetName(), actor->GetName());
                            std::random_device rd;
                            std::mt19937 rng(rd());
                            std::uniform_int_distribution dist(0, 3);
                            const auto idx = dist(rng);
                            const auto undie = Utility::underwear[idx];
                            actor->AddObjectToContainer(undie, nullptr, 1, nullptr);
                            logger::debug("Added {} to {}'s inventory", undie->GetName(), actor->GetName());
                            const auto manager = RE::ActorEquipManager::GetSingleton();
                            SKSE::GetTaskInterface()->AddTask([=] {
                                manager->EquipObject(actor, undie, nullptr, 1, nullptr, true, true, false, true);
                                actor->Update3DModel();
                                logger::debug("Equipped underwear to {}", actor->GetName());
                            });
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
