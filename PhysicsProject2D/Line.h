#pragma once
#include "PhysicsObject.h"

class Rigidbody;

class Line : public PhysicsObject
{
public:
	Line(glm::vec2 v1, glm::vec2 v2, float elasticity = 1.0f, glm::vec4 color = glm::vec4(1, 0, 0, 1));
	~Line() {};

	virtual void fixedUpdate(glm::vec2 gravity, float timeStep);
	virtual void makeGizmo();
	virtual void debug() const {};


	void resolveCollision(Rigidbody* otherActor, glm::vec2 contact);

	glm::vec2 getV1() const { return v1; }
	glm::vec2 getV2() const { return v2; }
	glm::vec2 getNormal() const { return normal; }
	glm::vec4 getColor() const { return color; }

protected:
	glm::vec4 color;

	glm::vec2 v1;
	glm::vec2 v2;
	glm::vec2 normal;

};