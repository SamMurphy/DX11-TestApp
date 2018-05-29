#include "Hitable.h"

bool Sphere::hit(const Ray & r, float tMin, float tMax, HitRecord & hitrecord) const
{
	vec3 oc = r.origin() - center;
	float a = dot(r.direction(), r.direction());
	float b = dot(oc, r.direction());
	float c = dot(oc, oc) - radius * radius;
	float discriminant = b*b - a*c;
	if (discriminant > 0)
	{
		float temp = (-b - glm::sqrt(b*b - a * c)) / a;
		if (temp < tMax && temp > tMin)
		{
			hitrecord.t = temp;
			hitrecord.p = r.point_at_param(temp);
			hitrecord.normal = (hitrecord.p - center) / radius;
			return true;
		}
		temp = (-b + glm::sqrt(b*b - a * c)) / a;
		if (temp < tMax && temp > tMin)
		{
			hitrecord.t = temp;
			hitrecord.p = r.point_at_param(temp);
			hitrecord.normal = (hitrecord.p - center) / radius;
			return true;
		}
	}
	return false;
}

bool HitableList::hit(const Ray & r, float tMin, float tMax, HitRecord & hitrecord) const
{
	HitRecord tempRec;
	bool hitAnything = false;
	float closestSoFar = tMax;

	for (int i = 0; i < listSize; i++)
	{
		if (list[i]->hit(r, tMin, closestSoFar, tempRec))
		{
			hitAnything = true;
			closestSoFar = tempRec.t;
			hitrecord = tempRec;
		}
	}
	return hitAnything;
}
