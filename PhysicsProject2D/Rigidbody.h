#pragma once
#include "PhysicsObject.h"
#include <list>
#include <functional>
#include <iostream>

class Rigidbody : public PhysicsObject
{
public:
	Rigidbody(ShapeType shapeID, glm::vec2 position, glm::vec2 velocity, float rotation, float mass, float elasticity = 1.0f, float linearDrag = 0.0f, float angularDrag = 0.0f);
	~Rigidbody();

	virtual void fixedUpdate(glm::vec2 gravity, float timeStep);
	virtual void debug() const {}

	void applyForce(glm::vec2 force, glm::vec2 pos);
	void resolveCollision(Rigidbody* otherActor, glm::vec2 contact, glm::vec2* collisionNormal = nullptr, float pen = 0);


	glm::vec2 getPosition() const { return position; }
	glm::vec2 getVelocity() const { return velocity; }
	float getRotation() const { return rotation; }
	float getAngularVelocity() const { return angularVelocity; }

	float getMass() const { return isKinematic ? INT_MAX : mass; }
	float getLinearDrag() const { return linearDrag; }
	float getAngularDrag() const { return angularDrag; }

	float getMoment() const { return isKinematic ? INT_MAX : moment; }

	glm::vec2 getLocalX() const { return localX; }
	glm::vec2 getLocalY() const { return localY; }


	void setPosition(const glm::vec2 position) { this->position = position; }
	void setRotation(const float rotation) { this->rotation = rotation; }

	glm::vec2 toWorld(const glm::vec2 localPos) const;
	std::function<void(PhysicsObject*)> collisionCallback;


	bool isTrigger() const { return isTriggerFlag; }
	void setTrigger(bool state) { isTriggerFlag = state; }

	void triggerEnter(PhysicsObject* otherActor);
	std::function<void(PhysicsObject*)> onTriggerEnter;
	std::function<void(PhysicsObject*)> onTriggerExit;

protected:
	static const float MIN_LINEAR_THRESHHOLD;
	static const float MIN_ANGULAR_THRESHHOLD;

	bool isTriggerFlag;
	std::list<PhysicsObject*> objectsInside;
	std::list<PhysicsObject*> objectsInsideThisFrame;


	glm::vec2 position;
	glm::vec2 velocity;
	float rotation;
	float angularVelocity;

	float mass;
	float linearDrag;
	float angularDrag;
	float moment;

	// These will store the local x and y axes of the box based on its angle of rotation
	glm::vec2 localX;
	glm::vec2 localY;

};