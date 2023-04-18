#pragma once

#include "pch.h"

#include <thread>

#include "Game.hpp"

class Game;

class ICheat
{
public:
	ICheat() : m_Client(nullptr), m_Active(false), m_Key(VK_RETURN) {};
	ICheat(Game* client, int key) : m_Client(client), m_Active(false), m_Key(key) {};
	~ICheat() {};
	virtual void Routine() {};
	virtual void CheckInteraction();
	virtual void Toggle();
	virtual void Enable() {};
	virtual void Disable() {};
	std::thread* GetThread() { return &p_Thread; };


protected:
	Game* m_Client = nullptr;
	bool m_Active;
	int m_Key;
	std::thread p_Thread;
};