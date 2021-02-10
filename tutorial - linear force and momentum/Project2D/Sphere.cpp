#include "Sphere.h"
#include "Gizmos.h"
Sphere::Sphere(glm::vec2 position, glm::vec2 velocity, float mass, float radius, glm::vec4 colour) : Rigidbody(ShapeType::SPHERE, position, velocity, 0, mass)
{
	this->radius = radius;
	this->colour = colour;
}

Sphere::~Sphere()
{

}

void Sphere::draw()
{
	int segemnts = 32;
	aie::Gizmos::add2DCircle(position, radius, segemnts, colour);
}        
