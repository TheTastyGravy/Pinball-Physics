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

	ShapeType getShapeID() const { return shapeID; }

	bool getKinematic() const { return isKinematic; }
	void setKinematic(const bool state) { this->isKinematic = state; }

	float getElasticity() const { return elasticity; }

protected:
	ShapeType shapeID;
	bool isKinematic;
	float elasticity;

	PhysicsObject(ShapeType shapeID, float elasticity) :
		shapeID(shapeID),
		elasticity(elasticity)
	{}

};