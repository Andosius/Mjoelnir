#pragma once

#include "ICheat.hpp"

class CHealth : public ICheat
{
public:
	using ICheat::ICheat;

	using ICheat::Toggle;
	using ICheat::CheckInteraction;

	void Enable() override;
	void Disable() override;
};

