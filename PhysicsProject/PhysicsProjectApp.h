#pragma once

#include "Application.h"
#include <glm/mat4x4.hpp>

class PhysicsProjectApp : public aie::Application {
public:

	PhysicsProjectApp();
	virtual ~PhysicsProjectApp();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

protected:

	// camera transforms
	glm::mat4	m_viewMatrix;
	glm::mat4	m_projectionMatrix;

	// variables to move box with input
	glm::vec3 boxPos;
	float speed = 3;
};