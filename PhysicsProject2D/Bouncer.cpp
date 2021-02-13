#include "Bouncer.h"

Bouncer::Bouncer(glm::vec2 position, float radius, float elasticity, int& score, glm::vec4 color) :
	Sphere(position, glm::vec2(0), 1, radius, elasticity, 0, 0, color)
{
	setKinematic(true);
	collisionCallback = [this, &score](PhysicsObject* other)
	{
		Rigidbody* otherRB = dynamic_cast<Rigidbody*>(other);
		if (otherRB)
		{
			score += 100;

			// Apply constant force to colliding rigidbody
			glm::vec2 force = otherRB->getPosition() - this->position;
			force = glm::normalize(force) * 50.0f;
			otherRB->applyForce(force, glm::vec2(0));
		}
	};
}

void Bouncer::fixedUpdate(glm::vec2 gravity, float timestep)
{
	Rigidbody::fixedUpdate(gravity, timestep);
}

void Bouncer::makeGizmo()
{
	Sphere::makeGizmo();
}
