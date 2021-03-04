#pragma once
#include <glm/vec2.hpp>
#include "glm/vec4.hpp"
enum class ShapeType
{
	PLANE = 0,
	SPHERE,
	AABB,
	NUM_SHAPES = 3
};

class PhysicsObject
{
protected:
	PhysicsObject(ShapeType shape, glm::vec4 colour) : shapeType(shape), colour(colour) {}
	ShapeType shapeType;
	glm::vec4 colour;
public:
	virtual void fixedUpdate(glm::vec2 gravity, float timeStep) = 0;
	virtual void draw(float ptnt) = 0;//ptnt stands for percentage to next tick, for temporal anti aliasing
	virtual void resetPosition() {};
	glm::vec4 getColour() { return colour; }
	ShapeType getShapeType() { return shapeType; }
};