#include "Plane.h"
#include "glm/vec2.hpp"
#include "glm/ext.hpp"
#include "Gizmos.h"
#include "RigidBody.h"

using namespace glm;

Plane::Plane(glm::vec2 normal, float offset, glm::vec4 colour) : PhysicsObject(ShapeType::PLANE, colour)
{
	this->normal = normal;
	this->offset = offset;
}

Plane::~Plane()
{
}

void Plane::fixedUpdate(glm::vec2 gravity, float timeStep)
{
}

void Plane::draw()
{
	float lineSegmentLength = 300;
	vec2 centerPoint = normal * offset;
	// easy to rotate normal through 90 degrees around z
	vec2 parallel(normal.y, -normal.x);
	vec4 colourFade = colour;
	colourFade.a = 0;
	vec2 start = centerPoint + (parallel * lineSegmentLength);
	vec2 end = centerPoint - (parallel * lineSegmentLength);
	//aie::Gizmos::add2DLine(start, end, colour);
	aie::Gizmos::add2DTri(start, end, start - normal * 10.0f, colour, colour, colourFade);
	aie::Gizmos::add2DTri(end, end - normal * 10.0f, start - normal * 10.0f, colour, colourFade, colourFade);
}

void Plane::resolveCollision(Rigidbody* other, glm::vec2 contactPos)
{
	float elasticity = 1;//TODO: elascticity should be made a member value of a rigidbody
	float j = glm::dot((1 + elasticity) * other->getVelocity(), normal) / (1 / other->getMass());
	glm::vec2 force = normal * j;
	other->applyForce(-force, contactPos - other->getPosition());
}

void Plane::resetPosition()
{
}
