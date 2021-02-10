#include "Box.h"
#include <Gizmos.h>


Box::Box(glm::vec2 position, glm::vec2 velocity, float rotation, float mass, float width, float height, float elasticity, float linearDrag, float angularDrag, glm::vec4 color) :
	Rigidbody(BOX, position, velocity, rotation, mass, elasticity, linearDrag, angularDrag), extents(width, height)
{
	this->color = color;
	this->moment = (1.0f / 3.0f) * mass * width * height;

	this->localX = glm::vec2();
	this->localY = glm::vec2();
}

Box::~Box()
{
}


void Box::fixedUpdate(glm::vec2 gravity, float timestep)
{
	Rigidbody::fixedUpdate(gravity, timestep);

	float cs = cosf(rotation);
	float sn = sinf(rotation);

	localX = glm::normalize(glm::vec2(cs, sn));
	localY = glm::normalize(glm::vec2(-sn, cs));
}

void Box::makeGizmo()
{
	glm::vec2 p1 = position - localX * extents.x - localY * extents.y;
	glm::vec2 p2 = position + localX * extents.x - localY * extents.y;
	glm::vec2 p3 = position - localX * extents.x + localY * extents.y;
	glm::vec2 p4 = position + localX * extents.x + localY * extents.y;

	aie::Gizmos::add2DTri(p1, p2, p4, color);
	aie::Gizmos::add2DTri(p1, p4, p3, color);
}


bool Box::checkBoxCorners(const Box& box, glm::vec2& contact, int& numContacts, float& pen, glm::vec2& edgeNormal)
{
	float minX, minY, maxX, maxY;

	float boxW = box.getWidth() * 2.f;
	float boxH = box.getHeight() * 2.f;

	int numLocalContacts = 0;
	glm::vec2 localContact(0, 0);
	bool first = true;


	for (float x = -box.getExtents().x; x < boxW; x += boxW)
	{
		for (float y = -box.getExtents().y; y < boxH; y += boxH)
		{
			// Get the position in world space
			glm::vec2 p = box.getPosition() + x * box.getLocalX() + y * box.getLocalY();
			// Get the position in our box's space
			glm::vec2 p0(glm::dot(p - position, localX), glm::dot(p - position, localY));

			// Update the extents in each cardinal direction of our box's space
			// ~ Extents along the seperating axes
			if (first || p0.x < minX)
			{
				minX = p0.x;
			}
			if (first || p0.x > maxX)
			{
				maxX = p0.x;
			}
			if (first || p0.y < minY)
			{
				minY = p0.y;
			}
			if (first || p0.y > maxY)
			{
				maxY = p0.y;
			}


			if (p0.x >= -extents.x && p0.x <= extents.x &&
				p0.y >= -extents.y && p0.y <= extents.y)
			{
				numLocalContacts++;
				localContact += p0;
			}

			first = false;
		}
	}


	// If we lie entierly to one side of the box along one axis, weve found a seperating
	// axis, and we can exit

	if (maxX <= -extents.x || minX >= extents.x ||
		maxY <= -extents.y || minY >= extents.y)
	{
		return false;
	}

	if (numLocalContacts == 0)
	{
		return false;
	}

	bool res = false;
	contact += position + (localContact.x * localX + localContact.y * localY) / (float)numLocalContacts;
	numContacts++;

	// Find the minimum penetration vector as a penetration amount and normal
	float pen0 = extents.x - minX;
	if (pen0 > 0 && (pen0 < pen || pen == 0))
	{
		edgeNormal = localX;
		pen = pen0;
		res = true;
	}
	pen0 = maxX + extents.x;
	if (pen0 > 0 && (pen0 < pen || pen == 0))
	{
		edgeNormal = -localX;
		pen = pen0;
		res = true;
	}

	pen0 = extents.y - minY;
	if (pen0 > 0 && (pen0 < pen || pen == 0))
	{
		edgeNormal = localY;
		pen = pen0;
		res = true;
	}
	pen0 = maxY + extents.y;
	if (pen0 > 0 && (pen0 < pen || pen == 0))
	{
		edgeNormal = -localY;
		pen = pen0;
		res = true;
	}


	return res;
}