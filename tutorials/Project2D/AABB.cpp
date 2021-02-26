#include "AABB.h"
#include "Gizmos.h"
#include "glm/ext.hpp"
AABB::AABB(glm::vec2 position, glm::vec2 velocity, float mass, glm::vec2 extents, glm::vec4 colour) : Rigidbody(ShapeType::AABB, position, velocity, 0, mass, colour)
{
	this->extents = extents;
}

AABB::~AABB()
{
}

void AABB::draw()
{
	glm::vec2 topLeft = { (-extents.x) + position.x, extents.y + position.y};
	glm::vec2 topRight = { extents.x + position.x, extents.y + position.y };
	glm::vec2 bottomLeft = { (-extents.x) + position.x, (-extents.y) + position.y };
	glm::vec2 bottomRight = { extents.x + position.x, (-extents.y) + position.y };
	aie::Gizmos::add2DTri(topRight, topLeft, bottomLeft, colour, colour, colour);
	aie::Gizmos::add2DTri(bottomLeft, bottomRight, topRight, colour, colour, colour);
}

glm::vec2 AABB::clampToBox(glm::vec2 vec)
{
	return glm::vec2{ glm::clamp(vec.x, position.x - extents.x, position.x + extents.x), glm::clamp(vec.y, position.y - extents.y, position.y + extents.y) };
}
