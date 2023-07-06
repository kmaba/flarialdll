#pragma once

#include "../Module.hpp"
#include "../../../Events/EventHandler.hpp"
#include "KeystrokesListener.hpp"


class Keystrokes : public Module {

public:

    enum Strokes {
        W,
        A,
        S,
        D,
        SPACEBAR,
        LMB,
        RMB
    };

    std::vector<D2D1_COLOR_F> states;

    Keystrokes() : Module("Keystrokes", "yes br", "\\Flarial\\assets\\auto_sprint.png", 'M') {

        onEnable();
        D2D1_COLOR_F d = FlarialGUI::HexToColorF(settings.getSettingByName<std::string>("bgColor")->value);
        d.a = settings.getSettingByName<float>("bgOpacity")->value;
        states = std::vector<D2D1_COLOR_F>(7, d);

    };

    void onEnable() override {

        Module::onEnable();

        if (settings.getSettingByName<bool>("cps") == nullptr) {
            settings.addSetting("cps", false);
        }

        if (settings.getSettingByName<float>("percentageX") == nullptr) {
            settings.addSetting("percentageX", 0.0f);
            settings.addSetting("percentageY", 0.0f);
        }

        if (settings.getSettingByName<bool>("border") == nullptr) {
            settings.addSetting("border", true);
            settings.addSetting("borderWidth", 1.0f);
        }

        if (settings.getSettingByName<float>("rounding") == nullptr) settings.addSetting("rounding", 11.0f);

        if (settings.getSettingByName<std::string>("bgColor") == nullptr) {
            settings.addSetting("bgColor", (std::string) "120e0f");
            settings.addSetting("textColor", (std::string) "3f2a2d");
            settings.addSetting("enabledColor", (std::string) "fafafa");

        }

        if (settings.getSettingByName<float>("bgOpacity") == nullptr) {
            settings.addSetting("bgOpacity", 1.0f);
            settings.addSetting("textOpacity", 1.0f);
            settings.addSetting("enabledOpacity", 1.0f);
        }

        if (settings.getSettingByName<float>("uiscale") == nullptr) {

            settings.addSetting("uiscale", 1.0f);
        }

        EventHandler::registerListener(new KeystrokesListener("Keystrok", this));
    }

    void onDisable() override {

        EventHandler::unregisterListener("Keystrok");

        Module::onDisable();

    }

