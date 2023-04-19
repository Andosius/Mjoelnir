#include "pch.h"
#include "Window.hpp"

Window::Window(HMODULE hModule, const char* gameWindowClassName, const char* windowClassName, const char* m_WindowName)
{
	this->m_HModule = hModule;
	this->m_GameWindowClassName = gameWindowClassName;
	this->m_WindowClassName = windowClassName;
	this->m_WindowName = m_WindowName;
}