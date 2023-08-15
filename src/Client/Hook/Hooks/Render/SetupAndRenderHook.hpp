

#pragma once
#include "../../../../SDK/Client/Render/MinecraftUIRenderContext.hpp"
#include "../../../../SDK/Client/Render/ScreenView/ScreenView.hpp"
#include "../../../../SDK/Client/Render/BaseActorRenderContext.hpp"
#include "../../../../SDK/SDK.hpp"
#include "../Hook.hpp"
#include "../../../../SDK/Client/Render/ItemRenderer.hpp"
#include <format>

bool sendoncethx = false;

class SetUpAndRenderHook : public Hook
{
private:

    static void drawTextCallback(MinecraftUIRenderContext* ctx, void* font, float* pos, std::string* text, float* color, float alpha, unsigned int textAlignment, void* textMeasureData, void* caretMeasureData) {

        if (SDK::clientInstance != nullptr) {
            if (SDK::clientInstance->getLocalPlayer() != nullptr) {
                if (ModuleManager::getModule("Nick") != nullptr) {
                    auto  module = ModuleManager::getModule("Nick");
                    if (module->settings.getSettingByName<bool>("enabled")->value) {
                        std::string localPlayerName = SDK::clientInstance->getLocalPlayer()->playerName;
                        size_t found = text->find(localPlayerName);
                        if (found != std::string::npos) {
                            *text = module->settings.getSettingByName<std::string>("nick")->value + text->substr(found + localPlayerName.length());
                        }
                    }
                }
            }
        }

        func_originalText(ctx, font, pos, text, color, alpha, textAlignment, textMeasureData, caretMeasureData);
    }


    static void SetUpAndRenderCallback(ScreenView* pScreenView, MinecraftUIRenderContext* muirc) {

        SDK::hasInstanced = true;
        SDK::clientInstance = muirc->getclientInstance();
        SDK::screenView = pScreenView;

        std::string layer = SDK::screenView->VisualTree->root->LayerName;

        if (layer != "debug_screen" && layer != "toast_screen") {
            SDK::CurrentScreen = layer;


        }

        if (func_originalText == nullptr) {

            auto VTable = *(uintptr_t**)muirc;

            MH_CreateHook((void*)VTable[5], drawTextCallback, (LPVOID*)&func_originalText);
            if (MH_EnableHook((void*)VTable[5]) != MH_OK)
            {
                Logger::error(std::format("Failed to find address of {}", "drawText"));
            }
            else {
                Logger::info(std::format("[Hook] Successfully hooked {}", "drawText"));
            }

        }

        SetupAndRenderEvent e;
        e.muirc = muirc;

        // SDK::clientInstance->getBlockSource()->dimension->weather->rainLevel = 1.0f;
        // SDK::clientInstance->getBlockSource()->dimension->weather->lightingLevel = 1.0f;

        EventHandler::onSetupAndRender(e);
        func_original(pScreenView, muirc);
    }


public:
    typedef void(__thiscall* setUpAndRenderOriginal)(void*, MinecraftUIRenderContext*);
    static inline  setUpAndRenderOriginal func_original = nullptr;

    typedef void(__thiscall* drawTextOriginal)(MinecraftUIRenderContext*, void*, float*, std::string*, float*, float, unsigned int, const void*, const void*);
    static inline  drawTextOriginal func_originalText = nullptr;
    SetUpAndRenderHook() : Hook("SetupAndRender", "48 8B C4 48 89 58 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D A8 ? ? ? ? 48 81 EC ? ? ? ? ? ? 70 B8 ? ? 78 A8 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 4C 8B F2 48 89 54 ? ? 4C") {
    }

    void enableHook() override {
        this->autoHook(SetUpAndRenderCallback, (void**)&func_original);
    }
};