    void SettingsRender() override {
        /* Border Start */

        float toggleX = Constraints::PercentageConstraint(0.019, "left");
        float toggleY = Constraints::PercentageConstraint(0.10, "top");

        const float textWidth = Constraints::RelativeConstraint(0.12, "height", true);
        const float textHeight = Constraints::RelativeConstraint(0.029, "height", true);

        FlarialGUI::ScrollBar(toggleX, toggleY, 140, 40, 2);
        FlarialGUI::SetScrollView(toggleX, toggleY, Constraints::RelativeConstraint(1.0, "width"), Constraints::RelativeConstraint(1.0, "height"));

        FlarialGUI::FlarialTextWithFont(toggleX, toggleY, L"UI Scale", D2D1::ColorF(D2D1::ColorF::White), textWidth,
                                        textHeight, DWRITE_TEXT_ALIGNMENT_LEADING,
                                        Constraints::RelativeConstraint(0.12, "height", true));

        float percent = FlarialGUI::Slider(4, toggleX + Constraints::SpacingConstraint(0.65, textWidth),
                                           toggleY,
                                           D2D1::ColorF(255.0f / 255.0f, 36.0f / 255.0f, 56.0f / 255.0f),
                                           D2D1::ColorF(154.0f / 255.0f, 107.0f / 255.0f, 114.0f / 255.0f),
                                           D2D1::ColorF(D2D1::ColorF::White),
                                           this->settings.getSettingByName<float>("uiscale")->value, 2.0f);

        this->settings.getSettingByName<float>("uiscale")->value = percent;



        /* Rounding Start */
        toggleY += Constraints::SpacingConstraint(0.35, textWidth);
        toggleX -= Constraints::SpacingConstraint(0.175, textWidth);

        FlarialGUI::FlarialText(toggleX, toggleY, L"Rounding", D2D1::ColorF(D2D1::ColorF::White), textWidth * 1.05f,
                                textHeight, DWRITE_TEXT_ALIGNMENT_CENTER);

        percent = FlarialGUI::Slider(6, toggleX + Constraints::SpacingConstraint(0.95, textWidth),
                                     toggleY,
                                     D2D1::ColorF(255.0f / 255.0f, 36.0f / 255.0f, 56.0f / 255.0f),
                                     D2D1::ColorF(154.0f / 255.0f, 107.0f / 255.0f, 114.0f / 255.0f),
                                     D2D1::ColorF(D2D1::ColorF::White),
                                     this->settings.getSettingByName<float>("rounding")->value);

        this->settings.getSettingByName<float>("rounding")->value = percent;

        toggleY += Constraints::SpacingConstraint(0.30, textWidth);
        toggleX = Constraints::PercentageConstraint(0.019, "left");

        FlarialGUI::FlarialTextWithFont(toggleX + Constraints::SpacingConstraint(0.60, textWidth), toggleY,
                                        FlarialGUI::to_wide("Show CPS").c_str(), D2D1::ColorF(D2D1::ColorF::White),
                                        textWidth * 1.4f, textHeight, DWRITE_TEXT_ALIGNMENT_LEADING,
                                        Constraints::SpacingConstraint(1.05, textWidth));

        if (FlarialGUI::Toggle(2, toggleX, toggleY, D2D1::ColorF(255.0f / 255.0f, 35.0f / 255.0f, 58.0f / 255.0f),
                               D2D1::ColorF(112.0f / 255.0f, 75.0f / 255.0f, 82.0f / 255.0f),
                               D2D1::ColorF(D2D1::ColorF::White),
                               this->settings.getSettingByName<bool>("cps")->value))
            this->settings.getSettingByName<bool>("cps")->value = !this->settings.getSettingByName<bool>(
                    "cps")->value;

        /* Rounding End */

        /* Color Pickers Start*/

        toggleX = Constraints::PercentageConstraint(0.55, "left");
        toggleY -= Constraints::SpacingConstraint(0.30, textWidth);

        FlarialGUI::FlarialTextWithFont(toggleX, toggleY, FlarialGUI::to_wide("Unpressed").c_str(),
                                        D2D1::ColorF(D2D1::ColorF::White), textWidth * 1.4f, textHeight,
                                        DWRITE_TEXT_ALIGNMENT_LEADING, Constraints::SpacingConstraint(1.05, textWidth));
        FlarialGUI::ColorPicker(3, toggleX + Constraints::SpacingConstraint(0.79, textWidth),
                                toggleY - Constraints::SpacingConstraint(0.017, textWidth),
                                settings.getSettingByName<std::string>("bgColor")->value,
                                settings.getSettingByName<float>("bgOpacity")->value);

        toggleX = Constraints::PercentageConstraint(0.55, "left");
        toggleY = Constraints::PercentageConstraint(0.10, "top");

        FlarialGUI::FlarialTextWithFont(toggleX, toggleY, FlarialGUI::to_wide("Text").c_str(),
                                        D2D1::ColorF(D2D1::ColorF::White), textWidth * 1.4f, textHeight,
                                        DWRITE_TEXT_ALIGNMENT_LEADING, Constraints::SpacingConstraint(1.05, textWidth));
        FlarialGUI::ColorPicker(4, toggleX + Constraints::SpacingConstraint(0.40, textWidth), toggleY * 0.99f,
                                settings.getSettingByName<std::string>("textColor")->value,
                                settings.getSettingByName<float>("textOpacity")->value);

        toggleY += Constraints::SpacingConstraint(0.70, textWidth);

        FlarialGUI::FlarialTextWithFont(toggleX, toggleY, FlarialGUI::to_wide("Pressed").c_str(),
                                        D2D1::ColorF(D2D1::ColorF::White), textWidth * 1.4f, textHeight,
                                        DWRITE_TEXT_ALIGNMENT_LEADING, Constraints::SpacingConstraint(1.05, textWidth));
        FlarialGUI::ColorPicker(5, toggleX + Constraints::SpacingConstraint(0.63, textWidth), toggleY * 0.99f,
                                settings.getSettingByName<std::string>("enabledColor")->value,
                                settings.getSettingByName<float>("enabledColor")->value);

        FlarialGUI::ColorPickerWindow(3, settings.getSettingByName<std::string>("bgColor")->value,
                                      settings.getSettingByName<float>("bgOpacity")->value);
        FlarialGUI::ColorPickerWindow(4, settings.getSettingByName<std::string>("textColor")->value,
                                      settings.getSettingByName<float>("textOpacity")->value);
        FlarialGUI::ColorPickerWindow(5, settings.getSettingByName<std::string>("enabledColor")->value,
                                      settings.getSettingByName<float>("enabledOpacity")->value);
        /* Color Pickers End */

        FlarialGUI::UnsetScrollView();

    }

