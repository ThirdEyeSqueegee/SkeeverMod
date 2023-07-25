#pragma once

class Utility {
  protected:
    Utility() = default;
    ~Utility() = default;

  public:
    Utility(const Utility&) = delete;             // Prevent copy construction
    Utility(Utility&&) = delete;                  // Prevent move construction
    Utility& operator=(const Utility&) = delete;  // Prevent copy assignment
    Utility& operator=(Utility&&) = delete;       // Prevent move assignment

    static Utility* GetSingleton() {
        static Utility singleton;
        return std::addressof(singleton);
    }

    inline static RE::TESObjectARMO* red = nullptr;
    inline static RE::TESObjectARMO* green = nullptr;
    inline static RE::TESObjectARMO* blue = nullptr;
    inline static RE::TESObjectARMO* yellow = nullptr;

    inline static std::vector<RE::TESObjectARMO*> underwear;

    static void InitUnderwear() {
        const auto handler = RE::TESDataHandler::GetSingleton();

        if (!handler->LookupModByName("zzzUnderwear.esp"sv)) 
            logger::info("zzzUnderwear.esp not found");

        red = handler->LookupForm<RE::TESObjectARMO>(0x813, "zzzUnderwear.esp"sv);
        green = handler->LookupForm<RE::TESObjectARMO>(0x812, "zzzUnderwear.esp"sv);
        blue = handler->LookupForm<RE::TESObjectARMO>(0x80b, "zzzUnderwear.esp"sv);
        yellow = handler->LookupForm<RE::TESObjectARMO>(0x801, "zzzUnderwear.esp"sv);

        underwear = {red, green, blue, yellow};

        logger::info("Cached underwear");
    }

    static void RemoveOldUnderwear() {
        const auto handler = RE::TESDataHandler::GetSingleton();
        handler->GetFormArray(RE::FormType::ActorCharacter);
    }
};
