#include "Rigidbody.h"
#include "PhysicsScene.h"

const float Rigidbody::MIN_LINEAR_THRESHHOLD = 0.001f;
const float Rigidbody::MIN_ANGULAR_THRESHHOLD = 0.001f;

Rigidbody::Rigidbody(ShapeType shapeID, glm::vec2 position, glm::vec2 velocity, float rotation, float mass, float elasticity, float linearDrag, float angularDrag) :
	PhysicsObject(shapeID, elasticity)
{
	this->position = position;
	this->velocity = velocity;
	this->rotation = rotation;
	this->angularVelocity = 0;

	this->mass = mass;
	this->linearDrag = linearDrag;
	this->angularDrag = angularDrag;

	this->isKinematic = false;
}

Rigidbody::~Rigidbody()
{
}


void Rigidbody::fixedUpdate(glm::vec2 gravity, float timestep)
{
	applyForce(gravity * getMass() * timestep, glm::vec2(0));

	// Apply velocity
	position += getVelocity() * timestep;
	rotation += angularVelocity * timestep;

	// Apply drag
	velocity -= velocity * linearDrag * timestep;
	angularVelocity -= angularVelocity * angularDrag * timestep;
	
	// Stop the object if its velocity is too low
	if (glm::length(velocity) < MIN_LINEAR_THRESHHOLD)
	{
		velocity = glm::vec2(0);
	}
	if (glm::length(angularVelocity) < MIN_ANGULAR_THRESHHOLD)
	{
		angularVelocity = 0.0f;
	}
}


void Rigidbody::applyForce(glm::vec2 force, glm::vec2 pos)
{
	velocity += force / getMass();
	angularVelocity += (force.y * pos.x - force.x * pos.y) / getMoment();
}


void Rigidbody::resolveCollision(Rigidbody* otherActor, glm::vec2 contact, glm::vec2* collisionNormal, float pen)
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

		if (pen > 0)
		{
			PhysicsScene::applyContactForces(this, otherActor, normal, pen);
		}
	}
}