#pragma once
#include "PhysicsObject.h"

// This is a one-sided object that extends infinetly along both its edge and backwards from its normal direction

class Rigidbody;
class Plane : public PhysicsObject
{
public:
	Plane();
	Plane(glm::vec2 normal, float distance);
	~Plane();

	virtual void fixedUpdate(glm::vec2 gravity, float timeStep);
	virtual void makeGizmo();
	virtual void debug() {};
	virtual void resetPosition() {};

	void resolveCollision(Rigidbody* otherActor, glm::vec2 contact);

	glm::vec2 getNormal() const { return normal; }
	float getDistance() const { return distanceToOrigin; }
	glm::vec4 getColor() const { return color; }

protected:
	glm::vec2 normal;
	float distanceToOrigin;
	glm::vec4 color;
	
};