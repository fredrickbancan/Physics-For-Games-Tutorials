#include "PhysicsScene.h"
#include "PhysicsObject.h"
#include "Sphere.h"
#include "Plane.h"
#include "AABB.h"
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
	return sphere2Plane(obj2, obj1);
}

bool plane2Aabb(PhysicsObject* obj1, PhysicsObject* obj2)
{
	return aabb2Plane(obj2, obj1);
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
		glm::vec2 contact = sphere->getPosition() + (collisionNormal * -sphere->getRadius());
		if (intersection > 0 && velocityOutOfPlane < 0)
		{
			plane->resolveCollision(sphere, contact);
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
			sphere1->resolveCollision(sphere2, 0.5f * (sphere1->getPosition() + sphere2->getPosition()));
			return true;
		}
	}
	return false;
}

bool sphere2Aabb(PhysicsObject* obj1, PhysicsObject* obj2)
{
	return aabb2Sphere(obj2, obj1);
}

bool aabb2Plane(PhysicsObject* obj1, PhysicsObject* obj2)
{
	//TODO: implement
	return false;
}

bool aabb2Sphere(PhysicsObject* obj1, PhysicsObject* obj2)
{ 
	AABB* box = dynamic_cast<AABB*>(obj1);
	Sphere* sphere = dynamic_cast<Sphere*>(obj2);

	// if we are successful then test for collision
	if (box != nullptr && sphere != nullptr)
	{
		glm::vec2 spherePos = sphere->getPosition();
		glm::vec2 closestPoint = box->clampToBox(spherePos);
		glm::vec2 colNorm = glm::normalize(spherePos - closestPoint);

		if (glm::distance(closestPoint, spherePos) <= sphere->getRadius())
		{
			//collide
			box->resolveCollision(sphere, closestPoint, &colNorm);
			return true;
		}
	}
	return false;
}

bool aabb2Aabb(PhysicsObject* obj1, PhysicsObject* obj2)
{
	AABB* box1 = dynamic_cast<AABB*>(obj1);
	AABB* box2 = dynamic_cast<AABB*>(obj2);
	if (box1 != nullptr && box2 != nullptr)
	{
		if (!(box1->minX() > box2->maxX() || box1->maxX() < box2->minX() || box1->minY() > box2->maxY() || box1->maxY() < box2->minY()))
		{
			//TODO: implement
			glm::vec2 box2Clamped = box1->clampToBox(box2->getPosition());
			box1->resolveCollision(box2, box2Clamped, );
			return true;
		}
	}
	return false;
}
