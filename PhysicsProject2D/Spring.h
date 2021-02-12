#pragma once
#include "PhysicsObject.h"
#include "Rigidbody.h"

class PhysicsScene;

class Spring : public PhysicsObject
{
public:
	Spring(Rigidbody* actor1, Rigidbody* actor2, float dampening, float springCoefficient, float restLength = 0, glm::vec2 contact1 = glm::vec2(0), glm::vec2 contact2 = glm::vec2(0));
	~Spring();

	virtual void fixedUpdate(glm::vec2 gravity, float timeStep);
	virtual void makeGizmo();
	virtual void debug() const {};

	bool isInside(glm::vec2 point) final { return false; }

	glm::vec2 getContact1() const { return actor1 ? actor1->toWorld(contact1) : contact1; }
	glm::vec2 getContact2() const { return actor2 ? actor2->toWorld(contact2) : contact2; }

	float getRestLength() const { return restLength; }
	void setRestLength(const float restLength) { this->restLength = restLength; }

protected:
	Rigidbody* actor1;
	Rigidbody* actor2;

	glm::vec2 contact1;
	glm::vec2 contact2;

	float dampening;
	float restLength;
	float springCoefficient;

};