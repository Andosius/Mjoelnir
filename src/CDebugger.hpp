#pragma once

#include "ICheat.hpp"

class CDebugger : public ICheat
{
public:
	using ICheat::ICheat;
	virtual void Routine();
};

