#pragma once

#include "pch.h"


class weaponinfo
{
public:
	char FILLER_1[288];
	uint16_t w_RecoilSeed[2];
};

class ent;

class weapon
{
public:
	char FILLER_1[4];
	uint32_t w_ID; //0x0004
	ent* w_Owner; //0x0008
	weaponinfo* w_Info; //0x000C
	int* w_SpareAmmo; //0x0010
	int* w_CurrentAmmo; //0x0014
};


class ent
{
public:	
	char FILLER_1[4];
	glm::vec3 m_HeadPosition; //0x0004
	glm::vec3 m_Velocity; //0x0010
	char FILLER_2[24];
	glm::vec3 m_Position; //0x0034
	glm::vec3 m_HeadAngle; //0x0040
	char FILLER_3[37];
	bool m_IsAiming; //0x0071
	char FILLER_4[134];
	uint32_t m_Health; //0x00F8
	uint32_t m_Armor; //0x00FC
	char FILLER_5[292];
	bool m_Attacking;
	char m_Username[16]; //0x0225
	char FILLER_6[247];
	int8_t m_Team; //0x032C
	char FILLER_7[11];
	uint32_t m_State; //0x0338
	char FILLER_8[56];
	weapon* m_Weapon; //0x0374
	char FILLER_9[520];
	// WORKING!
	/*
	char FILLER_1[4];
	vec3 m_HeadPosition; //0x0004
	vec3 m_Velocity; //0x0010
	char FILLER_2[24];
	vec3 m_Position; //0x0034
	vec3 m_HeadAngle; //0x0040
	char FILLER_3[37];
	BYTE m_IsAiming; //0x0071
	char FILLER_4[134];
	__int32 m_Health; //0x00F8
	__int32 m_Armor; //0x00FC
	char FILLER_5[292];
	BYTE m_Attacking;
	char m_Username[16]; //0x0225
	char FILLER_6[247];
	BYTE m_Team; //0x032C
	char FILLER_7[11];
	BYTE m_State; //0x0338
	char FILLER_8[59];
	weapon* m_Weapon; //0x0374
	*/
};