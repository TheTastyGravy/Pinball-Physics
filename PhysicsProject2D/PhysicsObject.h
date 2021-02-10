#pragma once
#include <glm/glm.hpp>

// This is a pure abstact class. It will be used as a base for the derived class that will use fixedUpdate

enum ShapeType
{
	PLANE = 0,
	SPHERE,
	BOX,
	SHAPE_COUNT
};

class PhysicsObject
{
public:
	virtual void fixedUpdate(glm::vec2 gravity, float timeStep) = 0;
	virtual void makeGizmo() = 0;

	virtual void debug() = 0;
	
	virtual void resetPosition() {};

	ShapeType getShapeID() const { return shapeID; }

protected:
	ShapeType shapeID;

	PhysicsObject(ShapeType shapeID) :
		shapeID(shapeID)
	{}

};