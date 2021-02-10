#pragma once
#include "Rigidbody.h"
class Box : public Rigidbody
{
public:
	Box(glm::vec2 position, glm::vec2 velocity, float rotation, float mass, float width, float height);
	Box(glm::vec2 position, glm::vec2 velocity, float rotation, float mass, float width, float height, glm::vec4 color);
	~Box();

	virtual void fixedUpdate(glm::vec2 gravity, float timestep);
	virtual void makeGizmo();

	bool CheckBoxCorners(const Box& box, glm::vec2& contact, int& numContacts, float& pen, glm::vec2& edgeNormal);

	float getWidth() const { return extents.x; }
	float getHeight() const { return extents.y; }
	glm::vec2 getExtents() const { return extents; }

	glm::vec2 getLocalX() const { return localX; }
	glm::vec2 getLocalY() const { return localY; }

protected:
	// The half extents of the box
	glm::vec2 extents;
	glm::vec4 color;

	// These will store the local x and y axes of the box based on its angle of rotation
	glm::vec2 localX;
	glm::vec2 localY;

};