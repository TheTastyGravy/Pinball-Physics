#include "PhysicsProject2DApp.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include "glm\ext.hpp"
#include "Gizmos.h"


PhysicsProject2DApp::PhysicsProject2DApp()
{

}

PhysicsProject2DApp::~PhysicsProject2DApp()
{

}


bool PhysicsProject2DApp::startup()
{
	aie::Gizmos::create(255U, 255U, 65535U, 65535U);

	m_2dRenderer = new aie::Renderer2D();

	// TODO: remember to change this when redistributing a build!
	// the following path would be used instead: "./font/consolas.ttf"
	m_font = new aie::Font("../bin/font/consolas.ttf", 32);

	return true;
}

void PhysicsProject2DApp::shutdown()
{
	delete m_font;
	delete m_2dRenderer;
}


void PhysicsProject2DApp::update(float deltaTime)
{
	// input example
	aie::Input* input = aie::Input::getInstance();

	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
	{
		quit();
	}
}

void PhysicsProject2DApp::draw()
{
	// wipe the screen to the background colour
	clearScreen();

	// begin drawing sprites
	m_2dRenderer->begin();


	// draw your stuff here!

	static float aspectRatio = 16.0f / 9.0f;

	aie::Gizmos::draw2D(glm::ortho<float>(-100, 100, -100 / aspectRatio, 100 / aspectRatio, -1.0f, 1.0f));



	
	// output some text, uses the last used colour
	m_2dRenderer->drawText(m_font, "Press ESC to quit", 0, 0);

	// done drawing sprites
	m_2dRenderer->end();
}