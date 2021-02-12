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
#include "Bouncer.h"
#include "Flipper.h"


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
	physicsScene->setGravity(glm::vec2(0, -30));

	
	//drawRect();
	//ballsInBox();
	//springTest(10);
	//triggerTest();
	//springLauncher();
	pinball();


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


	//pull spring back with 'space'
	if (input->isKeyDown(aie::INPUT_KEY_SPACE))
	{
		launchSpring->setRestLength(launchSpring->getRestLength() - 10.0f * deltaTime);
	}
	if (input->wasKeyReleased(aie::INPUT_KEY_SPACE))
	{
		launchSpring->setRestLength(defaultRestLength);
	}

	//set flipper input
	leftFlipper->setKeyDown(input->isKeyDown(aie::INPUT_KEY_A));
	rightFlipper->setKeyDown(input->isKeyDown(aie::INPUT_KEY_D));

	//reset ball with 'r'
	if (input->wasKeyPressed(aie::INPUT_KEY_R))
	{
		if (!gameOver)
		{
			resetBall();
		}
		//if the game is over, restart game
		else
		{
			physicsScene->resetScene();
			pinball();
		}
		
	}



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

	// When extents = 100: X-axis = -100 to 100, Y-axis = -56.25 to 56.25
	aie::Gizmos::draw2D(glm::ortho<float>(-extents, extents, -extents / aspectRatio, extents / aspectRatio, -1.0f, 1.0f));


	// draw your stuff here!



	//show FPS
	char fps[32];
	sprintf_s(fps, 32, "FPS: %i", getFPS());
	m_2dRenderer->drawText(m_font, fps, 0, 720 - 32);

	//show score
	char scoreText[32];
	sprintf_s(scoreText, 32, "Score: %i", score);
	m_2dRenderer->drawText(m_font, scoreText, 0, 720 - 32 * 2 - 10);
	//show balls
	char ballText[32];
	sprintf_s(ballText, 32, "Balls Remaining: %i", ballsRemaining);
	m_2dRenderer->drawText(m_font, ballText, 0, 720 - 32 * 3 - 10);


	// output some text, uses the last used colour
	m_2dRenderer->drawText(m_font, "Press ESC to quit", 0, 0);

	// done drawing sprites
	m_2dRenderer->end();
}


glm::vec2 PhysicsProject2DApp::screen2World(glm::vec2 screenPos) const
{
	glm::vec2 worldPos = screenPos;

	// We will move the center of the screen to (0,0)
	worldPos.x -= getWindowWidth() / 2.0f;
	worldPos.y -= getWindowHeight() / 2.0f;

	// Scale this according to the extensts
	worldPos.x *= 2.0f * extents / getWindowWidth();
	worldPos.y *= 2.0f * extents / aspectRatio / getWindowHeight();

	return worldPos;
}


