#include "pch.h"

#include <glm/vec3.hpp>

#include "Utils.hpp"

#define PI 3.141592653f

float GetDistance(glm::vec3 src, glm::vec3 dest)
{
	glm::vec3 tmp = dest - src;
	return sqrt(tmp.x * tmp.x + tmp.y * tmp.y + tmp.z * tmp.z);
}

glm::vec3 CalculateHeadPosition(glm::vec3 src, glm::vec3 dest)
{
	glm::vec3 angles;
	angles.x = -atan2f(dest.x - src.x, dest.y - src.y) / PI * 180.0f + 180.0f;
	angles.y = asinf((dest.z - src.z) / GetDistance(src, dest)) * (180.0f / PI);
	angles.z = 0.0f;

	return angles; 
}

bool TraceLine(uintptr_t baseAddr, ent* localPlayer, glm::vec3 target)
{
	uintptr_t tr_func = baseAddr + TRACE_LINE_FUNC;
	glm::vec3 start = localPlayer->m_HeadPosition;

	traceresult_s result;

	__asm
	{
		push 0; bSkipTags
		push 0; bCheckPlayers
		push localPlayer
		push target.z
		push target.y
		push target.x
		push start.z
		push start.y
		push start.x
		lea eax, [result]
		call tr_func
		add esp, 36
	} // each var takes 4 bytes and they are 9 in total => 36 / 9 = 4

	return !result.collided;
}