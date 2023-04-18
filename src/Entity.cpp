#include "pch.h"

#include "Entity.hpp"


glm::vec3 Entity::GetHeadPosition()
{
	return m_PlayerEntity->m_HeadPosition;
}

glm::vec3 Entity::GetVelocity()
{
	return m_PlayerEntity->m_Velocity;
}

void Entity::SetVelocity(glm::vec3 vel)
{
	m_PlayerEntity->m_Velocity = vel;
}

glm::vec3 Entity::GetPlayerPosition()
{
	return m_PlayerEntity->m_Position;
}

void Entity::SetPlayerPosition(glm::vec3 pos)
{
	m_PlayerEntity->m_Position = pos;
}

glm::vec3 Entity::GetPlayerHeadRotation()
{
	return m_PlayerEntity->m_HeadAngle;
}

void Entity::SetPlayerHeadRotation(glm::vec3 hpos)
{
	m_PlayerEntity->m_HeadAngle = hpos;
}

bool Entity::IsAiming()
{
	return m_PlayerEntity->m_IsAiming;
}

uint32_t Entity::GetPlayerHealth()
{
	return m_PlayerEntity->m_Health;
}

void Entity::SetPlayerHealth(uint32_t health)
{
	m_PlayerEntity->m_Health = health;
}

uint32_t Entity::GetPlayerArmor()
{
	return m_PlayerEntity->m_Armor;
}

void Entity::SetPlayerArmor(uint32_t armor)
{
	m_PlayerEntity->m_Armor = armor;
}

bool Entity::GetPlayerAttacking()
{
	return m_PlayerEntity->m_Attacking;
}

void Entity::SetPlayerAttacking(bool state)
{
	m_PlayerEntity->m_Attacking = state;
}

char* Entity::GetPlayerUsername()
{
	return m_PlayerEntity->m_Username;
}

void Entity::SetPlayerUsername(const char* name)
{
	strcpy_s(m_PlayerEntity->m_Username, name);
}

int8_t Entity::GetTeam()
{
	return m_PlayerEntity->m_Team;
}

uint32_t Entity::GetState()
{
	return m_PlayerEntity->m_State;
}

weapon* Entity::GetWeapon()
{
	return m_PlayerEntity->m_Weapon;
}

bool Entity::IsAlive()
{
	return m_PlayerEntity->m_State == 0;
}

bool Entity::Compare(ent* entity)
{
	return &m_PlayerEntity == &entity;
}

uintptr_t Entity::GetEntityAddress()
{
	return *(uintptr_t*)m_PlayerEntity;
}