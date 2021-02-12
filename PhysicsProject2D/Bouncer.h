#pragma once
#include "Sphere.h"


class Bouncer : public Sphere
{
public:
	Bouncer(glm::vec2 position, float radius, float elasticity, int& score, glm::vec4 color = glm::vec4(1, 0, 0, 1));
	~Bouncer() {};

	virtual void fixedUpdate(glm::vec2 gravity, float timestep);
	virtual void makeGizmo();



protected:





};