#pragma once

#include "pch.h"

#include <thread>

#include "Game.hpp"

class Game;

class ICheat
{
public:
	ICheat() : m_Client(nullptr), m_Active(false), m_Key(VK_RETURN) {};
	ICheat(Game* client, int key, const char* name) : m_Client(client), m_Active(false), m_Key(key), m_Name(name) {};
	~ICheat() {};

	const char* GetName() { return m_Name; }

	virtual void Routine() {};
	virtual void CheckInteraction() { if (GetAsyncKeyState(m_Key) & 1) Toggle();}
	virtual void Toggle()
	{
		if (m_Active)
		{
			m_Active = false;
			Disable();
		}
		else
		{
			m_Active = true;
			Enable();
		}
	}
	virtual void Enable() {};
	virtual void Disable() {};


protected:
	Game* m_Client = nullptr;
	bool m_Active;
	int m_Key;
	const char* m_Name;
};