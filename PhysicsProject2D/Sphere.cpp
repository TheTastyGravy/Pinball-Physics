#include "Sphere.h"
#include <Gizmos.h>

Sphere::Sphere(glm::vec2 position, glm::vec2 velocity, float mass, float radius, float elasticity, float linearDrag, float angularDrag, glm::vec4 color) :
	Rigidbody(SPHERE, position, velocity, 0.0f, mass, elasticity, linearDrag, angularDrag)
{
	this->radius = radius;
	this->color = color;

	this->moment = (2.f/5.f) * mass * (radius * radius);
}

Sphere::~Sphere()
{
}


void Sphere::makeGizmo()
{
	aie::Gizmos::add2DCircle(position, radius, 12, color);

	glm::vec2 thing(cosf(rotation), sinf(rotation));

	aie::Gizmos::add2DLine(position, position + thing * 2.0f, glm::vec4(1, 1, 1, 1));
}


bool Sphere::isInside(glm::vec2 point)
{
	return glm::distance(point, getPosition()) <= getRadius();
}