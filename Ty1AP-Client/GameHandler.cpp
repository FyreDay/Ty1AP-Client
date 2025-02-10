#include "GameHandler.h"

bool GameHandler::isTyShutdown = false;

std::unordered_map<int, std::vector<float>> spawnpointMap{
	{ 4, {-521.0f, 2372.0f, 11286.0f, 1.530f} },
	{ 5, {-792.0f, 2372.0f, 12520.0f, 1.400f} },
	{ 6, {1357.0f, 2372.0f, 12542.0f, -1.460f} },
	{ 7, {483.0f, 2372.0f, 12543.0f, 0.028f} },
	{ 8, {-3349.0f, 148.0f, 9256.0f, 1.810f} },
	{ 9, {-3349.0f, 148.0f, 9256.0f, 2.030f} },
	{ 10, {-3349.0f, 148.0f, 9256.0f, 1.810f} },
	{ 19, {-3349.0f, 148.0f, 9256.0f, 1.859f} },
	{ 12, {5038.0f, 1029.0f, 8098.0f, 1.820f} },
	{ 13, {4803.0f, 1024.0f, 8855.0f, 1.810f} },
	{ 14, {4646.0f, 1025.0f, 9410.0f, 1.790f} },
	{ 15, {4161.0f, 1054.0f, 10735.0f, 1.458f} },
};

typedef void(__stdcall* MainMenuFunctionType)(void);
MainMenuFunctionType mainMenuOrigin = nullptr;

typedef void(__stdcall* SpawnpointFunctionType)(void);
SpawnpointFunctionType spawnpointOrigin = nullptr;

typedef void(__stdcall* StopwatchFunctionType)(void);
StopwatchFunctionType stopwatchOrigin = nullptr;

typedef void(__stdcall* LoadGameFunctionType)(void);
LoadGameFunctionType loadGameOrigin = nullptr;

typedef void(__stdcall* MenuStateFunctionType)(void);
MenuStateFunctionType menuStateOrigin = nullptr;


void __stdcall GameHandler::MainMenuHook() {
	GameHandler::OnMainMenu();
	mainMenuOrigin();
}

void PatchRangMemory(std::vector<std::pair<uintptr_t, short>> patches) {
	DWORD oldProtect;
	for (const auto& patch : patches) {
		char* addr = (char*)(Core::moduleBase + patch.first);
		VirtualProtect(addr, 2, PAGE_EXECUTE_READWRITE, &oldProtect);
		*(short*)addr = patch.second;
	}
}

uintptr_t stopwatchJmpAddr;
uintptr_t stopwatchOriginAddr;
__declspec(naked) void __stdcall GameHandler::StopwatchHook() {
	__asm {
		cmp eax, 0x2
		je skip
		cmp eax, 0x3
		je skip
		mov[esi + 0x68], eax 
	skip :
		jmp[stopwatchOriginAddr]
	}
}

uintptr_t spawnpointOriginAddr;
void (*loadEndTransitionFunc)();
__declspec(naked) void __stdcall GameHandler::SpawnpointHook() {
	__asm {
		pushfd
		call GameHandler::OnSpawnpointSet
		popfd
		call loadEndTransitionFunc
		jmp dword ptr[spawnpointOriginAddr]
	}
}

uintptr_t loadGameOriginAddr;
__declspec(naked) void __stdcall GameHandler::LoadGameHook() {
	__asm {
		push eax
		push ebx
		push ecx
		push edx
		push esi
		push edi
		call GameHandler::OnLoadGame
		pop edi
		pop esi
		pop edx
		pop ecx
		pop ebx
		pop eax
		jmp dword ptr[loadGameOriginAddr]
	}
}

