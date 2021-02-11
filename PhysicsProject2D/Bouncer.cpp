#include "Bouncer.h"




Bouncer::Bouncer(glm::vec2 position, float radius, float elasticity, glm::vec4 color) :
	Sphere(position, glm::vec2(0), 1, radius, elasticity, 0, 0, color)
{
	setKinematic(true);
}

void Bouncer::fixedUpdate(glm::vec2 gravity, float timestep)
{
	Rigidbody::fixedUpdate(gravity, timestep);
}

void Bouncer::makeGizmo()
{
	Sphere::makeGizmo();
}
