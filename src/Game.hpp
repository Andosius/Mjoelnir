#pragma once

#include "pch.h"

#include <memory>
#include <vector>

#include <mutex>

#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>


#include "Entity.hpp"
#include "ICheat.hpp"

class Game
{
public:
	Game() :
		m_BaseAddr(NULL), m_Active(false), m_LocalPlayer(nullptr), m_Matrix(nullptr), m_Window(NULL), m_WindowData(0.0f, 0.0f) {};
	~Game() { Cleanup(); };
	void Initialize();
	void Cleanup();
	void Routine();
	bool IsRunning() { return m_Active; };

	uintptr_t GetBaseAddr() { return m_BaseAddr; };
	Entity* GetLocalPlayer() { return m_LocalPlayer; };
	glm::mat4x4* GetViewMatrix() { return m_Matrix; };
	uint32_t GetPlayerCount();
	std::vector<Entity>* GetPlayers() { return &m_Players; };

	std::mutex* GetMutex() { return &p_Mutex; };

	void AddCheat(ICheat* cheat) { m_Cheats.push_back(cheat); };
	std::vector<ICheat*>* GetCheats() { return &m_Cheats; }
	

	glm::vec2 GetWindowData() { return m_WindowData; };

private:
	void UpdateGameInformation();

	bool m_Active;
	uintptr_t m_BaseAddr;
	Entity* m_LocalPlayer;

	glm::mat4x4* m_Matrix;

	std::vector<ICheat*> m_Cheats;
	std::vector<Entity> m_Players;

	std::mutex p_Mutex;
	std::thread p_EntityUpdater;

	HWND m_Window;
	glm::vec2 m_WindowData;
};

