#include "Booster.h"

Booster::Booster(glm::vec2 position, float rotation, float width, float height, glm::vec2 appliedForce, glm::vec4 color) :
	Box(position, glm::vec2(0), rotation, 1, width, height, 1, 1, 1, color)
{
	this->setKinematic(true);
	this->setTrigger(true);

	// When a rigidbody enters the trigger, apply a force to it
	this->onTriggerEnter = [appliedForce](PhysicsObject* other) { dynamic_cast<Rigidbody*>(other)->applyForce(appliedForce, glm::vec2(0)); };
}