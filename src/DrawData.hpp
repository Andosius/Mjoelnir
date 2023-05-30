#pragma once

#include "imgui.h"

enum DrawType {
	Line,
	Circle
};

struct DrawData {
	ImVec2 Start;
	ImVec2 End;
	ImColor Color;
	float Radius;
	DrawType Type;
};
