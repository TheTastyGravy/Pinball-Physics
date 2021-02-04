#pragma once
#include "PhysicsObject.h"

class Rigidbody : public PhysicsObject
{
public:
	Rigidbody(ShapeType shapeID, glm::vec2 position, glm::vec2 velocity, float rotation, float mass);
	~Rigidbody();

	virtual void fixedUpdate(glm::vec2 gravity, float timeStep);

	virtual void debug() {}

	void applyForce(glm::vec2 force);
	void applyForceToOther(Rigidbody* otherActor, glm::vec2 force);

	virtual bool checkCollision(PhysicsObject* other) = 0;

	glm::vec2 getPosition() { return position; }
	glm::vec2 getVelocity() { return velocity; }
	float getMass() { return mass; }
	float getRotation() { return rotation; }

protected:
	glm::vec2 position;
	glm::vec2 velocity;
	float mass;
	float rotation;

};