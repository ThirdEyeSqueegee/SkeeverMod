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

        if (a_event->equipped)
            return RE::BSEventNotifyControl::kContinue;

        if (const auto actor = a_event->actor->As<RE::Actor>()) {
            if (actor->HasKeywordString("ActorTypeNPC"sv) && actor->Is3DLoaded() && !actor->IsPlayerRef()) {
                if (const auto item = RE::TESForm::LookupByID<RE::TESObjectARMO>(a_event->baseObject)) {
                    if (item->GetSlotMask() == RE::BGSBipedObjectForm::BipedObjectSlot::kBody) {
                        logger::debug("Unequipping slot 32 item {} from {}", item->GetName(), actor->GetName());
                        SKSE::GetTaskInterface()->AddTask([=] {
                            if (std::ranges::find(Utility::underwear, item) == Utility::underwear.end()) {
                                const auto inv = actor->GetInventory();
                                for (const auto& obj : inv | std::views::keys) {
                                    if (const auto armo = obj->As<RE::TESObjectARMO>()) {
                                        if (armo->GetSlotMask() == RE::BGSBipedObjectForm::BipedObjectSlot::kBody) {
                                            logger::debug("{} still has slot 32 item {}, skipping underwear", actor->GetName(), armo->GetName());
                                            return;
                                        }
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
                                manager->EquipObject(actor, undie, nullptr, 1, nullptr, true, true, false, true);
                                actor->Update3DModel();
                                actor->InitInventoryIfRequired();
                                logger::debug("Equipped underwear to {}", actor->GetName());
                            }
                        });
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

    OnContainerChangedEventHandler* OnContainerChangedEventHandler::GetSingleton() {
        static OnContainerChangedEventHandler singleton;
        return std::addressof(singleton);
    }

    RE::BSEventNotifyControl OnContainerChangedEventHandler::ProcessEvent(const RE::TESContainerChangedEvent* a_event,
                                                                          RE::BSTEventSource<RE::TESContainerChangedEvent>* a_eventSource) {
        if (!a_event)
            return RE::BSEventNotifyControl::kContinue;

        if (const auto actor = RE::TESForm::LookupByID<RE::Actor>(a_event->newContainer)) {
            const auto source = RE::TESForm::LookupByID(a_event->oldContainer);
            if (actor->HasKeywordString("ActorTypeNPC"sv) && actor->Is3DLoaded() && !actor->IsPlayerRef()) {
                if (const auto item = RE::TESForm::LookupByID<RE::TESObjectARMO>(a_event->baseObj)) {
                    logger::debug("OnContainerChanged: From {}, to {}, item {}", source ? source->GetName() : "nowhere", actor->GetName(),
                                  item->GetName());
                    if (item->GetSlotMask() == RE::BGSBipedObjectForm::BipedObjectSlot::kBody) {
                        if (std::ranges::find(Utility::underwear, item) == Utility::underwear.end()) {
                            logger::debug("Slot 32 item {} detected in OnContainerChanged", item->GetName());
                            actor->InitInventoryIfRequired();
                            logger::debug("Initialized {}'s inventory", actor->GetName());
                            const auto inv = actor->GetInventory();
                            for (const auto& obj : inv | std::views::keys) {
                                if (const auto undie = obj->As<RE::TESObjectARMO>()) {
                                    if (std::ranges::find(Utility::underwear, undie) != Utility::underwear.end()) {
                                        logger::debug("Underwear found in {}'s inventory during OnContainerChanged", actor->GetName());
                                        actor->RemoveItem(undie, 1, RE::ITEM_REMOVE_REASON::kRemove, nullptr, nullptr);
                                        logger::debug("Removed underwear from {} during OnContainerChanged", actor->GetName());
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

    void OnContainerChangedEventHandler::Register() {
        const auto holder = RE::ScriptEventSourceHolder::GetSingleton();
        holder->AddEventSink(GetSingleton());
        logger::info("Registered container changed event handler");
    }
}
