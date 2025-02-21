#include "LocationHandler.h"

void LocationHandler::HandleLocation(int64_t location)
{
	if (GameState::onMainMenu()) {
		storedCheckedLocations.push(location);
		return;
	}

	auto currentLevel = Level::getCurrentLevel();

	if (location >= 0x8750100 && location < 0x8750148) {
		// THUNDER EGG
		int theggId = location - 0x8750100;
		auto levelIndex = theggId / 8;
		auto levelId = levelIndex + 4 + (levelIndex > 2) + (levelIndex > 5);
		auto theggIndex = theggId % 8;
		SaveDataHandler::saveData.LevelData[levelId].ThunderEggs[theggIndex] = true;
		SaveDataHandler::SaveGame();
		if ((LevelCode)levelId != currentLevel)
			return;
		auto addr = *(uintptr_t*)(Core::moduleBase + 0x270238 + 0x48);
		for (int i = 0; i < 8; i++) {
			auto currentTheggIndex = *(int*)(addr + 0x6C);
			if (theggIndex == currentTheggIndex) {
				*(bool*)(addr + 0x66) = true;
				return;
			}
			addr = *(uintptr_t*)(addr + 0x34);
		}
	}
	if (location >= 0x8750148 && location < 0x87501A2) {
		// GOLDEN COG
		int cogId = location - 0x8750148;
		auto levelIndex = cogId / 10;
		auto levelId = levelIndex + 4 + (levelIndex > 2) + (levelIndex > 5);
		auto cogIndex = cogId % 10;
		SaveDataHandler::saveData.LevelData[levelId].GoldenCogs[cogIndex] = true;
		SaveDataHandler::SaveGame();
		if ((LevelCode)levelId != currentLevel)
			return;
		auto addr = *(uintptr_t*)(Core::moduleBase + 0x2702C8 + 0x48);
		for (int i = 0; i < 10; i++) {
			auto currentCogIndex = *(int*)(addr + 0x6C);
			if (cogIndex == currentCogIndex) {
				*(int*)(addr + 0xC4) = 5;
				return;
			}
			addr = *(uintptr_t*)(addr + 0x34);
		}
	}
	if (location >= 0x87501AC && location < 0x87501D9) {
		// BILBY
		int bilbyId = location - 0x87501AC;
		auto levelIndex = bilbyId / 5;
		auto levelId = levelIndex + 4 + (levelIndex > 2) + (levelIndex > 5);
		auto bilbyIndex = bilbyId % 5;
		auto bilbyWrite = std::count(SaveDataHandler::saveData.LevelData[levelId].Bilbies,
			SaveDataHandler::saveData.LevelData[levelId].Bilbies + 0x5, 1) == 4 ? 3 : 1;
		SaveDataHandler::saveData.LevelData[levelId].Bilbies[bilbyIndex] = bilbyWrite;
		SaveDataHandler::SaveGame();
		if ((LevelCode)levelId != currentLevel)
			return;
		if (bilbyWrite == 3)
			return;
		auto addr = *(uintptr_t*)(Core::moduleBase + 0x27D608);
		addr = *(uintptr_t*)(addr);
		for (int i = 0; i < 5; i++) {
			auto currentBilbyIndex = *(int*)(addr);
			if (currentBilbyIndex == bilbyIndex) {
				*(int*)(addr + 0x34) = 0;
				*(int*)(addr + 0x58) = 0;
				return;
			}
			addr += 0x134;
		}
	}
	if (location >= 0x87501D9 && location < 0x8750258) {
		// PF
		int frameId = location - 0x87501D9;
		auto byteIndex = frameId / 8;
		auto bitIndex = frameId % 8;
		auto b = SaveDataHandler::saveData.PictureFrames[byteIndex];
		b |= 1 << bitIndex;
		SaveDataHandler::saveData.PictureFrames[byteIndex] = b;
		SaveDataHandler::SaveGame();
		for (auto it = RUNNING_FRAME_COUNTS.begin(); it != RUNNING_FRAME_COUNTS.end(); ++it) {
			int start = it->second;
			int end = (std::next(it) != RUNNING_FRAME_COUNTS.end()) ? std::next(it)->second : INT_MAX;
			LevelCode expectedLevel;
			switch (it->first) {
			case 0: expectedLevel = LevelCode::Z1; break;
			case 4: expectedLevel = LevelCode::A1; break;
			case 5: expectedLevel = LevelCode::A2; break;
			case 6: expectedLevel = LevelCode::A3; break;
			case 8: expectedLevel = LevelCode::B1; break;
			case 9: expectedLevel = LevelCode::B2; break;
			case 12: expectedLevel = LevelCode::C1; break;
			case 13: expectedLevel = LevelCode::C2; break;
			case 14: expectedLevel = LevelCode::C3; break;
			default: return;  // In case of unexpected keys
			}

			if (frameId >= start && frameId < end && currentLevel == expectedLevel) {
				auto frameIndex = frameId - start;
				auto frameCount = *(int*)(Core::moduleBase + 0x254D68 + 0x44);
				auto addr = *(uintptr_t*)(Core::moduleBase + 0x254D68 + 0x48);
				for (int i = 0; i < frameCount; i++) {
					if (frameIndex == i) {
						*(int*)(addr + 0x48) = 0;
						*(int*)(addr + 0x114) = 2;
						return;
					}
					addr = *(uintptr_t*)(addr + 0x34);
				}
				return;
			}
		}
		return;
	}
	if (location >= 0x8750320 && location < 0x8750339) {
		// RAINBOW SCALES
		int scaleId = location - 0x8750320;
		auto byteIndex = scaleId / 8;
		auto bitIndex = scaleId % 8;
		auto b = SaveDataHandler::saveData.LevelData[(int)LevelCode::Z1].Opals[byteIndex];
		b |= 1 << bitIndex;
		SaveDataHandler::saveData.LevelData[(int)LevelCode::Z1].Opals[byteIndex] = b;
		SaveDataHandler::SaveGame();
		if (currentLevel != LevelCode::Z1)
			return;
		auto addr = *(uintptr_t*)(Core::moduleBase + 0x269818 + 0x48);
		for (int i = 0; i < 25; i++) {
			if (scaleId == i) {
				*(int*)(addr + 0x78) = 5;
				return;
			}
			addr = *(uintptr_t*)(addr + 0x34);
		}
	}
	if (location >= 0x8750261 && location < 0x8750266) {
		// TALISMAN
		int talismanId = location - 0x8750261;
		SaveDataHandler::saveData.Talismans[talismanId] = true;
		SaveDataHandler::SaveGame();
	}
	if (location >= 0x8750310 && location < 0x8750320) {
		// ATTRIBUTE
		int attributeId = location - 0x8750310;
		switch (attributeId) {
		case 0:
			SaveDataHandler::saveData.AttributeData.LearntToSwim = true;
			break;
		case 1:
			SaveDataHandler::saveData.AttributeData.LearntToDive = true;
			break;
		case 2:
			SaveDataHandler::saveData.AttributeData.GotSecondRang = true;
			break;
		case 3:
			SaveDataHandler::saveData.AttributeData.GotExtraHealth = true;
			break;
		case 4:
			SaveDataHandler::saveData.AttributeData.GotBoomerang = true;
			break;
		case 5:
			SaveDataHandler::saveData.AttributeData.GotFrostyrang = true;
			break;
		case 6:
			SaveDataHandler::saveData.AttributeData.GotFlamerang = true;
			break;
		case 7:
			SaveDataHandler::saveData.AttributeData.GotKaboomerang = true;
			break;
		case 8:
			SaveDataHandler::saveData.AttributeData.GotDoomerang = true;
			break;
		case 9:
			SaveDataHandler::saveData.AttributeData.GotMegarang = true;
			break;
		case 10:
			SaveDataHandler::saveData.AttributeData.GotZoomerang = true;
			break;
		case 11:
			SaveDataHandler::saveData.AttributeData.GotInfrarang = true;
			break;
		case 12:
			SaveDataHandler::saveData.AttributeData.GotZappyrang = true;
			break;
		case 13:
			SaveDataHandler::saveData.AttributeData.GotAquarang = true;
			break;
		case 14:
			SaveDataHandler::saveData.AttributeData.GotMultirang = true;
			break;
		case 15:
			SaveDataHandler::saveData.AttributeData.GotChronorang = true;
			break;
		}
		SaveDataHandler::SaveGame();
	}
	
}

void LocationHandler::HandleStoredCheckedLocations()
{
	while (!storedCheckedLocations.empty() && !GameState::onMainMenu()) {
		HandleLocation(storedCheckedLocations.front());
		storedCheckedLocations.pop();
	}
}
