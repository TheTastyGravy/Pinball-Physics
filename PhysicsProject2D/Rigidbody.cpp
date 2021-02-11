#include "Rigidbody.h"
#include "PhysicsScene.h"
#include <iostream>

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
	this->allowRotationAsKinematic = false;
	this->isTriggerFlag = false;
}

Rigidbody::~Rigidbody()
{
}


void Rigidbody::fixedUpdate(glm::vec2 gravity, float timestep)
{
	// Checks if it is a trigger
	if (isTrigger())
	{
		// This will let us check every object that is inside a trigger object and call onTriggerEnter on if they
		// havent registered inside the trigger this frame, they must have exited so we can remove them from the list and then call onTriggerExit
		for (auto it = objectsInside.begin(); it != objectsInside.end(); it++)
		{
			if (std::find(objectsInsideThisFrame.begin(), objectsInsideThisFrame.end(), *it) == objectsInsideThisFrame.end())
			{
				if (onTriggerExit)
				{
					onTriggerExit(*it);
				}

				it = objectsInside.erase(it);
				if (it == objectsInside.end())
				{
					break;
				}
			}
		}

		// Clear the list now for the next frame
		objectsInsideThisFrame.clear();
	}


	// Kinematic objects dont react to forces
	if (isKinematic)
	{
		velocity = glm::vec2(0);

		// If flag is set, update rotation
		if (allowRotationAsKinematic)
		{
			rotation += angularVelocity * timestep;
			angularVelocity -= angularVelocity * angularDrag * timestep;
			if (glm::length(angularVelocity) < MIN_ANGULAR_THRESHHOLD)
			{
				angularVelocity = 0.0f;
			}
		}
		else
		{
			angularVelocity = 0;
		}

		return;
	}

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
		if (glm::length(velocity) < glm::length(gravity) * linearDrag * timestep)
		{
			velocity = glm::vec2(0);
		}
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
	bool collidingWithTrigger = false;

	// Register that these two objects have overlapped this frame. This is only used by triggers
	if (isTrigger())
	{
		objectsInsideThisFrame.push_back(otherActor);
		triggerEnter(otherActor);
		collidingWithTrigger = true;
	}
	if (otherActor->isTrigger())
	{
		otherActor->objectsInsideThisFrame.push_back(this);
		otherActor->triggerEnter(this);
		collidingWithTrigger = true;
	}

	// Dont collide with triggers
	if (collidingWithTrigger)
	{
		return;
	}
	

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

		// Trigger collision callbacks
		if (collisionCallback)
			{
				collisionCallback(otherActor);
			}
		if (otherActor->collisionCallback)
			{
				otherActor->collisionCallback(this);
			}

		// Apply contact forces to prevent objects from being inside eachother
		if (pen > 0)
		{
			PhysicsScene::applyContactForces(this, otherActor, normal, pen);
		}
	}
}


glm::vec2 Rigidbody::toWorld(const glm::vec2 localPos) const
{
	return position + localX * localPos.x + localY * localPos.y;
}


void Rigidbody::triggerEnter(PhysicsObject* otherActor)
{
	if (isTrigger() && std::find(objectsInside.begin(), objectsInside.end(), otherActor) == objectsInside.end())
	{
		objectsInside.push_back(otherActor);
		if (onTriggerEnter)
		{
			onTriggerEnter(otherActor);
		}
	}
}