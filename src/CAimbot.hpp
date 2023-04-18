#pragma once

#include "IThreadedCheat.hpp"

class CAimbot : public IThreadedCheat
{
public:
	using IThreadedCheat::IThreadedCheat;
	virtual void Routine() override;

	using IThreadedCheat::Enable;
	using IThreadedCheat::Disable;
	using IThreadedCheat::Toggle;
	using IThreadedCheat::CheckInteraction;


private:
	bool m_HasTarget = false;
};

