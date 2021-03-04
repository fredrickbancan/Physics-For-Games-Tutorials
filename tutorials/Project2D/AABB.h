#pragma once
#include "RigidBody.h"
#include "glm/vec4.hpp"

class AABB : public Rigidbody
{
public:
	AABB(glm::vec2 position, glm::vec2 velocity, float mass, glm::vec2 extents, glm::vec4 colour);
	~AABB();
	virtual void draw(float ptnt);
	glm::vec2 clampToBox(glm::vec2 vec);
	glm::vec2 getExtents() { return extents; }
	float minX() { return position.x - extents.x; }
	float maxX() { return position.x + extents.x; }
	float minY() { return position.y - extents.y; }
	float maxY() { return position.y + extents.y; }
protected:
	glm::vec2 extents;//x extent and y extent from center of aabb
};
