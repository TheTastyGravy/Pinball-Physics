#include "Spring.h"
#include "Rigidbody.h"
#include "PhysicsScene.h"
#include <Gizmos.h>

Spring::Spring(Rigidbody* actor1, Rigidbody* actor2, float dampening, float springCoefficient, float restLength, glm::vec2 contact1, glm::vec2 contact2) :
	PhysicsObject(JOINT, 0.f)
{
	this->actor1 = actor1;
	this->actor2 = actor2;
	this->contact1 = contact1;
	this->contact2 = contact2;

	this->dampening = dampening;
	this->springCoefficient = springCoefficient;

	this->restLength = (restLength == 0) ? glm::distance(getContact1(), getContact2()) : restLength;
}

Spring::~Spring()
{
}


void Spring::fixedUpdate(glm::vec2 gravity, float timestep)
{
	// Get the world coordinates of the start and end of the spring
	glm::vec2 p1 = getContact1();
	glm::vec2 p2 = getContact2();

	float length = glm::distance(p1, p2);
	glm::vec2 direction = glm::normalize(p2 - p1);

	// Applying dampening
	glm::vec2 relativeVel = actor2->getVelocity() - actor1->getVelocity();

	// F = -kX - bv
	glm::vec2 force = direction * springCoefficient * (restLength - length) - dampening * relativeVel;

	// We will cap the spring force to 1000N to prevent numerical instability
	const float threshold = 1000.0f;
	float forceMag = glm::length(force);
	if (forceMag > threshold)
	{
		force *= threshold / forceMag;
	}

	actor1->applyForce(-force * timestep, p1 - actor1->getPosition());
	actor2->applyForce(force * timestep, p2 - actor2->getPosition());
}

void Spring::makeGizmo()
{
	// Draw line between connections
	aie::Gizmos::add2DLine(getContact1(), getContact2(), glm::vec4(1, 0, 1, 1));
}