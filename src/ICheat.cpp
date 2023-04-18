#include "pch.h"

void ICheat::Toggle()
{
	if (m_Active)
	{
		m_Active = false;
		Disable();
	}
	else
	{
		m_Active = true;
		Enable();
	}
}

void ICheat::CheckInteraction()
{
	if (GetAsyncKeyState(m_Key) & 1)
		Toggle();
}