#include "Rigidbody.h"

Rigidbody::Rigidbody(ShapeType shapeID, glm::vec2 position, glm::vec2 velocity, float rotation, float mass, float elasticity) :
	PhysicsObject(shapeID)
{
	this->position = position;
	this->velocity = velocity;
	this->rotation = rotation;
	this->mass = mass;
	this->elasticity = elasticity;
}

Rigidbody::~Rigidbody()
{
}


void Rigidbody::fixedUpdate(glm::vec2 gravity, float timeStep)
{
	applyForce(gravity * getMass() * timeStep);
	position += getVelocity() * timeStep;
}


void Rigidbody::applyForce(glm::vec2 force)
{
	velocity += force / getMass();
}

void Rigidbody::applyForceToOther(Rigidbody* otherActor, glm::vec2 force)
{
	applyForce(-force);
	otherActor->applyForce(force);
}


void Rigidbody::resolveCollision(Rigidbody* otherActor)
{
	glm::vec2 normal = glm::normalize(otherActor->getPosition() - getPosition());
	glm::vec2 relativeVelocity = otherActor->getVelocity() - getVelocity();

	float j = glm::dot(-(1.0f + elasticity) * relativeVelocity, normal) / ((1.0f / getMass()) + (1.0f / otherActor->getMass()));
	glm::vec2 impulse = normal * j;

	applyForceToOther(otherActor, impulse);
}