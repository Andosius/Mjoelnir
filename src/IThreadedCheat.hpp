#pragma once

#include <thread>

#include "ICheat.hpp"

class IThreadedCheat : public ICheat
{
public:
	using ICheat::ICheat;
	using ICheat::CheckInteraction;
	using ICheat::Toggle;

	virtual void Routine() {};

	virtual void Enable() override { p_Thread = std::thread(&IThreadedCheat::Routine, this); };
	virtual void Disable() override {
		try
		{
			m_Active = false;
			if (p_Thread.joinable())
				p_Thread.detach();
		}
		catch (std::system_error* e) { }
	};

	std::thread* GetThread() { return &p_Thread; };

private:
	std::thread p_Thread;
};

