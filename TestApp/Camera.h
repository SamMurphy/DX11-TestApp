#pragma once
#include <glm/glm.hpp>
#include "Ray.h"
using namespace glm;

class Camera
{
public:
	Camera()
	{
		lowerLeftCorner = vec3(-2.0f, -1.0f, -1.0f);
		horizontal = vec3(4.0f, 0.0f, 0.0f);
		vertical = vec3(0.0f, 2.0f, 0.0f);
		origin = vec3(0.0f, 0.0f, 0.0f);
	}
	~Camera();

	Ray GetRay(float u, float v) { return Ray(origin, lowerLeftCorner + u * horizontal + v * vertical - origin); }


	vec3 origin;
	vec3 lowerLeftCorner;
	vec3 horizontal;
	vec3 vertical;
};

