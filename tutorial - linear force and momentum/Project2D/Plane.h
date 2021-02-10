#pragma once
#include "PhysicsObject.h"

class Plane : public PhysicsObject
{
public:
	Plane(glm::vec2 normal, float offset);
	~Plane();

protected:
	glm::vec2 normal;
	float offset;
};