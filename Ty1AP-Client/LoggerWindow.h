#pragma once
#include "Window.h"
#include <string>
#include <chrono>
#include <vector>
#include "gui.h"

class LoggerWindow : public Window
{
public:
	LoggerWindow() : Window("Logger") {};
	void ToggleVisibility() override;
	void Draw(int outerWidth, int outerHeight, float uiScale) override;
	void AddLogMessage(const std::string& message);
	static void Log(const std::string& message);
private:
	struct LogMessage {
		std::string message;
		std::chrono::steady_clock::time_point timestamp;
	};
	std::vector<LogMessage> logMessages;
	void RenderFormattedText(ImDrawList* draw_list, const char* text, ImVec2 pos);
};

