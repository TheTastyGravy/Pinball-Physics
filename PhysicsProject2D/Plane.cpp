#include "Plane.h"
#include <Gizmos.h>

Plane::Plane() : PhysicsObject(PLANE)
{
	this->normal = glm::vec2(0, 1);
	this->distanceToOrigin = 0.0f;
	this->color = glm::vec4(1);
}

Plane::Plane(glm::vec2 normal, float distance) : PhysicsObject(PLANE)
{
	this->normal = glm::normalize(normal);
	this->distanceToOrigin = distance;
	this->color = glm::vec4(0, 1, 0, 1);
}

Plane::~Plane()
{
}


void Plane::fixedUpdate(glm::vec2 gravity, float timeStep)
{
}

void Plane::makeGizmo()
{
	float lineSegmentLength = 300.0f;
	glm::vec2 centerPoint = normal * distanceToOrigin;
	glm::vec2 parallel(normal.y, -normal.x);
	glm::vec4 colorFade = color;
	colorFade.a = 0;

	glm::vec2 start = centerPoint + (parallel * lineSegmentLength);
	glm::vec2 end = centerPoint - (parallel * lineSegmentLength);

	//draw lines
	aie::Gizmos::add2DTri(start, end, start - getNormal() * 5.0f, getColor(), getColor(), colorFade);
	aie::Gizmos::add2DTri(end, end - getNormal() * 5.0f, start - getNormal() * 5.0f, getColor(), colorFade, colorFade);
}