void PhysicsProject2DApp::resetBall()
{
	if (ballsRemaining > 0)
	{
		ballsRemaining--;

		ball->setPosition(glm::vec2(44, -23));
		ball->resetVelocity();
	}
	else
	{
		//game over
		gameOver = true;

		//removing the ball from the game causes problems, so just move it out of the scene
		ball->setPosition(glm::vec2(200, -100));
	}
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

void PhysicsProject2DApp::triggerTest()
{
	Sphere* ball1 = new Sphere(glm::vec2(-20, 0), glm::vec2(0), 4, 4);
	Sphere* ball2 = new Sphere(glm::vec2(10, -20), glm::vec2(0), 4, 4);
	ball2->setKinematic(true);
	ball2->setTrigger(true);

	physicsScene->addActor(ball1);
	physicsScene->addActor(ball2);

	//box
	Plane* plane1 = new Plane(glm::vec2(0.0f, 1.0f), -50);
	physicsScene->addActor(plane1);
	Plane* plane2 = new Plane(glm::vec2(0.0f, -1.0f), -50);
	physicsScene->addActor(plane2);
	Plane* plane3 = new Plane(glm::vec2(1.0f, 0.0f), -70);
	physicsScene->addActor(plane3);
	Plane* plane4 = new Plane(glm::vec2(-1.0f, 0.0f), -70);
	physicsScene->addActor(plane4);


	physicsScene->addActor(new Box(glm::vec2(20, 10), glm::vec2(3, 0), 0, 4, 8, 3));
	physicsScene->addActor(new Box(glm::vec2(-40, 10), glm::vec2(3, 0), 0, 4, 8, 3));

	//display when an object enters or exits the trigger
	ball2->onTriggerEnter = [=](PhysicsObject* other) { std::cout << "Entered: " << other << std::endl; };
	ball2->onTriggerExit = [=](PhysicsObject* other) { std::cout << "Exited: " << other << std::endl; };
}

void PhysicsProject2DApp::springLauncher()
{
	Plane* plane1 = new Plane(glm::vec2(1, 0), -8);
	physicsScene->addActor(plane1);
	Plane* plane2 = new Plane(glm::vec2(-1, 0), -8);
	physicsScene->addActor(plane2);


	Sphere* ball = new Sphere(glm::vec2(0, 3), glm::vec2(0), 5, 7);
	physicsScene->addActor(ball);


	Box* springBase = new Box(glm::vec2(0, -50), glm::vec2(0), 0, 10, 8, 3);
	springBase->setKinematic(true);
	physicsScene->addActor(springBase);

	Box* springTop = new Box(glm::vec2(0, -40), glm::vec2(0), 0, 5, 8, 3, 0.3f);
	physicsScene->addActor(springTop);

	physicsScene->addActor(new Spring(springBase, springTop, 10, 400, 50));


	Box* stopper1 = new Box(glm::vec2(7, 0), glm::vec2(0), 0, 10, 1, 1, 0);
	stopper1->setKinematic(true);
	physicsScene->addActor(stopper1);
	Box* stopper2 = new Box(glm::vec2(-7, 0), glm::vec2(0), 0, 10, 1, 1, 0);
	stopper2->setKinematic(true);
	physicsScene->addActor(stopper2);
}

void PhysicsProject2DApp::pinball()
{
	score = 0;
	ballsRemaining = 3;
	gameOver = false;


	const float pi = 3.1415;
	//bounce objects apply a force and add score
	std::function<void(PhysicsObject*)> addScoreFunc = [this](PhysicsObject*) { score += 100; };


	//basic shape
	{
		//outer walls
		Box* box = new Box(glm::vec2(50, 0), glm::vec2(0), 0, 10, 3, 60);
		box->setKinematic(true);
		physicsScene->addActor(box);
		box = new Box(glm::vec2(-50, 0), glm::vec2(0), 0, 10, 3, 60);
		box->setKinematic(true);
		physicsScene->addActor(box);
		box = new Box(glm::vec2(0, 50), glm::vec2(0), pi * 0.5f, 10, 3, 60);
		box->setKinematic(true);
		physicsScene->addActor(box);
		box = new Box(glm::vec2(0, -50), glm::vec2(0), pi * 0.5f, 10, 3, 60);
		box->setKinematic(true);
		physicsScene->addActor(box);

		//seperating wall for spring
		box = new Box(glm::vec2(40, -10), glm::vec2(0), 0, 10, 1, 40);
		box->setKinematic(true);
		physicsScene->addActor(box);
		//angle for ball to bounce off
		box = new Box(glm::vec2(45, 40), glm::vec2(0), pi * 0.25f, 10, 1, 13);
		box->setKinematic(true);
		physicsScene->addActor(box);
	}

	//details
	{
		//angles by flippers
		Box* box = new Box(glm::vec2(24, -25), glm::vec2(0), pi * -0.35f, 10, 1, 18, 0.f);
		box->setKinematic(true);
		physicsScene->addActor(box);
		box = new Box(glm::vec2(-32, -25), glm::vec2(0), pi * 0.35f, 10, 1, 18, 0.f);
		box->setKinematic(true);
		physicsScene->addActor(box);
		
		//left triangle
		box = new Box(glm::vec2(20, -18), glm::vec2(0), pi * -0.3f, 10, 1, 12, 0.5f);
		box->setKinematic(true);
		physicsScene->addActor(box);
		box = new Box(glm::vec2(15, -16), glm::vec2(0), pi * -0.15f, 10, 0.5f, 10, 1.7f, 0, 0, glm::vec4(0, 1, 0, 1));
		box->setKinematic(true);
		box->collisionCallback = addScoreFunc;	//bouncy wall gives score
		physicsScene->addActor(box);
		box = new Box(glm::vec2(24.75f, -9.5f), glm::vec2(0), pi * -0.62f, 10, 1, 5.75f, 0.5f);
		box->setKinematic(true);
		physicsScene->addActor(box);
		//right triangle
		box = new Box(glm::vec2(-28, -18), glm::vec2(0), pi * 0.3f, 10, 1, 12, 0.5f);
		box->setKinematic(true);
		physicsScene->addActor(box);
		box = new Box(glm::vec2(-23, -16), glm::vec2(0), pi * 0.15f, 10, 0.5f, 10, 1.7f, 0, 0, glm::vec4(0, 1, 0, 1));
		box->setKinematic(true);
		box->collisionCallback = addScoreFunc;	//bouncy wall gives score
		physicsScene->addActor(box);
		box = new Box(glm::vec2(-32.75f, -9.5f), glm::vec2(0), pi * 0.62f, 10, 1, 5.75f, 0.5f);
		box->setKinematic(true);
		physicsScene->addActor(box);

	}

	//spring
	{
		Box* springBase = new Box(glm::vec2(44, -50), glm::vec2(0), 0, 10, 3, 1);
		springBase->setKinematic(true);
		physicsScene->addActor(springBase);

		Box* springTop = new Box(glm::vec2(44, -40), glm::vec2(0), 0, 5, 2.95f, 1, 0.8f);
		springTop->setRotationLock(true);
		physicsScene->addActor(springTop);

		defaultRestLength = 25;
		launchSpring = new Spring(springBase, springTop, 10, 300, defaultRestLength);
		physicsScene->addActor(launchSpring);


		Box* stopper1 = new Box(glm::vec2(47, -25), glm::vec2(0), 0, 10, 1.65f, 0.3f, 0);
		stopper1->setKinematic(true);
		physicsScene->addActor(stopper1);
		Box* stopper2 = new Box(glm::vec2(41, -25), glm::vec2(0), 0, 10, 1.65f, 0.3f, 0);
		stopper2->setKinematic(true);
		physicsScene->addActor(stopper2);
	}

	//bouncers
	Bouncer* bouncer = new Bouncer(glm::vec2(-30, 10), 3, 1, score, glm::vec4(1, 1, 0, 1));
	physicsScene->addActor(bouncer);
	bouncer = new Bouncer(glm::vec2(-22, 18), 3, 1, score, glm::vec4(1, 1, 0, 1));
	physicsScene->addActor(bouncer);
	bouncer = new Bouncer(glm::vec2(-18, 6), 3, 1, score, glm::vec4(1, 1, 0, 1));
	physicsScene->addActor(bouncer);

	//trigger for game over
	Box* trigger = new Box(glm::vec2(0, -43), glm::vec2(0), 0, 1, 20, 3, 0, 0, 0, glm::vec4(1, 0, 0, 0.5f));
	trigger->setKinematic(true);
	trigger->setTrigger(true);
	// When the ball enters the trigger, reset it
	trigger->onTriggerEnter = [this](PhysicsObject* other) { if (other == ball) { resetBall(); } };
	physicsScene->addActor(trigger);


	//start ball above spring
	ball = new Sphere(glm::vec2(44, -23), glm::vec2(0), 1, 1.5f, 0.7f, 0.2f, 0.2f, glm::vec4(0.7f, 0.7f, 0.7f, 1));
	physicsScene->addActor(ball);

	//flippers
	leftFlipper = new Flipper(glm::vec2(-12, -35.15f), pi * 0.35f, 10, 1, 5, glm::vec2(-3.3f, 2.3f), true, 0.2f);
	physicsScene->addActor(leftFlipper);
	rightFlipper = new Flipper(glm::vec2(4, -35.15f), pi * -0.35f, 10, 1, 5, glm::vec2(3.3f, 2.3f), false, 0.2f);
	physicsScene->addActor(rightFlipper);
}