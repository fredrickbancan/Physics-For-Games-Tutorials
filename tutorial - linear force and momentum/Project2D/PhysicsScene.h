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
protected:
	glm::vec2 gravity;
	float timeStep;
	std::vector<class PhysicsObject*> actors;
};

static bool sphere2Sphere(class PhysicsObject* obj1, class PhysicsObject* obj2);