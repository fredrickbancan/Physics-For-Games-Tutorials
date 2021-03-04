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

	//this is useful for interpolating the rendering of physics objects in between physics steps
	percentageToNextTick = accumulatedTime / timeStep;
}

void PhysicsScene::draw()
{
	for (auto pActor : actors) 
	{
		pActor->draw(percentageToNextTick);
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
		if (sphere->getIsStatic()) return false;
		glm::vec2 collisionNormal = plane->getNormal();
		float sphereToPlane = glm::dot(sphere->getPosition(), plane->getNormal()) - plane->getOffset();
		float intersection = sphere->getRadius() - sphereToPlane;
		float velocityOutOfPlane = glm::dot(sphere->getVelocity(), plane->getNormal());
		if (intersection > 0 && velocityOutOfPlane < 0)
		{
			plane->resolveCollision(sphere);
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
		if (glm::length(sphere1->getPosition() - sphere2->getPosition()) < (sphere1->getRadius() + sphere2->getRadius()))
		{
			sphere1->resolveCollision(sphere2);
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
	AABB* box = dynamic_cast<AABB*>(obj1);
	Plane* plane = dynamic_cast<Plane*>(obj2);

	if (plane != nullptr && box != nullptr)
	{
		if (box->getIsStatic()) return false;
		glm::vec2 collisionNormal = plane->getNormal();

		//brute force way - OPTIMIZE THIS!
		//There are highly optimized ways of doiing this (e.g. getting normal of plane, figure out the closest point and check that)
		//Create an array of four vertices and set them to the corners of the AABB
		glm::vec2 vertices[4];
		vertices[0] = box->getPosition() - (box->getExtents()); //bottom left
		vertices[1] = box->getPosition() + (box->getExtents()); //top right
		vertices[2].x = vertices[0].x; //top left
		vertices[2].y = vertices[1].y;
		vertices[3].x = vertices[1].x; //bottom right
		vertices[3].y = vertices[0].y;

		//Loop through the points ...
		for (int i = 0; i < 4; i++)
		{
			//Same as Sphere2Plane check, but with points rather than position.
			//Perform dot product of the plane's normal against each corner of the AABB; get a scalar
			float dotProduct = glm::dot(collisionNormal, vertices[i]);

			//Check the length of the vector between each point and the plane's distance from origin (i.e. the closest point on the plane)
			float distance = dotProduct - plane->getOffset();

			if (distance < 0)
			{
				plane->resolveCollision(box);
				return true;
			}
		}
	}

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

		if (closestPoint == spherePos) return false;

		glm::vec2 colNorm = glm::normalize(closestPoint - spherePos);

		if (glm::distance(closestPoint, spherePos) <= sphere->getRadius())
		{
			//collide
			sphere->resolveCollision(box, &colNorm);
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
		glm::vec2 collisionNormal{};

		//Subtract each AABB's position from the sum of their half extents; the difference being how far they need to be 'pushed' to not overlap
		float hOverlap = box1->getExtents().x + box2->getExtents().x - abs(box1->getPosition().x - box2->getPosition().x);
		float vOverlap = box1->getExtents().y + box2->getExtents().y - abs(box1->getPosition().y - box2->getPosition().y);

		if (hOverlap < 0 || vOverlap < 0)
		{
			return false;
		}

		if (hOverlap < vOverlap)
		{
			collisionNormal = {box1->getPosition().x > box2->getPosition().x ? -1 : 1, 0};
		}
		else
		{
			collisionNormal = { 0, box1->getPosition().y > box2->getPosition().y ? -1 : 1 };
		}

		box1->resolveCollision(box2, &collisionNormal);

		return true;
		
	}
	return false;
}
