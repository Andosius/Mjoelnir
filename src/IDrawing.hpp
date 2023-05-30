#pragma once

#include <Windows.h>

#include <vector>

#include "DrawData.hpp"


class IDrawing
{
public:
	IDrawing() = default;
	~IDrawing() = default;

	std::vector<DrawData>* GetDrawData() { return &m_DrawData; }

protected:
	std::vector<DrawData> m_DrawData;
};