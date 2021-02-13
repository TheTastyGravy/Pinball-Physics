#include "Flipper.h"


Flipper::Flipper(glm::vec2 position, float rotation, float mass, float width, float height, glm::vec2 rotationPoint, bool isLeftSide, float elasticity, glm::vec4 color) :
	Box(position, glm::vec2(0), rotation, mass, width, height, elasticity, 0, 0, color)
{
	this->rotationPoint = position + rotationPoint;
	this->startRot = rotation;
	this->isLeftSide = isLeftSide;

	this->keyDown = false;
	this->isKinematic = true;
	this->lockRotation = false;

	//moment with the center of mass at the edge of a box
	this->moment = (1.f / 12.f) * mass * (4 * height * height + width * width);
}


void Flipper::fixedUpdate(glm::vec2 gravity, float timestep)
{
	const float pi = 3.1415;

	// Move flipper up when key is down
	if (keyDown)
	{
		if (isLeftSide)
		{
			// Flipper is not at the max rotation yet
			if (rotation < startRot + pi * 0.25f)
			{
				angularVelocity = 10;
			}
			// The flipper cant rotate further
			else
			{
				angularVelocity = 0;
			}
		}

		else if (!isLeftSide)
		{
			if (rotation > startRot - pi * 0.25f)
			{
				angularVelocity = -10;
			}
			else
			{
				angularVelocity = 0;
			}
		}
	}
	// Move flipper back to default position
	else
	{
		if (isLeftSide)
		{
			if (rotation > startRot)
			{
				angularVelocity = -10;
			}
			else
			{
				angularVelocity = 0;
			}
		}

		else if (!isLeftSide)
		{
			if (rotation < startRot)
			{
				angularVelocity = 10;
			}
			else
			{
				angularVelocity = 0;
			}
		}
	}


	Box::fixedUpdate(gravity, timestep);


	// Rotate the object around the rotation point
	//https://stackoverflow.com/questions/2259476/rotating-a-point-about-another-point-2d
	float s = sinf(angularVelocity * timestep);
	float c = cosf(angularVelocity * timestep);

	position -= rotationPoint;
	glm::vec2 rotatedPos;

	rotatedPos.x = position.x * c - position.y * s;
	rotatedPos.y = position.x * s + position.y * c;

	position = rotatedPos + rotationPoint;
}