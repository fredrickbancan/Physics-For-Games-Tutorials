#include "Plane.h"

Plane::Plane(glm::vec2 normal, float offset) : PhysicsObject(ShapeType::PLANE)
{
	this->normal = normal;
	this->offset = offset;
}

Plane::~Plane()
{
}
