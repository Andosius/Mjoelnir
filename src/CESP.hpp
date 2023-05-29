#pragma once

#include "IThreadedCheat.hpp"
#include "IDrawing.hpp"

class CESP : public IThreadedCheat, public IDrawing
{
	using IThreadedCheat::IThreadedCheat;
	virtual void Routine() override;

	using IThreadedCheat::Enable;
	using IThreadedCheat::Disable;
	using IThreadedCheat::Toggle;
	using IThreadedCheat::CheckInteraction;

};