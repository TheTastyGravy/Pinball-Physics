#include "Line.h"
#include <Gizmos.h>

#include <iostream>

Line::Line(glm::vec2 v1, glm::vec2 v2, float elasticity, glm::vec4 color) :
	PhysicsObject(LINE, elasticity)
{
	this->v1 = v1;
	this->v2 = v2;
	this->color = color;

	this->setKinematic(true);
	this->setRotationLock(true);

	//the normal of the line is the perpendicular of v1 - v2
	glm::vec2 norm = glm::normalize(v1 - v2);
	this->normal = glm::vec2(-norm.y, norm.x);
}


void Line::fixedUpdate(glm::vec2 gravity, float timeStep)
{
}

void Line::makeGizmo()
{
	glm::vec4 colorFade = color;
	colorFade.a = 0;

	//draw lines
	aie::Gizmos::add2DTri(v1, v2, v1 - getNormal() * 5.0f, getColor(), getColor(), colorFade);
	aie::Gizmos::add2DTri(v2, v2 - getNormal() * 5.0f, v1 - getNormal() * 5.0f, getColor(), colorFade, colorFade);
}


void Line::resolveCollision(Rigidbody* otherActor, glm::vec2 contact)
{
	std::cout << "hit" << std::endl;
}
