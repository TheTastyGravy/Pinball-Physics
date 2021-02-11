#pragma once
#include <glm/glm.hpp>

// This is a pure abstact class. It will be used as a base for the derived class that will use fixedUpdate

enum ShapeType
{
	JOINT = -1,
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

	virtual void debug() const = 0;

	ShapeType getShapeID() const { return shapeID; }

	bool getKinematic() const { return isKinematic; }
	void setKinematic(const bool state) { this->isKinematic = state; }

	float getElasticity() const { return elasticity; }

	virtual bool isInside(glm::vec2 point) = 0;

protected:
	ShapeType shapeID;
	bool isKinematic;
	float elasticity;

	PhysicsObject(ShapeType shapeID, float elasticity) :
		shapeID(shapeID),
		elasticity(elasticity)
	{}

};