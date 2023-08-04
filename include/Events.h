#pragma once

namespace Events {
    class OnEquipEventHandler : public RE::BSTEventSink<RE::TESEquipEvent> {
    protected:
        OnEquipEventHandler() = default;

    public:
        OnEquipEventHandler(const OnEquipEventHandler&) = delete;
        OnEquipEventHandler(OnEquipEventHandler&&) = delete;
        OnEquipEventHandler& operator=(const OnEquipEventHandler&) = delete;
        OnEquipEventHandler& operator=(OnEquipEventHandler&&) = delete;

        static OnEquipEventHandler* GetSingleton();

        RE::BSEventNotifyControl ProcessEvent(const RE::TESEquipEvent* a_event, RE::BSTEventSource<RE::TESEquipEvent>* a_eventSource) override;

        static void Register();
    };

    class OnContainerChangedEventHandler : public RE::BSTEventSink<RE::TESContainerChangedEvent> {
    protected:
        OnContainerChangedEventHandler() = default;

    public:
        OnContainerChangedEventHandler(const OnContainerChangedEventHandler&) = delete;
        OnContainerChangedEventHandler(OnContainerChangedEventHandler&&) = delete;
        OnContainerChangedEventHandler& operator=(const OnContainerChangedEventHandler&) = delete;
        OnContainerChangedEventHandler& operator=(OnContainerChangedEventHandler&&) = delete;

        static OnContainerChangedEventHandler* GetSingleton();

        RE::BSEventNotifyControl ProcessEvent(const RE::TESContainerChangedEvent* a_event,
                                              RE::BSTEventSource<RE::TESContainerChangedEvent>* a_eventSource) override;

        static void Register();
    };
}
