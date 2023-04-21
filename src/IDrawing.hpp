#pragma once

#include <Windows.h>

#include <iostream>

struct Object;

class IDrawing
{
public:
	IDrawing() = default;
	void ResetVertices() { m_Objects.clear(); }

protected:
	std::vector<Object> m_Objects;
	std::string m_Key = "";
};


struct Vertex {
	float X;
	float Y;
	float Z;
	float RHW = 0;
	DWORD Color;
};

enum DRAWING_TYPE {
	DRAWING_TYPE_RECT = 1,
	DRAWING_TYPE_FILLED_RECT = 2,
	DRAWING_TYPE_TRIANGLE = 3
};

struct Object {
	std::vector<Vertex> Vertices;
	DWORD Color;
	DRAWING_TYPE Type;
};