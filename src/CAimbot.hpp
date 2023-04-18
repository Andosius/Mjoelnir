#pragma once

#include "IThreadedCheat.hpp"

class CAimbot : public IThreadedCheat
{
public:
	using IThreadedCheat::IThreadedCheat;
	virtual void Routine() override;

private:
	bool m_HasTarget = false;
};

