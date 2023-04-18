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
bool TraceLine(uintptr_t baseAddr, ent* localPlayer, glm::vec3 target);