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

	// colors to use for objects
	static const glm::vec4 colors[] = {
		glm::vec4(1, 0, 0, 1),			//red
		glm::vec4(0, 1, 0, 1),			//green
		glm::vec4(0, 0, 1, 1),			//blue
		glm::vec4(0.8f, 0, 0.5f, 1),	//something
		glm::vec4(0, 1, 1, 1)			//something
	};


	static const int rows = 5;
	static const int columns = 10;
	static const int hSpace = 1;
	static const int vSpace = 1;

	static const glm::vec2 scrExtents(100, 50);
	static const glm::vec2 boxExtents(7, 3);
	static const glm::vec2 startPos(-(columns >> 1) * ((boxExtents.x * 2) + vSpace) + boxExtents.x + (vSpace / 2.0f), 
									scrExtents.y - ((boxExtents.y * 2) + hSpace));


	glm::vec2 pos;
	for (int y = 0; y < rows; y++)
	{
		pos = glm::vec2(startPos.x, startPos.y - (y * ((boxExtents.y * 2) + hSpace)));

		for (int x = 0; x < columns; x++)
		{
			aie::Gizmos::add2DAABBFilled(pos, boxExtents, colors[y]);

			pos.x += (boxExtents.x * 2) + vSpace;
		}

	}

	aie::Gizmos::add2DCircle(glm::vec2(0, -35), 3, 12, glm::vec4(1, 1, 0, 1));

	aie::Gizmos::add2DAABBFilled(glm::vec2(0, -40), glm::vec2(12, 2), glm::vec4(1, 0, 1, 1));



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