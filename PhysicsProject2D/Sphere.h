#pragma once
#include "Rigidbody.h"

class Sphere :public Rigidbody
{
public:
	Sphere(glm::vec2 position, glm::vec2 velocity, float mass, float radius, float elasticity = 1.0f, float linearDrag = 0.0f, float angularDrag = 0.0f, glm::vec4 color = glm::vec4(0, 0, 1, 1));
	~Sphere();

	virtual void makeGizmo();

	float getRadius() const { return radius; }
	glm::vec4 getColor() const { return color; }

protected:
	float radius;
	glm::vec4 color;

};