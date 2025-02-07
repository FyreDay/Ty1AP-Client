#pragma once
#include "ArchipelagoHandler.h"
#include "imgui.h"
#include "gui.h"
#include "SaveDataHandler.h"
#include <string>
#include "Window.h"

class TrackerWindow : public Window {
public:
    TrackerWindow() : Window("Tracker") {};
    void ToggleVisibility() override;
    void Draw(int outerWidth, int outerHeight, float uiScale) override;
};
