#pragma once
#include "Box.h"


class Flipper : public Box
{
public:
	Flipper(glm::vec2 position, float rotation, float mass, float width, float height, glm::vec2 rotationPoint, bool isLeftSide, float elasticity = 1.0f, glm::vec4 color = glm::vec4(0, 0, 1, 1));
	~Flipper() {};

	virtual void fixedUpdate(glm::vec2 gravity, float timestep);

	void setKeyDown(const bool state) { this->keyDown = state; }

protected:
	glm::vec2 rotationPoint;
	float startRot;

	bool keyDown;
	bool isLeftSide;

};