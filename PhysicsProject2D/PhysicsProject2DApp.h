#pragma once

#include "Application.h"
#include "Renderer2D.h"
#include "PhysicsScene.h"

class Spring;
class Flipper;
class Sphere;

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

	void resetBall();

protected:
	const float aspectRatio = 16.0f / 9.0f;
	const float extents = 100;

	aie::Renderer2D*	m_2dRenderer;
	aie::Font*			m_font;

	PhysicsScene* physicsScene;


	Spring* launchSpring;
	float defaultRestLength;

	Sphere* ball;

	Flipper* leftFlipper;
	Flipper* rightFlipper;

	int score;
	int ballsRemaining;

public:
	void drawRect();
	void ballsInBox();
	void springTest(int amount);
	void triggerTest();
	void springLauncher();
	void pinball();

};