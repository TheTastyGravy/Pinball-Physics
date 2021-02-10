#include "Plane.h"
#include "Rigidbody.h"
#include <Gizmos.h>

Plane::Plane() : PhysicsObject(PLANE)
{
	this->normal = glm::vec2(0, 1);
	this->distanceToOrigin = 0.0f;
	this->color = glm::vec4(1);
}

Plane::Plane(glm::vec2 normal, float distance) : PhysicsObject(PLANE)
{
	this->normal = glm::normalize(normal);
	this->distanceToOrigin = distance;
	this->color = glm::vec4(0, 1, 0, 1);
}

Plane::~Plane()
{
}


void Plane::fixedUpdate(glm::vec2 gravity, float timeStep)
{
}

void Plane::makeGizmo()
{
	float lineSegmentLength = 300.0f;
	glm::vec2 centerPoint = normal * distanceToOrigin;
	glm::vec2 parallel(normal.y, -normal.x);
	glm::vec4 colorFade = color;
	colorFade.a = 0;

	glm::vec2 start = centerPoint + (parallel * lineSegmentLength);
	glm::vec2 end = centerPoint - (parallel * lineSegmentLength);

	//draw lines
	aie::Gizmos::add2DTri(start, end, start - getNormal() * 5.0f, getColor(), getColor(), colorFade);
	aie::Gizmos::add2DTri(end, end - getNormal() * 5.0f, start - getNormal() * 5.0f, getColor(), colorFade, colorFade);
}


void Plane::resolveCollision(Rigidbody* otherActor, glm::vec2 contact)
{
	// The posiution at which we'll apply the force, relative to the other objects center of mass
	glm::vec2 localContact = contact - otherActor->getPosition();

	// The plane never moves, so the relative velocity is just 
	// the other actors velocity at the contact point
	glm::vec2 vRel = otherActor->getVelocity() + otherActor->getAngularVelocity() * glm::vec2(-localContact.y, localContact.x);
	float velocityIntoPlane = glm::dot(vRel, normal);

	// This is the perpendicular distance we apply the force at relative 
	// to the center of mass: Torque = F * r
	float r = glm::dot(localContact, glm::vec2(normal.y, -normal.x));

	// This will work out the 'effective mass' - a combination of the moment of inertia 
	// and mass, it will tell how much the contact point velocity will change the force we apply
	float mass0 = 1.0f / (1.0f / otherActor->getMass() + (r * r) / otherActor->getMoment());

	// The plane does not move (static) so we only use the other actors velocity
	float j = -(1 + otherActor->getElasticity()) * velocityIntoPlane * mass0;
	glm::vec2 force = normal * j;
	otherActor->applyForce(force, contact - otherActor->getPosition());
}