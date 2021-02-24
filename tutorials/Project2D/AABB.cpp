#include "AABB.h"
#include "Gizmos.h"
AABB::AABB(glm::vec2 position, glm::vec2 velocity, float mass, glm::vec2 extents, glm::vec4 colour) : Rigidbody(ShapeType::AABB, position, velocity, 0, mass, colour)
{
	this->extents = extents;
}

AABB::~AABB()
{
}

void AABB::draw()
{
	glm::vec2 topLeft = { -extents.x, extents.y };
	glm::vec2 topRight = { extents.x, extents.y };
	glm::vec2 bottomLeft = { -extents.x, -extents.y };
	glm::vec2 bottomRight = { extents.x, -extents.y };
	aie::Gizmos::add2DTri(topLeft, topRight, bottomLeft, colour, colour, colour);
	aie::Gizmos::add2DTri(bottomLeft, topRight, bottomRight, colour, colour, colour);
}
