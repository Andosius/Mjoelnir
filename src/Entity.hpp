#pragma once

#include "pch.h"

#include <glm/vec3.hpp>

#include "ent.hpp"


class Entity
{
public:
	Entity() {};
	Entity(ent* player) : m_PlayerEntity(player) {};

	glm::vec3 GetHeadPosition();
	glm::vec3 GetVelocity();
	void SetVelocity(glm::vec3 vel);
	glm::vec3 GetPlayerPosition();
	void SetPlayerPosition(glm::vec3 pos);
	glm::vec3 GetPlayerHeadRotation();
	void SetPlayerHeadRotation(glm::vec3 hpos);
	bool IsAiming();
	uint32_t GetPlayerHealth();
	void SetPlayerHealth(uint32_t health);
	uint32_t GetPlayerArmor();
	void SetPlayerArmor(uint32_t armor);
	bool GetPlayerAttacking();
	void SetPlayerAttacking(bool state);
	char* GetPlayerUsername();
	void SetPlayerUsername(const char* name);
	int8_t GetTeam();
	uint32_t GetState();
	weapon* GetWeapon();
	bool IsAlive();

	bool Compare(ent* entity);

	uintptr_t GetEntityAddress();
	ent* GetOriginal() { return m_PlayerEntity; }

private:
	ent* m_PlayerEntity = nullptr;
	float m_Height = 5.5f;
};