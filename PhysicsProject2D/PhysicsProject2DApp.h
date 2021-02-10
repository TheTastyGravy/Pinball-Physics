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

protected:
	aie::Renderer2D*	m_2dRenderer;
	aie::Font*			m_font;

	PhysicsScene* physicsScene;

public:
	void drawRect();
	void ballsInBox();

};