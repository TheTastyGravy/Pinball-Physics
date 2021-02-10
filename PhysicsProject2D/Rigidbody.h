#pragma once
#include "PhysicsObject.h"

class Rigidbody : public PhysicsObject
{
public:
	Rigidbody(ShapeType shapeID, glm::vec2 position, glm::vec2 velocity, float rotation, float mass, float elasticity = 1.0f);
	~Rigidbody();

	virtual void fixedUpdate(glm::vec2 gravity, float timeStep);
	virtual void debug() {}

	void applyForce(glm::vec2 force);
	void applyForceToOther(Rigidbody* otherActor, glm::vec2 force);

	void resolveCollision(Rigidbody* otherActor);


	glm::vec2 getPosition() const { return position; }
	glm::vec2 getVelocity() const { return velocity; }
	float getMass() const { return mass; }
	float getRotation() const { return rotation; }

protected:
	glm::vec2 position;
	glm::vec2 velocity;
	float mass;
	float elasticity;
	float rotation;

	float angularVelocity;
	float moment;

};