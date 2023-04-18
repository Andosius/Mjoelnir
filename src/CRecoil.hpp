#pragma once

#include "ICheat.hpp"

class CRecoil : public ICheat
{
public:
	using ICheat::ICheat;
	virtual void Enable();
	virtual void Disable();

private:
};

