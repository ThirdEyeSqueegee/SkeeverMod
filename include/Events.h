#pragma once

namespace Events {
    class EquipEventHandler : public RE::BSTEventSink<RE::TESEquipEvent> {
    protected:
        EquipEventHandler() = default;

    public:
        EquipEventHandler(const EquipEventHandler&) = delete;
        EquipEventHandler(EquipEventHandler&&) = delete;
        EquipEventHandler& operator=(const EquipEventHandler&) = delete;
        EquipEventHandler& operator=(EquipEventHandler&&) = delete;

        static EquipEventHandler* GetSingleton();

        RE::BSEventNotifyControl ProcessEvent(const RE::TESEquipEvent* a_event, RE::BSTEventSource<RE::TESEquipEvent>* a_eventSource) override;

        static void Register();
    };

    class OnCellAttachEventHandler : public RE::BSTEventSink<RE::TESCellAttachDetachEvent> {
    protected:
        OnCellAttachEventHandler() = default;

    public:
        OnCellAttachEventHandler(const OnCellAttachEventHandler&) = delete;
        OnCellAttachEventHandler(OnCellAttachEventHandler&&) = delete;
        OnCellAttachEventHandler& operator=(const OnCellAttachEventHandler&) = delete;
        OnCellAttachEventHandler& operator=(OnCellAttachEventHandler&&) = delete;

        static OnCellAttachEventHandler* GetSingleton();

        RE::BSEventNotifyControl ProcessEvent(const RE::TESCellAttachDetachEvent* a_event, RE::BSTEventSource<RE::TESCellAttachDetachEvent>* a_eventSource) override;

        static void Register();
    };
}
