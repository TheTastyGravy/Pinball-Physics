#pragma once
#include "PhysicsObject.h"

class Rigidbody : public PhysicsObject
{
public:
	Rigidbody(ShapeType shapeID, glm::vec2 position, glm::vec2 velocity, float rotation, float mass, float elasticity = 1.0f, float linearDrag = 0.0f, float angularDrag = 0.0f);
	~Rigidbody();

	virtual void fixedUpdate(glm::vec2 gravity, float timeStep);
	virtual void debug() {}

	void applyForce(glm::vec2 force, glm::vec2 pos);
	void resolveCollision(Rigidbody* otherActor, glm::vec2 contact, glm::vec2* collisionNormal = nullptr);


	glm::vec2 getPosition() const { return position; }
	glm::vec2 getVelocity() const { return velocity; }
	float getMass() const { return mass; }
	float getElasticity() const { return elasticity; }
	float getRotation() const { return rotation; }

	float getAngularVelocity() const { return angularVelocity; }
	float getMoment() const { return moment; }


	void setRotation(const float rotation) { this->rotation = rotation; }

protected:
	static const float MIN_LINEAR_THRESHHOLD;
	static const float MIN_ANGULAR_THRESHHOLD;

	glm::vec2 position;
	glm::vec2 velocity;

	float rotation;
	float angularVelocity;

	float mass;
	float linearDrag;
	float angularDrag;
	float elasticity;
	
	float moment;

};