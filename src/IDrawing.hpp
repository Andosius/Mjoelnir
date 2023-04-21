#pragma once

#include <Windows.h>

#include <iostream>


struct Vertex {
	float X;
	float Y;
	float Z = 0.0f;
	float RHW = 1.0f;
};

struct Vertices {
	std::vector<Vertex> Vertices;
	DWORD Color;
	bool Filled = false;
};

class IDrawing
{
public:
	IDrawing() = default;
	void ResetVertices() { m_Vertices.clear(); }

protected:
	std::vector<Vertices> m_Vertices;
	std::string m_Key = "";
};

