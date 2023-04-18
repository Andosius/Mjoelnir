#pragma once

#include <thread>

#include "ICheat.hpp"

class IThreadedCheat : public ICheat
{
public:
	using ICheat::ICheat;
	virtual void Routine() {};
	virtual void Enable() override { p_Thread = std::thread(&IThreadedCheat::Routine, this); };
	virtual void Disable() override { m_Active = false; p_Thread.detach(); };

	std::thread* GetThread() { return &p_Thread; };

private:
	std::thread p_Thread;
};

