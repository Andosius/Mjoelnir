#pragma once

#include "ICheat.hpp"
#include "IDrawing.hpp"

class CESP : public ICheat, public IDrawing
{
	using ICheat::ICheat;
	virtual void Routine() override;

	using ICheat::Enable;
	using ICheat::Disable;
	using ICheat::Toggle;
	using ICheat::CheckInteraction;
	using ICheat::IsActive;

};