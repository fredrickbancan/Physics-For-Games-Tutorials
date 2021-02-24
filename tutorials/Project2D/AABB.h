#pragma once
#include "RigidBody.h"
#include "glm/vec4.hpp"

class AABB : public Rigidbody
{
public:
	AABB(glm::vec2 position, glm::vec2 velocity, float mass, glm::vec2 extents, glm::vec4 colour);
	~AABB();
	virtual void draw();
protected:
	glm::vec2 extents;//x extent and y extent from center of aabb
};
