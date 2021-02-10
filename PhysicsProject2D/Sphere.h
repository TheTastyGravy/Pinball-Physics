#pragma once
#include "Rigidbody.h"

class Sphere :public Rigidbody
{
public:
	Sphere(glm::vec2 position, glm::vec2 velocity, float mass, float radius, glm::vec4 color);
	~Sphere();

	virtual void makeGizmo();

	float getRadius() const { return radius; }
	glm::vec4 getColor() const { return color; }

protected:
	float radius;
	glm::vec4 color;

};