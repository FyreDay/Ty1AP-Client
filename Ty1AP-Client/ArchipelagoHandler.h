#pragma once
#include "apclient.hpp"
#include "GameHandler.h"
#include "uuid.h"
#include "nlohmann/json.hpp"
#include <fstream>
#include <map>
#include "TygerFrameworkAPI.hpp"
#include "gui.h"
#include "LoginWindow.h"
#include "gamestate.h"


enum LevelUnlockStyle {
	VANILLA,
	CHECKS,
	VANILLA_BOSS
};

enum Goal {
	BEAT_CASS,
	ALL_BOSSES,
	ALL_THEGGS,
	COMPLETION
};

class LoginWindow;

class ArchipelagoHandler
{
public:
	static inline bool polling;
	static void Poll();
	static void ConnectAP(LoginWindow* login);
	static bool IsConnected();
private:
	static void SetAPStatus(std::string status, char important);
};

