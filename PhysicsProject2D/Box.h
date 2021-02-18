#pragma once
#include "Rigidbody.h"
class Box : public Rigidbody
{
public:
	Box(glm::vec2 position, glm::vec2 velocity, float rotation, float mass, float width, float height, float elasticity = 1.0f, float linearDrag = 0.0f, float angularDrag = 0.0f, glm::vec4 color = glm::vec4(1, 0, 0, 1));
	~Box();

	virtual void fixedUpdate(glm::vec2 gravity, float timestep);
	virtual void makeGizmo();

	bool checkBoxCorners(const Box& box, glm::vec2& contact, int& numContacts, float& pen, glm::vec2& edgeNormal);

	float getWidth() const { return extents.x; }
	float getHeight() const { return extents.y; }
	glm::vec2 getExtents() const { return extents; }

protected:
	// The half extents of the box
	glm::vec2 extents;
	glm::vec4 color;

};