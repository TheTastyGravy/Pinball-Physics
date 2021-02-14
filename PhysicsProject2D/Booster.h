#pragma once
#include "Box.h"

class Booster : public Box
{
public:
	Booster(glm::vec2 position, float rotation, float width, float height, glm::vec2 appliedForce, glm::vec4 color = glm::vec4(0, 1, 0, 0.5f));
	~Booster() {};

protected:

};