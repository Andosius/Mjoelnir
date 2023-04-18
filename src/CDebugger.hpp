#pragma once

#include "ICheat.hpp"

class CDebugger : public ICheat
{
public:
	using ICheat::ICheat;
	virtual void CheckInteraction() override;

	using ICheat::Toggle;
	using ICheat::CheckInteraction;
	using ICheat::Enable;
	using ICheat::Disable;
};

