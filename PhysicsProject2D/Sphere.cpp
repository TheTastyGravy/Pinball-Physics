#include "Sphere.h"
#include <Gizmos.h>

Sphere::Sphere(glm::vec2 position, glm::vec2 velocity, float mass, float radius, float elasticity, float linearDrag, float angularDrag, glm::vec4 color) :
	Rigidbody(SPHERE, position, velocity, 0.0f, mass, elasticity, linearDrag, angularDrag)
{
	this->radius = radius;
	this->color = color;

	this->moment = (2.f/5.f) * mass * (radius * radius);

	this->collisionCallback = [this](PhysicsObject* other, glm::vec2 collision)
	{
		Rigidbody* otherRB = dynamic_cast<Rigidbody*>(other);
		glm::vec2 otherVel = otherRB ? otherRB->getVelocity() : glm::vec2(0);

		//find the perpendicular of the collision
		glm::vec2 norm = glm::normalize(collision - getPosition());
		glm::vec2 perp(norm.y, -norm.x);

		glm::vec2 relativeVel = getVelocity() + otherVel;

		//find the speed of the collision point
		//angular velocity = velocity / radius
		angularVelocity = glm::dot(relativeVel, perp) / getRadius();
	};
}

Sphere::~Sphere()
{
}


void Sphere::makeGizmo()
{
	aie::Gizmos::add2DCircle(position, radius, 12, color);

	//draw line for rotation on non kinematic objects
	if (!getKinematic())
	{
		glm::vec2 end = glm::vec2(std::cos(rotation), std::sin(rotation)) * radius;
		aie::Gizmos::add2DLine(position, position + end, glm::vec4(1));
	}
}


bool Sphere::isInside(glm::vec2 point)
{
	return glm::distance(point, getPosition()) <= getRadius();
}