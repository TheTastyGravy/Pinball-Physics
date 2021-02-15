#include "RolloverButton.h"
#include <Gizmos.h>

RolloverButton::RolloverButton(glm::vec2 position, float rotation, float width, float height, glm::vec4 color) :
	Box(position, glm::vec2(0), rotation, 1, width, height, 1, 1, 1, color)
{
	this->hasBeenTriggered = false;

	this->setKinematic(true);
	this->setTrigger(true);
}


void RolloverButton::makeGizmo()
{
	//when not triggered, it should be dark grey
	glm::vec4 drawColor = hasBeenTriggered ? color : glm::vec4(0.3f, 0.3f, 0.3f, 0.5f);


	glm::vec2 p1 = position - localX * extents.x - localY * extents.y;
	glm::vec2 p2 = position + localX * extents.x - localY * extents.y;
	glm::vec2 p3 = position - localX * extents.x + localY * extents.y;
	glm::vec2 p4 = position + localX * extents.x + localY * extents.y;

	aie::Gizmos::add2DTri(p1, p2, p4, drawColor);
	aie::Gizmos::add2DTri(p1, p4, p3, drawColor);
}

void RolloverButton::createTriggerFunction(int& score, std::vector<RolloverButton*> group)
{
	onTriggerEnter = [this, &score, group](PhysicsObject* other)
	{
		this->hasBeenTriggered = true;

		//check if all are triggered
		bool allTriggered = true;
		for (auto it : group)
		{
			if (!it->hasBeenTriggered)
			{
				allTriggered = false;
				break;
			}
		}


		//add score and reset all buttons
		if (allTriggered)
		{
			score += 500;

			for (auto it : group)
			{
				it->hasBeenTriggered = false;
			}
		}
	};
}