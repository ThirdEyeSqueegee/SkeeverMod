#pragma once

namespace Hooks {
    void Install();

    class RemoveItem : public Singleton<RemoveItem> {
    public:
        // 2nd arg is a hack to place return value in rdx, thanks to Nukem, Noah, Parapets, and aers for explaining
        static RE::ObjectRefHandle* Thunk(RE::Character* a_this, RE::ObjectRefHandle* a_hidden_return_argument, RE::TESBoundObject* a_item,
                                          std::int32_t a_count, RE::ITEM_REMOVE_REASON a_reason, RE::ExtraDataList* a_extraList,
                                          RE::TESObjectREFR* a_moveToRef, const RE::NiPoint3* a_dropLoc, const RE::NiPoint3* a_rotate);

        inline static REL::Relocation<decltype(&Thunk)> func;

        inline static constexpr std::size_t idx = (0xae0 - 0x830) / 0x8;
    };

    class Load3D : public Singleton<Load3D> {
    public:
        static RE::NiAVObject* Thunk(RE::Character* a_this, bool a_arg1);

        inline static REL::Relocation<decltype(&Thunk)> func;

        inline static constexpr std::size_t idx = (0xb80 - 0x830) / 0x8;
    };
}
