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

		m_Matrix = (glm::mat4x4*)(m_BaseAddr + OPENGL_MATRIX_ADDRESS);
	}

	m_Window = FindWindow(nullptr, L"AssaultCube");

	p_EntityUpdater = std::thread(&Game::UpdateGameInformation, this);
}

void Game::Cleanup()
{
	//std::cout << "Disabling cheats!" << std::endl;
	for (auto& cheat : m_Cheats)
	{
		//std::cout << "[EOF] " << cheat->GetName() << " gets deleted!" << std::endl;
		cheat->Disable();
		delete cheat;
	}

	//std::cout << "Deleting m_LocalPlayer!" << std::endl;

	delete m_LocalPlayer;

	try
	{
		m_Active = false;
		if (p_EntityUpdater.joinable())
			p_EntityUpdater.detach();
	}
	catch (std::system_error* e) {}
}

void Game::Routine()
{
	//std::cout << "Game routine!" << std::endl;

	for (auto& cheat : m_Cheats)
	{
		cheat->CheckInteraction();
	}
}

uint32_t Game::GetPlayerCount()
{
	return *(uint32_t*)(m_BaseAddr + PLAYER_COUNT_ADDRESS);
}

void Game::UpdateGameInformation()
{

	while (m_Active)
	{
		//std::cout << "Update Entity List!" << std::endl;
		p_Mutex.lock();

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
		p_Mutex.unlock();

	}

	RECT rect;
	if (GetClientRect(m_Window, &rect))
	{
		m_WindowData.x = rect.right - rect.left;
		m_WindowData.y = rect.bottom - rect.top;
	}

}
