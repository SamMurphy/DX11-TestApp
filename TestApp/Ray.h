#pragma once
#include <glm/glm.hpp>

using namespace glm;

class Ray
{
public:
	Ray() {}
	Ray(const vec3& a, const vec3& b) { A = a; B = b; }
	~Ray();

	vec3 origin() const { return A; }
	vec3 direction() const { return B; }
	vec3 point_at_param(float t) const { return A + t * B; }

	vec3 A;
	vec3 B;
};

