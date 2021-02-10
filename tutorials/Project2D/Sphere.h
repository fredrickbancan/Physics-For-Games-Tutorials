#pragma once
#include "RigidBody.h"
#include "glm/vec4.hpp"

class Sphere : public Rigidbody
{
public:
	Sphere(glm::vec2 position, glm::vec2 velocity, float mass, float radius, glm::vec4 colour);
	~Sphere();
	virtual void draw();
	float getRadius() { return radius; }
protected:
	float radius;
};
