#include "Rigidbody.h"

Rigidbody::Rigidbody(ShapeType shapeID, glm::vec2 position, glm::vec2 velocity, float rotation, float mass, float elasticity) :
	PhysicsObject(shapeID)
{
	this->position = position;
	this->velocity = velocity;
	this->rotation = rotation;
	this->mass = mass;
	this->elasticity = elasticity;

	this->angularVelocity = 0;
}

Rigidbody::~Rigidbody()
{
}


void Rigidbody::fixedUpdate(glm::vec2 gravity, float timeStep)
{
	applyForce(gravity * getMass() * timeStep, glm::vec2(0));

	position += getVelocity() * timeStep;
	rotation += angularVelocity * timeStep;
}


void Rigidbody::applyForce(glm::vec2 force, glm::vec2 pos)
{
	velocity += force / getMass();
	angularVelocity += (force.y * pos.x - force.x * pos.y);
}


void Rigidbody::resolveCollision(Rigidbody* otherActor, glm::vec2 contact, glm::vec2* collisionNormal)
{
	// Find the vector between their centers, or use the provided
	// direction of force, and make sure its normalized
	glm::vec2 normal = glm::normalize(collisionNormal ? *collisionNormal : otherActor->getPosition() - getPosition());

	// Get the vector perpendicular to the collision normal
	glm::vec2 perpColNorm(normal.y, -normal.x);

	// These are applied to the radius from axis to the application of force
	float radius1 = glm::dot(contact - position, -perpColNorm);
	float radius2 = glm::dot(contact - otherActor->getPosition(), perpColNorm);

	// Velocity of the contact point on this object
	float cp_velocity1 = glm::dot(velocity, normal) - radius1 * angularVelocity;
	// Velocities of contact point of the other object
	float cp_velocity2 = glm::dot(otherActor->getVelocity(), normal) + radius2 * otherActor->getAngularVelocity();


	if (cp_velocity1 > cp_velocity2) // They are moving closer
	{
		// This will calculate the effective mass at the point of each 
		// object (How much it will move due to the forces applied)
		float mass1 = 1.0f / (1.0f / mass + (radius1 * radius1) / getMoment());
		float mass2 = 1.0f / (1.0f / otherActor->mass + (radius2 * radius2) / otherActor->getMoment());

		// Use the average elasticity of the colliding objects
		float elasticity = 0.5f * (getElasticity() + otherActor->getElasticity());

		glm::vec2 impact = (1.0f + elasticity) * mass1 * mass2 / (mass1 + mass2) * (cp_velocity1 - cp_velocity2) * normal;
		applyForce(-impact, contact - position);
		otherActor->applyForce(impact, contact - otherActor->getPosition());
	}
}