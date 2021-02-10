#include "PhysicsScene.h"
#include "PhysicsObject.h"

PhysicsScene::PhysicsScene() : timeStep(0.01F), gravity({ 0,0 })
{

}

PhysicsScene::~PhysicsScene()
{
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
	}
}

void PhysicsScene::draw()
{
	for (auto pActor : actors) 
	{
		pActor->draw();
	}
}