#pragma once
#include <glm/vec2.hpp>
#include <vector>
class PhysicsScene
{
public:
	PhysicsScene();
	~PhysicsScene();
	void addActor(class PhysicsObject* actor);
	void removeActor(class PhysicsObject* actor);
	void update(float dt);
	void draw();
	void setGravity(const glm::vec2 gravity) { this->gravity = gravity; }
	glm::vec2 getGravity() const { return gravity; }
	void setTimeStep(const float timeStep) { this->timeStep = timeStep; }
	float getTimeStep() const { return timeStep; }
	void checkForCollision();
protected:
	glm::vec2 gravity;
	float timeStep;
	float percentageToNextTick;//used for interpolation (temporal anti aliasing)
	std::vector<class PhysicsObject*> actors;
};

static bool plane2Plane(class PhysicsObject* obj1, class PhysicsObject* obj2);
static bool plane2Sphere(class PhysicsObject* obj1, class PhysicsObject* obj2);
static bool plane2Aabb(class PhysicsObject* obj1, class PhysicsObject* obj2);
static bool sphere2Plane(class PhysicsObject* obj1, class PhysicsObject* obj2);
static bool sphere2Sphere(class PhysicsObject* obj1, class PhysicsObject* obj2);
static bool sphere2Aabb(class PhysicsObject* obj1, class PhysicsObject* obj2);
static bool aabb2Plane(class PhysicsObject* obj1, class PhysicsObject* obj2);
static bool aabb2Sphere(class PhysicsObject* obj1, class PhysicsObject* obj2);
static bool aabb2Aabb(class PhysicsObject* obj1, class PhysicsObject* obj2);