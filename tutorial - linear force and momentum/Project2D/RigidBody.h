#pragma once
#include "PhysicsObject.h"

class Rigidbody : public PhysicsObject 
{
public:
	Rigidbody(ShapeType shapeID, glm::vec2 position, glm::vec2 velocity, float orientation, float mass);
	~Rigidbody();
	virtual void fixedUpdate(glm::vec2 gravity, float timeStep);
	void applyForce(glm::vec2 force);
	void applyForceToActor(Rigidbody* actor2, glm::vec2 force);
	glm::vec2 getPosition() { return position; }
	float getOrientatation() { return orientation; }
	glm::vec2 getVelocity() { return velocity; }
	void setVelocity(glm::vec2 vel) { velocity = vel; }
	float getMass() { return mass; }
protected:
	glm::vec2 position;
	glm::vec2 velocity;
	float mass;
	float orientation; //2D so we only need a single float to represent our orientation
};