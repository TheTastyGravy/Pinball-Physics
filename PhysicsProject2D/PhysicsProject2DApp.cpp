#include "PhysicsProject2DApp.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include "glm\ext.hpp"
#include "Gizmos.h"
#include "Sphere.h"
#include "Plane.h"
#include "Box.h"
#include "Spring.h"


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

	physicsScene = new PhysicsScene();
	// A smaller timestep will give a more accurate simulation, at the cost of speed
	physicsScene->setTimeStep(0.01f);
	physicsScene->setGravity(glm::vec2(0, -10));


	drawRect();
	//ballsInBox();
	//springTest(10);


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


	aie::Gizmos::clear();
	physicsScene->update(deltaTime);
	physicsScene->draw();


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


void PhysicsProject2DApp::drawRect()
{
	float linDrag = 0.1f;
	float angDrag = 0.1f;

	physicsScene->addActor(new Sphere(glm::vec2(20, 10), glm::vec2(-10, -17), 1, 3, 1, linDrag, angDrag, glm::vec4(1, 0, 0, 1)));
	physicsScene->addActor(new Plane(glm::vec2(-0.65, 0.75), -25, 0.5f));
	physicsScene->addActor(new Plane(glm::vec2(0.05, 1), -25, 0.5f));

	Box* box1 = new Box(glm::vec2(-20, 0), glm::vec2(16, -4), 0, 4, 8, 4, 1, linDrag, angDrag, glm::vec4(0, 0, 1, 1));
	box1->setRotation(45.f);

	Box* box2 = new Box(glm::vec2(10, 0), glm::vec2(-4, 0), 0, 4, 8, 4, 1, linDrag, angDrag, glm::vec4(0, 0, 1, 1));


	physicsScene->addActor(box1);
	physicsScene->addActor(box2);

	box1->applyForce(glm::vec2(30, 0), glm::vec2(0));
	box2->applyForce(glm::vec2(-15, 0), glm::vec2(0));

	Sphere* ball = new Sphere(glm::vec2(5, -10), glm::vec2(5, 5), 1, 3, 1, linDrag, angDrag, glm::vec4(1, 0, 0, 1));
	ball->setRotation(0.5f);
	physicsScene->addActor(ball);
}

void PhysicsProject2DApp::ballsInBox()
{
	Sphere* ball1 = new Sphere(glm::vec2(-40, 20), glm::vec2(40, -0), 3.0f, 1, 1, 0, 0, glm::vec4(1, 0, 0, 1));
	physicsScene->addActor(ball1);
	Sphere* ball2 = new Sphere(glm::vec2(40, 20), glm::vec2(-40, -0), 3.0f, 1, 1, 0, 0, glm::vec4(1, 1, 0, 1));
	physicsScene->addActor(ball2);
	Sphere* ball3 = new Sphere(glm::vec2(0, 60), glm::vec2(0, -40), 10.0f, 5, 1, 0, 0, glm::vec4(1, 0, 1, 1));
	physicsScene->addActor(ball3);
	Sphere* ball4 = new Sphere(glm::vec2(15, -50), glm::vec2(20, 30), 3.0f, 1, 1, 0, 0, glm::vec4(0, 0, 1, 1));
	physicsScene->addActor(ball4);


	Plane* plane1 = new Plane(glm::vec2(0.0f, 1.0f), -50);
	physicsScene->addActor(plane1);
	Plane* plane2 = new Plane(glm::vec2(0.0f, -1.0f), -50);
	physicsScene->addActor(plane2);
	Plane* plane3 = new Plane(glm::vec2(1.0f, 0.0f), -70);
	physicsScene->addActor(plane3);
	Plane* plane4 = new Plane(glm::vec2(-1.0f, 0.0f), -70);
	physicsScene->addActor(plane4);

	Plane* plane5 = new Plane(glm::vec2(1.0f, 1.0f), -60);
	physicsScene->addActor(plane5);
	Plane* plane6 = new Plane(glm::vec2(-1.0f, -1.0f), -60);
	physicsScene->addActor(plane6);
	Plane* plane7 = new Plane(glm::vec2(1.0f, -1.0f), -60);
	physicsScene->addActor(plane7);
	Plane* plane8 = new Plane(glm::vec2(-1.0f, 1.0f), -60);
	physicsScene->addActor(plane8);
}

void PhysicsProject2DApp::springTest(int amount)
{
	Sphere* prev = nullptr;
	for (int i = 0; i < amount; i++)
	{
		// This will need to spawn the sphere at the bottom of the previous one, to
		// make a pendulum that is effected by gravity
		Sphere* sphere = new Sphere(glm::vec2(i * 5, 30 - i * 3), glm::vec2(0), 5, 2, 1, 0, 0, glm::vec4(0, 0, 1, 1));

		if (i == 0)
		{
			sphere->setKinematic(true);
		}

		physicsScene->addActor(sphere);

		if (prev)
		{
			physicsScene->addActor(new Spring(sphere, prev, 10, 500));
		}

		prev = sphere;
	}

	Box* box = new Box(glm::vec2(0, -20), glm::vec2(0), 0.5f, 20, 8, 2);
	box->setKinematic(true);
	physicsScene->addActor(box);
}