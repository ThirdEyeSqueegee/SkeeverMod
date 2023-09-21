#pragma once

namespace Hooks
{
    void Install() noexcept;

    class RemoveItem : public Singleton<RemoveItem>
    {
    public:
        // 2nd arg is a hack to place return value in rdx, thanks to Nukem, Noah, Parapets, and aers for explaining
        static RE::ObjectRefHandle* Thunk(RE::Character* a_this, RE::ObjectRefHandle* a_hidden_return_argument, RE::TESBoundObject* a_item, std::int32_t a_count,
                                          RE::ITEM_REMOVE_REASON a_reason, RE::ExtraDataList* a_extraList, RE::TESObjectREFR* a_moveToRef, const RE::NiPoint3* a_dropLoc,
                                          const RE::NiPoint3* a_rotate) noexcept;

        inline static REL::Relocation<decltype(&Thunk)> func;

        static constexpr std::size_t idx{ 86 }; // 0x56
    };

    class Load3D : public Singleton<Load3D>
    {
    public:
        static RE::NiAVObject* Thunk(RE::Character* a_this, bool a_arg1) noexcept;

        inline static REL::Relocation<decltype(&Thunk)> func;

        static constexpr std::size_t idx{ 106 }; // 0x6a
    };
} // namespace Hooks
