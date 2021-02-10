#include "Sphere.h"
#include <Gizmos.h>

Sphere::Sphere(glm::vec2 position, glm::vec2 velocity, float mass, float radius, float elasticity, glm::vec4 color) :
	Rigidbody(SPHERE, position, velocity, 0.0f, mass, elasticity)
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
}