#include "Sphere.h"
#include "Gizmos.h"
#include "glm/ext.hpp"
Sphere::Sphere(glm::vec2 position, glm::vec2 velocity, float mass, float radius, glm::vec4 colour) : Rigidbody(ShapeType::SPHERE, position, velocity, 0, mass, colour)
{
	this->radius = radius;
}

Sphere::~Sphere()
{

}

void Sphere::draw()
{
	glm::vec2 end = glm::vec2(std::cos(orientation), std::sin(orientation)) * radius;
	aie::Gizmos::add2DCircle(position, radius, 32, colour);
	aie::Gizmos::add2DLine(position, position + end, glm::vec4(1, 1, 1, 1));
}        
