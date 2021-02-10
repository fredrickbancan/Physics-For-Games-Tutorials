#include "PhysicsScene.h"
#include "PhysicsObject.h"
#include "Sphere.h"
#include "Plane.h"
#include "glm/ext.hpp"

typedef bool(*fn)(PhysicsObject*, PhysicsObject*);

static fn collisionFunctionArray[] =
{
	plane2Plane, plane2Sphere, plane2Aabb,
	sphere2Plane, sphere2Sphere, sphere2Aabb,
	aabb2Plane, aabb2Sphere, aabb2Aabb
};

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
		checkForCollision();
	}
}

void PhysicsScene::draw()
{
	for (auto pActor : actors) 
	{
		pActor->draw();
	}
}

void PhysicsScene::checkForCollision()
{
	int actorCount = actors.size();
	//need to check for collisions against all objects except this one.
	for (int outer = 0; outer < actorCount - 1; outer++)
	{
		for (int inner = outer + 1; inner < actorCount; inner++)
		{
			PhysicsObject* object1 = actors[outer];
			PhysicsObject* object2 = actors[inner];
			ShapeType shapeId1 = object1->getShapeType();
			ShapeType shapeId2 = object2->getShapeType();
			// using function pointers
			int functionIdx = ((int)shapeId1 * (int)ShapeType::NUM_SHAPES) + (int)shapeId2;
			fn collisionFunctionPtr = collisionFunctionArray[functionIdx];
			if (collisionFunctionPtr != nullptr)
			{
				// did a collision occur?
				collisionFunctionPtr(object1, object2);
			}
		}
	}

}

bool plane2Plane(PhysicsObject* obj1, PhysicsObject* obj2)
{
	return false;
}

bool plane2Sphere(PhysicsObject* obj1, PhysicsObject* obj2)
{
	return false;
}

bool plane2Aabb(PhysicsObject* obj1, PhysicsObject* obj2)
{
	return false;
}

bool sphere2Plane(PhysicsObject* obj1, PhysicsObject* obj2)
{
	Sphere* sphere = dynamic_cast<Sphere*>(obj1);
	Plane* plane = dynamic_cast<Plane*>(obj2);

	//if we are successful then test for collision
	if (sphere != nullptr && plane != nullptr)
	{
		glm::vec2 collisionNormal = plane->getNormal();
		float sphereToPlane = glm::dot(sphere->getPosition(), plane->getNormal()) - plane->getDistance();
		float intersection = sphere->getRadius() - sphereToPlane;
		float velocityOutOfPlane = glm::dot(sphere->getVelocity(), plane->getNormal());
		if (intersection > 0 && velocityOutOfPlane < 0)
		{
			//set sphere velocity to zero here
			sphere->applyForce(-sphere->getVelocity() * sphere->getMass());
			return true;
		}
	}
	return false;

}

bool sphere2Sphere(PhysicsObject* obj1, PhysicsObject* obj2)
{
	// try to cast objects to sphere and sphere
	Sphere* sphere1 = dynamic_cast<Sphere*>(obj1);
	Sphere* sphere2 = dynamic_cast<Sphere*>(obj2);

	// if we are successful then test for collision
	if (sphere1 != nullptr && sphere2 != nullptr)
	{
		if (glm::length(sphere1->getPosition() - sphere2->getPosition()) <= (sphere1->getRadius() + sphere2->getRadius()))
		{
			return true;
		}
		return false;
	}
	return false;
}

bool sphere2Aabb(PhysicsObject* obj1, PhysicsObject* obj2)
{
	return false;
}

bool aabb2Plane(PhysicsObject* obj1, PhysicsObject* obj2)
{
	return false;
}

bool aabb2Sphere(PhysicsObject* obj1, PhysicsObject* obj2)
{
	return false;
}

bool aabb2Aabb(PhysicsObject* obj1, PhysicsObject* obj2)
{
	return false;
}
