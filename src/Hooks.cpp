#include "Hooks.h"

// Includes for logging, Skyrim runtime access, SKSE helpers, and trampoline patching
#include <spdlog/spdlog.h>

#include "RE/Skyrim.h"
#include "REL/Relocation.h"
#include "SKSE/API.h"
#include "SKSE/SKSE.h"
#include "logger.h"

namespace {
    // The address of the function we're hooking
    constexpr REL::ID kTargetFuncID(179050);  // Make sure 0x179050 is correct ID
    REL::Relocation<std::uintptr_t> targetFunc(kTargetFuncID);

    using FnOriginal = std::uint32_t*(__fastcall*)(std::uint32_t*, std::int64_t);
    FnOriginal originalFunc = nullptr;

    // Dummy detour that logs and calls original
    std::uint32_t* __fastcall Detour_At179050(std::uint32_t* param1, std::int64_t param2) {
        logger::info("[DIMT] Detour_At179050 hit!");
        logger::info("[DIMT] param1: {:p}, param2: 0x{:016X}", static_cast<void*>(param1),
                     static_cast<std::uint64_t>(param2));

        // Call original function to avoid crash
        return originalFunc(param1, param2);
    }
}

void InstallHooks() {
    logger::info("[DIMT] Installing Detour_At179050...");

    // Write trampoline jump
    auto& trampoline = SKSE::GetTrampoline();
    originalFunc = reinterpret_cast<FnOriginal>(trampoline.write_call<5>(targetFunc.address(),  // target function start
                                            Detour_At179050)        // our detour
    );

    logger::info("[DIMT] Hook installed at 0x{:X}", targetFunc.address());
}
