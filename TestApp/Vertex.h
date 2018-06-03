#pragma once

/**
*  @brief The vertex struct.
*
*  x,y,z = position
*  nx, ny, nz = normal
*  u, v = texture coordinates
*/
struct Vertex
{
	Vertex() {}

	Vertex(float lx, float ly, float lz, float lnx, float lny, float lnz, float lu, float lv) 
	{
		x = lx; y = ly; z = lz;
		nx = lnx; ny = lny; nz = lnz;
		u = lu; v = lv;
	}

	Vertex(float lx, float ly, float lz, float lu, float lv)
	{
		x = lx; y = ly; z = lz;
		nx = 1.0f; ny = 1.0f; nz = 1.0f;
		u = lu; v = lv;
	}

	float x, y, z;
	float nx, ny, nz;
	float u, v;
};
