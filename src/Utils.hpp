#pragma once

#include <glm/vec3.hpp>

#include "ent.hpp"


struct traceresult_s
{
	glm::vec3 end{ 0,0,0 };
	bool collided{ false };
};

float GetDistance(const glm::vec3 src, const glm::vec3 dest);
glm::vec3 CalculateHeadPosition(glm::vec3 src, glm::vec3 dest);
bool TraceLine(uintptr_t baseAddr, ent* localPlayer, glm::vec3 target, int playerCheck = false, int skipObjects = false);

bool OpenGL_WorldToScreen(glm::vec3 pos, glm::mat4x4* viewMatrix, glm::vec2& screen, glm::vec2 windowData);

HWND GetWindowHandle(DWORD processID, const wchar_t* overlayedWindowClassName);

std::string generate_uuid();
