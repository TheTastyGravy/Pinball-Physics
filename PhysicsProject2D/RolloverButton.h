#pragma once
#include "Box.h"
#include <vector>

class RolloverButton : public Box
{
public:
	RolloverButton(glm::vec2 position, float rotation, float width, float height, glm::vec4 color = glm::vec4(1, 1, 0, 0.5f));
	~RolloverButton() {};

	virtual void makeGizmo();

	// Create the lambda function for onTriggerEnter
	// 'group' is the set of buttons that, once all triggered, will add score
	void createTriggerFunction(int& score, std::vector<RolloverButton*> group);

protected:
	bool hasBeenTriggered;

};