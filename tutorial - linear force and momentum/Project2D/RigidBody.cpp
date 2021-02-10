#include "RigidBody.h"

Rigidbody::Rigidbody(ShapeType shapeID, glm::vec2 position, glm::vec2 velocity, float orientation, float mass) : PhysicsObject(shapeID)
{
	this->position = position;
	this->velocity = velocity;
	this->orientation = orientation;
	this->mass = mass;
}

Rigidbody::~Rigidbody()
{
}

void Rigidbody::fixedUpdate(glm::vec2 gravity, float timeStep)
{
	position += velocity * timeStep;
	applyForce(gravity * mass);
}

void Rigidbody::applyForce(glm::vec2 force)
{
	velocity += force / mass;
}

void Rigidbody::applyForceToActor(Rigidbody* actor2, glm::vec2 force)
{
	actor2->applyForce(force);
	applyForce(-force);
}