void GameHandler::Setup()
{
	// STOPWATCH
	char* addr = (char*)(Core::moduleBase + 0xF8388);
	DWORD oldProtect;
	VirtualProtect(addr, 5, PAGE_EXECUTE_READWRITE, &oldProtect);
	memset(addr, 0x90, 5);

	// ONE RANG SWAP
	addr = (char*)(Core::moduleBase + 0x162B9C);
	VirtualProtect(addr, 6, PAGE_EXECUTE_READWRITE, &oldProtect);
	memset(addr, 0x90, 6);

	// RANG COG REQ CHECKS
	addr = (char*)(Core::moduleBase + 0xD8A4E);
	VirtualProtect(addr, 10, PAGE_EXECUTE_READWRITE, &oldProtect);
	uintptr_t goldenCogAddr = (uintptr_t)&SaveDataHandler::saveData.GoldenCogCount;
	addr[0] = 0xA1;
	*(uintptr_t*)(addr + 1) = goldenCogAddr;
	memset(addr + 5, 0x90, 5);
	addr = (char*)(Core::moduleBase + 0xD8A85);
	VirtualProtect(addr, 10, PAGE_EXECUTE_READWRITE, &oldProtect);
	goldenCogAddr = (uintptr_t)&SaveDataHandler::saveData.GoldenCogCount;
	addr[0] = 0xA1;
	*(uintptr_t*)(addr + 1) = goldenCogAddr;
	memset(addr + 5, 0x90, 5);
	addr = (char*)(Core::moduleBase + 0xD8ABC);
	VirtualProtect(addr, 10, PAGE_EXECUTE_READWRITE, &oldProtect);
	goldenCogAddr = (uintptr_t)&SaveDataHandler::saveData.GoldenCogCount;
	addr[0] = 0xA1;
	*(uintptr_t*)(addr + 1) = goldenCogAddr;
	memset(addr + 5, 0x90, 5);
	addr = (char*)(Core::moduleBase + 0xD8AF3);
	VirtualProtect(addr, 10, PAGE_EXECUTE_READWRITE, &oldProtect);
	goldenCogAddr = (uintptr_t)&SaveDataHandler::saveData.GoldenCogCount;
	addr[0] = 0xA1;
	*(uintptr_t*)(addr + 1) = goldenCogAddr;
	memset(addr + 5, 0x90, 5);
	addr = (char*)(Core::moduleBase + 0xD8B2A);
	VirtualProtect(addr, 10, PAGE_EXECUTE_READWRITE, &oldProtect);
	goldenCogAddr = (uintptr_t)&SaveDataHandler::saveData.GoldenCogCount;
	addr[0] = 0xA1;
	*(uintptr_t*)(addr + 1) = goldenCogAddr;
	memset(addr + 5, 0x90, 5);
	addr = (char*)(Core::moduleBase + 0xD8B61);
	VirtualProtect(addr, 10, PAGE_EXECUTE_READWRITE, &oldProtect);
	goldenCogAddr = (uintptr_t)&SaveDataHandler::saveData.GoldenCogCount;
	addr[0] = 0xA1;
	*(uintptr_t*)(addr + 1) = goldenCogAddr;
	memset(addr + 5, 0x90, 5);

	MH_Uninitialize();
	MH_Initialize();
	addr = (char*)(Core::moduleBase + 0xE018C);
	MH_CreateHook((LPVOID)addr, &MainMenuHook, reinterpret_cast<LPVOID*>(&mainMenuOrigin));

	spawnpointOriginAddr = Core::moduleBase + 0x16929D;
	loadEndTransitionFunc = reinterpret_cast<void(*)()>(Core::moduleBase + 0x192C20);
	addr = (char*)(Core::moduleBase + 0x169298);
	MH_CreateHook((LPVOID)addr, &SpawnpointHook, reinterpret_cast<LPVOID*>(&spawnpointOrigin));

	stopwatchJmpAddr = Core::moduleBase + 0x15A0BD;
	stopwatchOriginAddr = Core::moduleBase + 0x15A081;
	addr = (char*)(Core::moduleBase + 0x15A07C);
	MH_CreateHook((LPVOID)addr, &StopwatchHook, reinterpret_cast<LPVOID*>(&stopwatchOrigin));

	// MENU STATE LOAD SLOT BYPASS
	loadGameOriginAddr = Core::moduleBase + 0x170834;
	addr = (char*)(Core::moduleBase + 0x17082A);
	MH_CreateHook((LPVOID)addr, &LoadGameHook, reinterpret_cast<LPVOID*>(&loadGameOrigin));

	CheckHandler::SetupHooks();
	MH_EnableHook(MH_ALL_HOOKS);

	GameState::setNoIdle(true);

	// MOVE RANG CHECKS TO MY CUSTOM DATA
	std::vector<std::pair<uintptr_t, short>> patches = {
		{0x3ED9E, 0xBA8}, {0x3CA46, 0xBA8}, {0x3C9AC, 0xBA8}, {0x3F446, 0xBA8}, {0x3EEE7, 0xBA8},
		{0x16C40F, 0xBA6}, {0x165B96, 0xBA6}, {0x162B97, 0xBA6}, {0x3F8E2, 0xBA6}, {0x3EF64, 0xBA6},
		{0x2E61B, 0xBA6}, {0x2E5B9, 0xBA6}, {0x2E3FB, 0xBA6}, {0x25F75, 0xBA6}, {0x35723, 0xBA4},
		{0x35754, 0xBA4}, {0x162EFD, 0xBA4}, {0x35781, 0xBA4}, {0x357AE, 0xBA4}, {0x34D0D, 0xBA4},
		{0x34F32, 0xBA4}, {0x26B1E, 0xBA5}, {0xF8490, 0xBA7}, {0xD50B1, 0xBA7},
		{0x3E32C, 0xBA8}, {0x3E02D, 0xBA8}, {0xF356A, 0xBA8}, {0xF330E, 0xBA8}, {0xF352E, 0xBA8}, {0xF356A, 0xBA8}
	};
	PatchRangMemory(patches);

	// DETATCH AQUAS FROM REQ SWIM
	addr = (char*)(Core::moduleBase + 0x162F02);
	VirtualProtect(addr, 1, PAGE_EXECUTE_READWRITE, &oldProtect);
	memset(addr, 0xEB, 1);

	auto menuAddr = *(int*)(Core::moduleBase + 0x286644);
	*(char*)(menuAddr + 0x164) = 0x0;
	*(char*)(menuAddr + 0x165) = 0x0;
	*(char*)(menuAddr + 0x168 + 0x164) = 0x0;
	*(char*)(menuAddr + 0x168 + 0x165) = 0x0;
	*(int*)(Core::moduleBase + 0x286640) = 0x2;
	std::thread watcher(WatchMemory);
	watcher.join();
}

