#include "PhysicsScene.h"
#include "PhysicsObject.h"
#include "Sphere.h"
#include "glm/ext.hpp"

PhysicsScene::PhysicsScene() : timeStep(0.01F), gravity({ 0,0 })
{

}

PhysicsScene::~PhysicsScene()
{
	for (auto pActor : actors)
	{
		delete pActor;
	}
}

void PhysicsScene::addActor(PhysicsObject* actor)
{
	actors.push_back(actor);
}

void PhysicsScene::removeActor(PhysicsObject* actor)
{
	for(std::vector<PhysicsObject*>::iterator i = actors.begin(); i != actors.end(); i++)
	{
		if (*i == actor)
		{
			std::swap(actors.back(), *i);
			actors.erase(actors.end());
		}
	}
}

void PhysicsScene::update(float dt)
{
	// update physics at a fixed time step
	static float accumulatedTime = 0.0f;
	accumulatedTime += dt;
	while (accumulatedTime >= timeStep)
	{
		for (auto pActor : actors)
		{
			pActor->fixedUpdate(gravity, timeStep);
		}
		accumulatedTime -= timeStep;

		// check for collisions (ideally you'd want to have some sort of
		// scene management in place)
		int actorCount = actors.size();
		// need to check for collisions against all objects except this one.
		for (int outer = 0; outer < actorCount - 1; outer++)
		{
			for (int inner = outer + 1; inner < actorCount; inner++)
			{
				PhysicsObject* object1 = actors[outer];
				PhysicsObject* object2 = actors[inner];
				// for now we can assume both shapes are spheres,
				// since that is all we’ve implemented for now.
				sphere2Sphere(object1, object2);
			}
		}
	}
}

void PhysicsScene::draw()
{
	for (auto pActor : actors) 
	{
		pActor->draw();
	}
}

bool sphere2Sphere(PhysicsObject* obj1, PhysicsObject* obj2)
{
	// try to cast objects to sphere and sphere
	Sphere* sphere1 = dynamic_cast<Sphere*>(obj1);
	Sphere* sphere2 = dynamic_cast<Sphere*>(obj2);

	// if we are successful then test for collision
	if (sphere1 != nullptr && sphere2 != nullptr)
	{
		// TODO do the necessary maths in here
		// TODO if the spheres touch, set their velocities to zero for now
		if (glm::length(sphere1->getPosition() - sphere2->getPosition()) <= (sphere1->getRadius() + sphere2->getRadius()))
		{
			sphere1->setVelocity({ 0, 0 });
			sphere2->setVelocity({ 0, 0 });
			return true;
		}
		return false;
	}
	return false;
}
