#include "Rigidbody.h"

Rigidbody::Rigidbody(ShapeType shapeID, glm::vec2 position, glm::vec2 velocity, float rotation, float mass) :
	PhysicsObject(shapeID)
{
	this->position = position;
	this->velocity = velocity;
	this->rotation = rotation;
	this->mass = mass;
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