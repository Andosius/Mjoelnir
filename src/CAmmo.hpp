#pragma once

#include "ICheat.hpp"

class CAmmo : public ICheat
{
public:
	using ICheat::ICheat;
	virtual void Enable();
	virtual void Disable();

private:
};

