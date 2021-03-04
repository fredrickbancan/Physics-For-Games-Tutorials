#pragma once
#include "PhysicsObject.h"

class Rigidbody : public PhysicsObject 
{
public:
	Rigidbody(ShapeType shapeID, glm::vec2 position, glm::vec2 velocity, float orientation, float mass, glm::vec4 colour);
	~Rigidbody();
	virtual void fixedUpdate(glm::vec2 gravity, float timeStep);
	void applyForce(glm::vec2 force);
	void resolveCollision(Rigidbody* actor2, glm::vec2* collisionNormal = nullptr);
	glm::vec2 getPosition() { return position; }
	float getOrientatation() { return orientation; }
	glm::vec2 getVelocity() { return velocity; }
	void setVelocity(glm::vec2 vel) { velocity = vel; }
	float getMass() { return mass; }
	float getMomentOfInertia() { return momentOfInertia; }
	void setAngularVelocity(float f) { angularVelocity = f; }
	void setStatic(bool flag) { isStatic = flag; }
	void offset(glm::vec2 amount) { position += amount; }
	bool getIsStatic() { return isStatic; }
	glm::vec2 lerpPos(float ptnt);
protected:
	glm::vec2 position;
	glm::vec2 prevTickPosition;
	glm::vec2 velocity;
	float mass;
	float orientation; //2D so we only need a single float to represent our orientation
	float angularVelocity;
	float momentOfInertia;
	bool isStatic;
};