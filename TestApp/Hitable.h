#pragma once
#include "Ray.h"
#include <glm/glm.hpp>

using namespace glm;

struct HitRecord
{
	float t;
	vec3 p;
	vec3 normal;
};

class Hitable
{
public:
	Hitable() {}
	virtual bool hit(const Ray& r, float tMin, float tMax, HitRecord& hitrecord) const = 0;
};

class Sphere : public Hitable
{
public:
	Sphere() : Hitable() {}
	Sphere(vec3 cen, float r) : center(cen), radius(r), Hitable() {}
	virtual bool hit(const Ray& r, float tMin, float tMax, HitRecord& hitrecord) const override;

	vec3 center;
	float radius;
};

class HitableList : public Hitable
{
public:
	HitableList() {}
	HitableList(Hitable** l, int n) { list = l; listSize = n; }
	virtual bool hit(const Ray& r, float tMin, float tMax, HitRecord& hitrecord) const override;

	Hitable** list;
	int listSize;
};