void GameHandler::WatchMemory() {
	auto currentLoadValue = *(int*)(Core::moduleBase + 0x27EBF0);
	auto oldLoadValue = currentLoadValue;
	auto currentMenuState = *(int*)(Core::moduleBase + 0x273F74);
	auto oldMenuState = currentMenuState;
	while (!isTyShutdown) {
		currentLoadValue = *(int*)(Core::moduleBase + 0x27EBF0);
		if (currentLoadValue != oldLoadValue) {
			if (currentLoadValue != 0 && oldLoadValue == 0)
				OnEnterLevel();
			oldLoadValue = currentLoadValue;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}

void GameHandler::SetupOnConnect(std::string seed) {
	if (SaveDataHandler::LoadSaveData(seed)) {
		SetLoadActive(true);
	}
}

void GameHandler::OnEnterRainbowCliffs() {
	auto theggCollectorCount = *(int*)(Core::moduleBase + 0x26D944);
	auto addr = *(uintptr_t*)(Core::moduleBase + 0x26D948);
	for (auto tcIndex = 0; tcIndex < theggCollectorCount; tcIndex++) {
		memset((char*)(addr + 0x94), 0x9, 1);
		memset((char*)(addr + 0x48), 0x0, 1);
		addr = *(int*)(addr + 0x34);
	}
	auto juliusCount = *(int*)(Core::moduleBase + 0x2608C4);
	addr = *(uintptr_t*)(Core::moduleBase + 0x2608C8);
	for (auto juliusIndex = 0; juliusIndex < juliusCount; juliusIndex++) {
		memset((char*)(addr + 0xA8), 0x9, 1);
		memset((char*)(addr + 0xC0), 0x0, 1);
		addr = *(int*)(addr + 0x34);
	}
	auto triggerCount = *(int*)(Core::moduleBase + 0x26DBD4);
	auto triggerAddr = *(uintptr_t*)(Core::moduleBase + 0x26DBD8);
	for (auto triggerIndex = 0; triggerIndex < triggerCount; triggerIndex++) {
		auto targetMessage = *(int*)(triggerAddr + 0xA0);
		if (targetMessage == 47) {
			memset((char*)(triggerAddr + 0x89), 0x0, 6);
			memset((char*)(triggerAddr + 0x85), 0x0, 1);
		}
		triggerAddr = *(int*)(triggerAddr + 0x34);
	}
	auto counterCount = *(int*)(Core::moduleBase + 0x26BCB8 + 0x44);
	auto counterAddr = *(uintptr_t*)(Core::moduleBase + 0x26BCB8 + 0x48);
	for (auto counterIndex = 0; counterIndex < counterCount; counterIndex++) {
		memset((char*)(counterAddr + 0xA0), 0x0, 6);
		memset((char*)(counterAddr + 0x48), 0x0, 1);
		counterAddr = *(int*)(counterAddr + 0x34);
	}

	if (ArchipelagoHandler::levelUnlockStyle != LevelUnlockStyle::VANILLA) {
		auto portalCount = *(int*)(Core::moduleBase + 0x267404);
		auto portalAddr = *(int*)(Core::moduleBase + 0x267408);
		for (auto portalIndex = 0; portalIndex < portalCount; portalIndex++) {
			auto portalDestination = *(int*)(portalAddr + 0xAC);
			if (SaveDataHandler::saveData.PortalOpen[portalDestination])
				*(int*)(portalAddr + 0x9C) = 1;
			else
				*(int*)(portalAddr + 0x9C) = 3;
			portalAddr = *(int*)(portalAddr + 0x34);
		}
	}

	auto& portalMap = ArchipelagoHandler::portalMap; 
	auto& bossMap = ArchipelagoHandler::bossMap;
	auto portalCount = *(int*)(Core::moduleBase + 0x267404);
	auto portalAddr = *(int*)(Core::moduleBase + 0x267408);
	for (auto portalIndex = 0; portalIndex < portalCount; portalIndex++) {
		auto portalDestination = *(int*)(portalAddr + 0xAC);
		if (portalDestination == 4)
			*(int*)(portalAddr + 0xAC) = portalMap[0];
		if (portalDestination == 5)
			*(int*)(portalAddr + 0xAC) = portalMap[1];
		if (portalDestination == 6)
			*(int*)(portalAddr + 0xAC) = portalMap[2];
		if (portalDestination == 7)
			*(int*)(portalAddr + 0xAC) = bossMap[0];
		if (portalDestination == 8)
			*(int*)(portalAddr + 0xAC) = portalMap[3];
		if (portalDestination == 9)
			*(int*)(portalAddr + 0xAC) = portalMap[4];
		if (portalDestination == 10)
			*(int*)(portalAddr + 0xAC) = portalMap[5];
		if (portalDestination == 19)
			*(int*)(portalAddr + 0xAC) = bossMap[1];
		if (portalDestination == 12)
			*(int*)(portalAddr + 0xAC) = portalMap[6];
		if (portalDestination == 13)
			*(int*)(portalAddr + 0xAC) = portalMap[7];
		if (portalDestination == 14)
			*(int*)(portalAddr + 0xAC) = portalMap[8];
		if (portalDestination == 15)
			*(int*)(portalAddr + 0xAC) = bossMap[2];
		portalAddr = *(int*)(portalAddr + 0x34);
	}

	if (SaveDataHandler::saveData.ArchAttributeData.GotSecondRang) {
		auto gateAddr = *(uintptr_t*)(Core::moduleBase + 0x269C14);
		gateAddr = *(uintptr_t*)(gateAddr + 0x78);
		*(bool*)(gateAddr + 0x111) = true;
	}
}

void GameHandler::HandleItemReceived(APClient::NetworkItem item) {
	if (GameState::onLoadScreenOrMainMenu())
		ItemHandler::storedItems.push(item);
	else
		ItemHandler::HandleItem(item);
}

void GameHandler::OnEnterLevel() {	
	*(uintptr_t*)(Core::moduleBase + 0x2888D8) = reinterpret_cast<uintptr_t>(&SaveDataHandler::saveData);
	*(uintptr_t*)(Core::moduleBase + 0x288730) = reinterpret_cast<uintptr_t>(&SaveDataHandler::saveData);

	SaveDataHandler::SaveGame();
	
	if (SaveDataHandler::saveData.ProgressiveRang == 0) 
		SaveDataHandler::saveData.ArchAttributeData.GotBoomerang = false;

	ItemHandler::HandleStoredItems();

	auto levelId = Level::getCurrentLevel();
	if (levelId == LevelCode::Z1)
		OnEnterRainbowCliffs();

	if (*(int*)(Core::moduleBase + 0x27041C) != 0
		&& SaveDataHandler::saveData.StopwatchesActive[(int)levelId] == 1)
		*(int*)(*(int*)(Core::moduleBase + 0x270420) + 0x68) = 0x2;
}

void GameHandler::OnMainMenu() {
	auto menuAddr = *(int*)(Core::moduleBase + 0x286644);
	if (!ArchipelagoHandler::ap_connected) {
		*(char*)(menuAddr + 0x164) = 0x0;
		*(char*)(menuAddr + 0x165) = 0x0;
	}
	*(char*)(menuAddr + 0x168 + 0x164) = 0x0;
	*(char*)(menuAddr + 0x168 + 0x165) = 0x0;
}

void GameHandler::OnLoadGame() {
	SaveDataHandler::LoadSaveData(ArchipelagoHandler::seed);
	*(int*)(Core::moduleBase + 0x27383C) = 0;
	*(int*)(Core::moduleBase + 0x52F2BC) = Core::moduleBase + 0x273844;
	*(int*)(Core::moduleBase + 0x52F2A4) = 0x3;
	*(uintptr_t*)(Core::moduleBase + 0x2888D8) = reinterpret_cast<uintptr_t>(&SaveDataHandler::saveData);
	*(uintptr_t*)(Core::moduleBase + 0x288730) = reinterpret_cast<uintptr_t>(&SaveDataHandler::saveData);
	*(int*)(Core::moduleBase + 0x28DCA0) = 0x9;
}

void GameHandler::OnSpawnpointSet() {
	if (SaveDataHandler::saveData.CurrentLevel != LevelCode::Z1)
		return;
	auto prevLevel = static_cast<int>(SaveDataHandler::saveData.PreviousLevel);
	if (prevLevel == 4 || prevLevel == 5 || prevLevel == 6 ||
		prevLevel == 8 || prevLevel == 9 || prevLevel == 10 ||
		prevLevel == 12 || prevLevel == 13 || prevLevel == 14) {
		auto it = std::find(ArchipelagoHandler::portalMap.begin(), ArchipelagoHandler::portalMap.end(), prevLevel);
		if (it != ArchipelagoHandler::portalMap.end()) {
			auto prevLevelPortalIndex = std::distance(ArchipelagoHandler::portalMap.begin(), it);
			auto pseudoPrevLevel = prevLevelPortalIndex + (4 + (prevLevelPortalIndex > 2) + (prevLevelPortalIndex > 5));
			if (spawnpointMap.find(pseudoPrevLevel) != spawnpointMap.end()) {
				auto& posData = spawnpointMap.at(pseudoPrevLevel);
				Vector3f posVector;
				posVector.x = posData[0];
				posVector.y = posData[1];
				posVector.z = posData[2];
				Hero::setPosition(posVector);
				*(float*)(Core::moduleBase + 0x271C20) = posData[3];
			}
		}
	}
	if (prevLevel == 7 || prevLevel == 19 || prevLevel == 15) {
		auto it = std::find(ArchipelagoHandler::bossMap.begin(), ArchipelagoHandler::bossMap.end(), prevLevel);
		if (it != ArchipelagoHandler::bossMap.end()) {
			auto prevLevelPortalIndex = std::distance(ArchipelagoHandler::bossMap.begin(), it);
			int pseudoPrevLevel;
			if (prevLevelPortalIndex == 0)
				pseudoPrevLevel = 7;
			if (prevLevelPortalIndex == 1)
				pseudoPrevLevel = 19;
			if (prevLevelPortalIndex == 2)
				pseudoPrevLevel = 15;
			if (spawnpointMap.find(pseudoPrevLevel) != spawnpointMap.end()) {
				auto& posData = spawnpointMap.at(pseudoPrevLevel);
				Vector3f posVector;
				posVector.x = posData[0];
				posVector.y = posData[1];
				posVector.z = posData[2];
				Hero::setPosition(posVector);
				*(float*)(Core::moduleBase + 0x271C20) = posData[3];
			}
		}
	}
}

void GameHandler::SetLoadActive(bool value)
{
	if (!GameState::onLoadScreenOrMainMenu())
		return;
	auto menuAddr = *(int*)(Core::moduleBase + 0x286644);
	*(bool*)(menuAddr + 0x164) = value;
}