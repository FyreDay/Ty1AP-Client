#include "TrackerWindow.h"

void TrackerWindow::ToggleVisibility() {
    isVisible = !isVisible;
}

void TrackerWindow::Draw(int outerWidth, int outerHeight, float uiScale) {
    if (!isVisible)
        return;

    if (GameState::onLoadScreenOrMainMenu())
        return;

    auto windowWidth = 680 * uiScale;
    auto windowHeight = 330 * uiScale;
    auto iconSize = 48 * uiScale;
    ImGui::SetNextWindowPos(ImVec2(outerWidth - 10 - windowWidth, outerHeight - 10 - windowHeight), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(windowWidth, windowHeight), ImGuiCond_Always);
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0.5));
    ImGui::Begin(name.c_str(), nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);

    ImVec2 cursorPos = ImGui::GetCursorScreenPos();
    ImVec4 tintColor = ImVec4(0.5, 0, 0, 1.0f);
    ImGui::Image((ImTextureID)(intptr_t)GUI::icons["thegg"], ImVec2(iconSize, iconSize), ImVec2(0,0), ImVec2(1,1), tintColor);
    ImVec2 textSize = ImGui::CalcTextSize(std::to_string(SaveDataHandler::saveData.FireThunderEggCount).c_str());
    ImVec2 textPos = ImVec2(cursorPos.x + (iconSize - textSize.x) * 0.5f,
        cursorPos.y + (iconSize - textSize.y) * 0.5f);
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    drawList->AddText(textPos, IM_COL32(255, 255, 255, 255), std::to_string(SaveDataHandler::saveData.FireThunderEggCount).c_str());
    ImGui::SameLine();

    cursorPos = ImGui::GetCursorScreenPos();
    tintColor = ImVec4(0, 0, 0.5, 1.0f);
    ImGui::Image((ImTextureID)(intptr_t)GUI::icons["thegg"], ImVec2(iconSize, iconSize), ImVec2(0, 0), ImVec2(1, 1), tintColor);
    textSize = ImGui::CalcTextSize(std::to_string(SaveDataHandler::saveData.IceThunderEggCount).c_str());
    textPos = ImVec2(cursorPos.x + (iconSize - textSize.x) * 0.5f,
        cursorPos.y + (iconSize - textSize.y) * 0.5f);
    drawList->AddText(textPos, IM_COL32(255, 255, 255, 255), std::to_string(SaveDataHandler::saveData.IceThunderEggCount).c_str());

    ImGui::SameLine();
    cursorPos = ImGui::GetCursorScreenPos();
    tintColor = ImVec4(0, 0.5, 0, 1.0f);
    ImGui::Image((ImTextureID)(intptr_t)GUI::icons["thegg"], ImVec2(iconSize, iconSize), ImVec2(0, 0), ImVec2(1, 1), tintColor);
    textSize = ImGui::CalcTextSize(std::to_string(SaveDataHandler::saveData.AirThunderEggCount).c_str());
    textPos = ImVec2(cursorPos.x + (iconSize - textSize.x) * 0.5f,
        cursorPos.y + (iconSize - textSize.y) * 0.5f);
    drawList->AddText(textPos, IM_COL32(255, 255, 255, 255), std::to_string(SaveDataHandler::saveData.AirThunderEggCount).c_str());

    ImGui::SameLine();
    cursorPos = ImGui::GetCursorScreenPos();
    tintColor = ImVec4(0.5, 0.5, 0.5, 1.0f);
    ImGui::Image((ImTextureID)(intptr_t)GUI::icons["cog"], ImVec2(iconSize, iconSize), ImVec2(0, 0), ImVec2(1, 1), tintColor);
    textSize = ImGui::CalcTextSize(std::to_string(SaveDataHandler::saveData.GoldenCogCount).c_str());
    textPos = ImVec2(cursorPos.x + (iconSize - textSize.x) * 0.5f,
        cursorPos.y + (iconSize - textSize.y) * 0.5f);
    drawList->AddText(textPos, IM_COL32(255, 255, 255, 255), std::to_string(SaveDataHandler::saveData.GoldenCogCount).c_str());

    for (bool b : SaveDataHandler::saveData.Talismans) {
        ImGui::SameLine();
        cursorPos = ImGui::GetCursorScreenPos();
        auto brightness = b ? 0.7f : 0.2f;
        tintColor = ImVec4(brightness, brightness, brightness, 1.0f);
        ImGui::Image((ImTextureID)(intptr_t)GUI::icons["talisman"], ImVec2(iconSize, iconSize), ImVec2(0, 0), ImVec2(1, 1), tintColor);
    }

    // Indices to extract
    constexpr std::array<int, 9> indices = { 4, 5, 6, 8, 9, 10, 12, 13, 14 };
    std::array<bool, 9> extractedStopwatchBools;
    for (size_t i = 0; i < indices.size(); ++i) {
        extractedStopwatchBools[i] = SaveDataHandler::saveData.StopwatchesActive[indices[i]];
    }

    for (bool b : extractedStopwatchBools) {
        cursorPos = ImGui::GetCursorScreenPos();
        auto brightness = b ? 0.7f : 0.2f;
        tintColor = ImVec4(brightness, brightness, brightness, 1.0f);
        ImGui::Image((ImTextureID)(intptr_t)GUI::icons["stopwatch"], ImVec2(iconSize, iconSize), ImVec2(0, 0), ImVec2(1, 1), tintColor);
        ImGui::SameLine();
    }

    ImGui::NewLine();

    for (int i : indices) {
        cursorPos = ImGui::GetCursorScreenPos();
        tintColor = ImVec4(0.5, 0.5, 0.5, 1.0f);
        ImGui::Image((ImTextureID)(intptr_t)GUI::icons["bilby"], ImVec2(iconSize, iconSize), ImVec2(0, 0), ImVec2(1, 1), tintColor);
        textSize = ImGui::CalcTextSize(std::to_string(SaveDataHandler::saveData.BilbyCount[i]).c_str());
        textPos = ImVec2(cursorPos.x + (iconSize - textSize.x) * 0.5f,
            cursorPos.y + (iconSize - textSize.y) * 0.5f);
        drawList->AddText(textPos, IM_COL32(255, 255, 255, 255), std::to_string(SaveDataHandler::saveData.BilbyCount[i]).c_str());
        ImGui::SameLine();
    }

    ImGui::NewLine();

    constexpr std::array<int, 13> levelIndices = { 5, 6, 7, 8, 9, 10, 19, 12, 13, 14, 15, 20 };

    for (int i : levelIndices) {
        auto b = SaveDataHandler::saveData.PortalOpen[i];
        auto brightness = b ? 0.7f : 0.2f;
        tintColor = ImVec4(brightness, brightness, brightness, 1.0f);
        ImGui::Image((ImTextureID)(intptr_t)GUI::icons["level"], ImVec2(iconSize, iconSize), ImVec2(0, 0), ImVec2(1, 1), tintColor);
        ImGui::SameLine();
    }

    ImGui::NewLine();

    auto brightness = SaveDataHandler::saveData.AttributeData.GotBoomerang  ? 0.7f : 0.2f;
    tintColor = ImVec4(brightness, brightness, brightness, 1.0f);
    ImGui::Image((ImTextureID)(intptr_t)GUI::icons["rang"], ImVec2(iconSize, iconSize), ImVec2(0, 0), ImVec2(1, 1), tintColor);

    ImGui::SameLine();

    brightness = SaveDataHandler::saveData.AttributeData.GotSecondRang ? 0.7f : 0.2f;
    tintColor = ImVec4(brightness, brightness, brightness, 1.0f);
    ImGui::Image((ImTextureID)(intptr_t)GUI::icons["rang"], ImVec2(iconSize, iconSize), ImVec2(0, 0), ImVec2(1, 1), tintColor);

    ImGui::SameLine();

    brightness = SaveDataHandler::saveData.AttributeData.LearntToSwim ? 0.7f : 0.2f;
    tintColor = ImVec4(brightness, brightness, brightness, 1.0f);
    ImGui::Image((ImTextureID)(intptr_t)GUI::icons["rang"], ImVec2(iconSize, iconSize), ImVec2(0, 0), ImVec2(1, 1), tintColor);

    ImGui::SameLine();

    brightness = SaveDataHandler::saveData.AttributeData.GotAquarang ? 0.7f : 0.2f;
    tintColor = ImVec4(brightness, brightness, brightness, 1.0f);
    ImGui::Image((ImTextureID)(intptr_t)GUI::icons["rang"], ImVec2(iconSize, iconSize), ImVec2(0, 0), ImVec2(1, 1), tintColor);

    ImGui::SameLine();

    brightness = SaveDataHandler::saveData.AttributeData.LearntToDive ? 0.7f : 0.2f;
    tintColor = ImVec4(brightness, brightness, brightness, 1.0f);
    ImGui::Image((ImTextureID)(intptr_t)GUI::icons["rang"], ImVec2(iconSize, iconSize), ImVec2(0, 0), ImVec2(1, 1), tintColor);
    
    ImGui::SameLine();

    brightness = SaveDataHandler::saveData.AttributeData.GotFlamerang ? 0.7f : 0.2f;
    tintColor = ImVec4(brightness, brightness, brightness, 1.0f);
    ImGui::Image((ImTextureID)(intptr_t)GUI::icons["rang"], ImVec2(iconSize, iconSize), ImVec2(0, 0), ImVec2(1, 1), tintColor);

    ImGui::SameLine();

    brightness = SaveDataHandler::saveData.AttributeData.GotFrostyrang ? 0.7f : 0.2f;
    tintColor = ImVec4(brightness, brightness, brightness, 1.0f);
    ImGui::Image((ImTextureID)(intptr_t)GUI::icons["rang"], ImVec2(iconSize, iconSize), ImVec2(0, 0), ImVec2(1, 1), tintColor);

    ImGui::SameLine();

    brightness = SaveDataHandler::saveData.AttributeData.GotZappyrang ? 0.7f : 0.2f;
    tintColor = ImVec4(brightness, brightness, brightness, 1.0f);
    ImGui::Image((ImTextureID)(intptr_t)GUI::icons["rang"], ImVec2(iconSize, iconSize), ImVec2(0, 0), ImVec2(1, 1), tintColor);

    ImGui::SameLine();

    brightness = SaveDataHandler::saveData.AttributeData.GotDoomerang ? 0.7f : 0.2f;
    tintColor = ImVec4(brightness, brightness, brightness, 1.0f);
    ImGui::Image((ImTextureID)(intptr_t)GUI::icons["rang"], ImVec2(iconSize, iconSize), ImVec2(0, 0), ImVec2(1, 1), tintColor);

    brightness = SaveDataHandler::saveData.AttributeData.GotZoomerang ? 0.7f : 0.2f;
    tintColor = ImVec4(brightness, brightness, brightness, 1.0f);
    ImGui::Image((ImTextureID)(intptr_t)GUI::icons["rang"], ImVec2(iconSize, iconSize), ImVec2(0, 0), ImVec2(1, 1), tintColor);

    ImGui::SameLine();

    brightness = SaveDataHandler::saveData.AttributeData.GotMultirang ? 0.7f : 0.2f;
    tintColor = ImVec4(brightness, brightness, brightness, 1.0f);
    ImGui::Image((ImTextureID)(intptr_t)GUI::icons["rang"], ImVec2(iconSize, iconSize), ImVec2(0, 0), ImVec2(1, 1), tintColor);

    ImGui::SameLine();

    brightness = SaveDataHandler::saveData.AttributeData.GotInfrarang ? 0.7f : 0.2f;
    tintColor = ImVec4(brightness, brightness, brightness, 1.0f);
    ImGui::Image((ImTextureID)(intptr_t)GUI::icons["rang"], ImVec2(iconSize, iconSize), ImVec2(0, 0), ImVec2(1, 1), tintColor);

    ImGui::SameLine();

    brightness = SaveDataHandler::saveData.AttributeData.GotMegarang ? 0.7f : 0.2f;
    tintColor = ImVec4(brightness, brightness, brightness, 1.0f);
    ImGui::Image((ImTextureID)(intptr_t)GUI::icons["rang"], ImVec2(iconSize, iconSize), ImVec2(0, 0), ImVec2(1, 1), tintColor);

    ImGui::SameLine();

    brightness = SaveDataHandler::saveData.AttributeData.GotKaboomerang ? 0.7f : 0.2f;
    tintColor = ImVec4(brightness, brightness, brightness, 1.0f);
    ImGui::Image((ImTextureID)(intptr_t)GUI::icons["rang"], ImVec2(iconSize, iconSize), ImVec2(0, 0), ImVec2(1, 1), tintColor);

    ImGui::SameLine();

    brightness = SaveDataHandler::saveData.AttributeData.GotChronorang ? 0.7f : 0.2f;
    tintColor = ImVec4(brightness, brightness, brightness, 1.0f);
    ImGui::Image((ImTextureID)(intptr_t)GUI::icons["rang"], ImVec2(iconSize, iconSize), ImVec2(0, 0), ImVec2(1, 1), tintColor);


    ImGui::End();
    ImGui::PopStyleColor();
}