#include "pch.h"

#include <iostream>
#include <memory>

#include <glm/mat4x4.hpp>

#include "Constants.hpp"

#include "Game.hpp"
#include "Entity.hpp"


void Game::Initialize()
{
	m_Active = true;

	m_BaseAddr = (uintptr_t)GetModuleHandle(L"ac_client.exe");


	if (m_BaseAddr != NULL)
	{

		ent* playerent = *(ent**)(m_BaseAddr + LOCAL_PLAYER_ADDRESS);
		m_LocalPlayer = new Entity(playerent);

		m_Matrix = *(glm::mat4x4**)(m_BaseAddr + OPENGL_MATRIX_ADDRESS);
	}
}

void Game::Cleanup()
{


	for (auto& cheat : m_Cheats)
	{
		cheat->Disable();
		delete cheat;
	}

	delete m_LocalPlayer;
}

void Game::Routine()
{
	/*
	* So far our only cheat involving a Routine is the Aimbot,
	* the ESP is about to follow. For synchronization reasons
	* I'm going to implement std::thread p_Thread to ICheat so
	* they can access the Player-List via Game-Mutex. That should
	* prevent crashes and flickering - atleast I hope so, lol.
	*/

	p_Mutex.lock();

	UpdateEntityList();
	for (auto& cheat : m_Cheats)
	{
		cheat->CheckInteraction();
	}

	p_Mutex.unlock();
}

uint32_t Game::GetPlayerCount()
{
	return *(uint32_t*)(m_BaseAddr + PLAYER_COUNT_ADDRESS);
}

void Game::UpdateEntityList()
{
	m_Players.clear();

	uint32_t player_count = GetPlayerCount();

	uint32_t* player_list = (uint32_t*)(m_BaseAddr + PLAYER_LIST_ADDRESS);

	for (uint32_t i = 0; i < player_count; i++)
	{

		ent* playerent = *(ent**)(*player_list + (i * 0x4));
		if (playerent != nullptr)
		{
			m_Players.push_back(Entity(playerent));
		}
	}
}
