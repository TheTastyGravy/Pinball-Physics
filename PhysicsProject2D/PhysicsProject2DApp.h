#pragma once

#include "Application.h"
#include "Renderer2D.h"
#include "PhysicsScene.h"

class PhysicsProject2DApp : public aie::Application
{
public:
	PhysicsProject2DApp();
	virtual ~PhysicsProject2DApp();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

	glm::vec2 screen2World(glm::vec2 screenPos) const;

protected:
	const float aspectRatio = 16.0f / 9.0f;
	const float extents = 100;

	aie::Renderer2D*	m_2dRenderer;
	aie::Font*			m_font;

	PhysicsScene* physicsScene;

public:
	void drawRect();
	void ballsInBox();
	void springTest(int amount);
	void triggerTest();

};