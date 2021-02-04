#include "Sphere.h"
#include <Gizmos.h>

Sphere::Sphere(glm::vec2 position, glm::vec2 velocity, float mass, float radius, glm::vec4 color) :
	Rigidbody(SPHERE, position, velocity, 0.0f, mass)
{
	this->radius = radius;
	this->color = color;
}

Sphere::~Sphere()
{
}


void Sphere::makeGizmo()
{
	aie::Gizmos::add2DCircle(position, radius, 12, color);
}

bool Sphere::checkCollision(PhysicsObject* other)
{
	Sphere* sphere = dynamic_cast<Sphere*>(other);

	if (sphere != nullptr)
	{
		float dist = glm::distance(getPosition(), sphere->getPosition());

		if (getRadius() + sphere->getRadius() > dist)
		{
			return true;
		}
	}

	return false;
}