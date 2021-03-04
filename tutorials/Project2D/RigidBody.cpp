#include "RigidBody.h"
#include "PhysicsApp.h"
#include "glm/ext.hpp"

Rigidbody::Rigidbody(ShapeType shapeID, glm::vec2 position, glm::vec2 velocity, float orientation, float mass, glm::vec4 colour) : PhysicsObject(shapeID, colour)
{
	this->position = position;
	prevTickPosition = position;
	this->velocity = velocity;
	this->orientation = orientation;
	this->mass = mass;
	angularVelocity = 0;
	momentOfInertia = 0;
	isStatic = false;
}

Rigidbody::~Rigidbody()
{
}

void Rigidbody::fixedUpdate(glm::vec2 gravity, float timeStep)
{
	if (isStatic) return;
	prevTickPosition = position;
	position += velocity * timeStep;
	applyForce(gravity * mass);
	orientation += angularVelocity * timeStep;
}

void Rigidbody::applyForce(glm::vec2 force)
{
	if (isStatic) return;

	velocity += force / mass;
}


void Rigidbody::resolveCollision(Rigidbody* actor2, glm::vec2* collisionNormal /* = nullptr*/)
{
	//TODO: Implement an overlap vector paremeter for more realistic and precise collision resolution via position correction.
	
	//avoid NAN issues
	if (actor2->getPosition() == position)
	{
		return;
	}

	glm::vec2 force{};

	glm::vec2 normal = collisionNormal ? *collisionNormal : glm::normalize(actor2->getPosition() - position);

	glm::vec2 relativeVelocity = actor2->getVelocity() - velocity;

	float elasticity = 1.0F;

	if (isStatic)
	{
		float j = glm::dot(-(1.0F + elasticity) * relativeVelocity, normal);
		force = normal * (j < 0 ? 0 : j);
		actor2->applyForce(force * actor2->getMass());
	}
	else if (actor2->isStatic)
	{
		float j = glm::dot(-(1.0F + elasticity) * relativeVelocity, normal);
		force = normal * (j < 0 ? 0 : j);
		applyForce(-force * mass);
	}
	else
	{
		float j = glm::dot(-(1.0F + elasticity) * (relativeVelocity), normal) / ((1 / mass) + (1 / actor2->getMass()));
		force = normal * (j < 0 ? 0 : j);
		applyForce(-force);
		actor2->applyForce(force);
	}

}

glm::vec2 Rigidbody::lerpPos(float ptnt)
{
	return glm::vec2{ PhysicsApp::lerp(prevTickPosition.x, position.x, ptnt), PhysicsApp::lerp(prevTickPosition.y, position.y, ptnt) };
}
