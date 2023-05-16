#pragma once

#include "pch.h"


class weaponinfo
{
public:
	char FILLER_1[288];
	uint16_t w_RecoilSeed[2]; // Offset: 288 (0x0120)
};

class ent;

class weapon
{
public:
	char FILLER_1[4];
	uint32_t w_ID; // Offset: 4 (0x0004)
	ent* w_Owner; // Offset: 8 (0x0008)
	weaponinfo* w_Info; // Offset: 12 (0x000C)
	int* w_SpareAmmo; // Offset: 16 (0x0010)
	int* w_CurrentAmmo; // Offset: 20 (0x0014)
};


class ent
{
public:
	char FILLER_1[4];
	glm::vec3 m_HeadPosition; // Offset: 4 (0x0004)
	glm::vec3 m_Velocity; // Offset: 16 (0x0010)
	char FILLER_2[24];
	glm::vec3 m_Position; // Offset: 52 (0x0034)
	glm::vec3 m_HeadAngle; // Offset: 64 (0x0040)
	char FILLER_3[37];
	bool m_IsAiming; // Offset: 113 (0x0071)
	char FILLER_4[134];
	uint32_t m_Health; // Offset: 248 (0x00F8)
	uint32_t m_Armor; // Offset: 252 (0x00FC)
	char FILLER_5[292];
	bool m_Attacking; // Offset: 548 (0x0224)
	char m_Username[16]; // Offset: 565 (0x0235)
	char FILLER_6[247];
	int8_t m_Team; // Offset: 820 (0x0334)
	char FILLER_7[11];
	uint32_t m_State; // Offset: 840 (0x0348)
	char FILLER_8[56];
	weapon* m_Weapon; // Offset: 884 (0x0374)
	char FILLER_9[520];
};
