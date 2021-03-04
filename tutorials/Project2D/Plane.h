#pragma once
#include "PhysicsObject.h"
#include "glm/vec4.hpp"
class Plane : public PhysicsObject
{
public:
	Plane(glm::vec2 normal, float distance, glm::vec4 colour);
	~Plane();
	virtual void fixedUpdate(glm::vec2 gravity, float timeStep);
	virtual void draw(float ptnt);
	virtual void resetPosition();
	glm::vec2 getNormal() { return normal; }
	float getOffset() { return offset; }
	void resolveCollision(class Rigidbody* other);
protected:
	glm::vec2 normal;
	float offset;
};