    void NormalRender(int index, std::string text, std::string value) override {

        if (SDK::hasInstanced) {
            if (SDK::clientInstance->getLocalPlayer() != nullptr) {

                float keycardSize = Constraints::RelativeConstraint(
                        0.05f * this->settings.getSettingByName<float>("uiscale")->value, "height", true);
                float spacing = Constraints::RelativeConstraint(
                        0.0015f * this->settings.getSettingByName<float>("uiscale")->value, "height", true);

                Vec2<float> settingperc = Vec2<float>(this->settings.getSettingByName<float>("percentageX")->value,
                                                      this->settings.getSettingByName<float>("percentageY")->value);

                Vec2<float> realcenter;

                if (settingperc.x != 0)
                    realcenter = Vec2<float>(settingperc.x * MC::windowSize.x,
                                             settingperc.y * MC::windowSize.y);
                else
                    realcenter = Constraints::CenterConstraint(keycardSize, keycardSize);

                Vec2<float> rounde = Constraints::RoundingConstraint(
                        this->settings.getSettingByName<float>("rounding")->value *
                        settings.getSettingByName<float>("uiscale")->value,
                        this->settings.getSettingByName<float>("rounding")->value *
                        settings.getSettingByName<float>("uiscale")->value);

                float totalWidth = keycardSize * 3 + spacing * 2;

                FlarialGUI::SetWindowRect(realcenter.x, realcenter.y, totalWidth, totalWidth, index);

                Vec2<float> vec2 = FlarialGUI::CalculateMovedXY(realcenter.x, realcenter.y, index, totalWidth,
                                                                totalWidth);

                realcenter.x = vec2.x;
                realcenter.y = vec2.y;

                realcenter = realcenter;

                Vec2<float> percentages = Constraints::CalculatePercentage(realcenter.x, realcenter.y);

                this->settings.setValue("percentageX", percentages.x);
                this->settings.setValue("percentageY", percentages.y);

                float fontSize = Constraints::SpacingConstraint(2.4, keycardSize);

                D2D1_COLOR_F enabledColor = FlarialGUI::HexToColorF(
                        settings.getSettingByName<std::string>("enabledColor")->value);
                D2D1_COLOR_F disabledColor = FlarialGUI::HexToColorF(
                        settings.getSettingByName<std::string>("bgColor")->value);
                D2D1_COLOR_F textColor = FlarialGUI::HexToColorF(
                        settings.getSettingByName<std::string>("textColor")->value);

                disabledColor.a = settings.getSettingByName<float>("bgOpacity")->value;
                textColor.a = settings.getSettingByName<float>("textOpacity")->value;
                enabledColor.a = settings.getSettingByName<float>("enabledOpacity")->value;

                MoveInputComponent *handler = SDK::clientInstance->getLocalPlayer()->getMoveInputHandler();

                if (handler->forward)
                    states[Strokes::W] = FlarialGUI::LerpColor(states[Strokes::W], enabledColor,
                                                               0.15f * FlarialGUI::frameFactor);
                else
                    states[Strokes::W] = FlarialGUI::LerpColor(states[Strokes::W], disabledColor,
                                                               0.15f * FlarialGUI::frameFactor);

                if (handler->backward)
                    states[Strokes::S] = FlarialGUI::LerpColor(states[Strokes::S], enabledColor,
                                                               0.15f * FlarialGUI::frameFactor);
                else
                    states[Strokes::S] = FlarialGUI::LerpColor(states[Strokes::S], disabledColor,
                                                               0.15f * FlarialGUI::frameFactor);

                if (handler->left)
                    states[Strokes::A] = FlarialGUI::LerpColor(states[Strokes::A], enabledColor,
                                                               0.15f * FlarialGUI::frameFactor);
                else
                    states[Strokes::A] = FlarialGUI::LerpColor(states[Strokes::A], disabledColor,
                                                               0.15f * FlarialGUI::frameFactor);

                if (handler->right)
                    states[Strokes::D] = FlarialGUI::LerpColor(states[Strokes::D], enabledColor,
                                                               0.15f * FlarialGUI::frameFactor);
                else
                    states[Strokes::D] = FlarialGUI::LerpColor(states[Strokes::D], disabledColor,
                                                               0.15f * FlarialGUI::frameFactor);

                if (handler->jumping)
                    states[Strokes::SPACEBAR] = FlarialGUI::LerpColor(states[Strokes::SPACEBAR], enabledColor,
                                                                      0.15f * FlarialGUI::frameFactor);
                else
                    states[Strokes::SPACEBAR] = FlarialGUI::LerpColor(states[Strokes::SPACEBAR], disabledColor,
                                                                      0.15f * FlarialGUI::frameFactor);



                // W
                FlarialGUI::RoundedRect(realcenter.x, realcenter.y, states[Strokes::W], keycardSize, keycardSize,
                                        rounde.x,
                                        rounde.x);
                FlarialGUI::FlarialTextWithFont(realcenter.x, realcenter.y, L"W", textColor, keycardSize, keycardSize,
                                                DWRITE_TEXT_ALIGNMENT_CENTER, fontSize);

                // S
                realcenter.y += (keycardSize + spacing);
                FlarialGUI::RoundedRect(realcenter.x, realcenter.y, states[Strokes::S], keycardSize, keycardSize,
                                        rounde.x,
                                        rounde.x);

                FlarialGUI::FlarialTextWithFont(realcenter.x, realcenter.y, L"S", textColor, keycardSize, keycardSize,
                                                DWRITE_TEXT_ALIGNMENT_CENTER, fontSize);
                // A
                realcenter.x -= (keycardSize + spacing);
                FlarialGUI::RoundedRect(realcenter.x, realcenter.y, states[Strokes::A], keycardSize, keycardSize,
                                        rounde.x,
                                        rounde.x);
                FlarialGUI::FlarialTextWithFont(realcenter.x, realcenter.y, L"A", textColor, keycardSize, keycardSize,
                                                DWRITE_TEXT_ALIGNMENT_CENTER, fontSize);

                // D
                realcenter.x += 2 * (keycardSize + spacing);
                FlarialGUI::RoundedRect(realcenter.x, realcenter.y, states[Strokes::D], keycardSize, keycardSize,
                                        rounde.x,
                                        rounde.x);
                FlarialGUI::FlarialTextWithFont(realcenter.x, realcenter.y, L"D", textColor, keycardSize, keycardSize,
                                                DWRITE_TEXT_ALIGNMENT_CENTER, fontSize);

                // SPACEBAR
                float spacebarWidth = 3 * (keycardSize) + 2 * spacing;
                float spacebarHeight = 0.55f * (keycardSize);
                realcenter.x -= 2 * (keycardSize + spacing);

                if (!settings.getSettingByName<bool>("cps")->value) realcenter.y += (keycardSize + spacing);
                else realcenter.y += 1.25f * (keycardSize + spacing);

                FlarialGUI::RoundedRect(realcenter.x, realcenter.y, states[Strokes::SPACEBAR], spacebarWidth,
                                        spacebarHeight, rounde.x, rounde.x);
                FlarialGUI::FlarialTextWithFont(realcenter.x, realcenter.y, L"-------", textColor, spacebarWidth,
                                                spacebarHeight, DWRITE_TEXT_ALIGNMENT_CENTER, fontSize);

                FlarialGUI::UnsetWindowRect();

            }
        }
    }
};