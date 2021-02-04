#include "PhysicsScene.h"
#include "PhysicsObject.h"
#include "Rigidbody.h"
#include <iostream>

#include <list>


PhysicsScene::PhysicsScene() :
	timeStep(0.01f),
	gravity(glm::vec2(0, 0))
{
}

PhysicsScene::~PhysicsScene()
{
	for (auto actor : actors)
	{
		delete actor;
	}
}


void PhysicsScene::addActor(PhysicsObject* actor)
{
	actors.push_back(actor);
}

void PhysicsScene::removeActor(PhysicsObject* actor)
{
	// find actor, then remove it from the vector
	auto it = std::find(actors.begin(), actors.end(), actor);
	if (it != actors.end())
	{
		actors.erase(it);
	}
}


void PhysicsScene::update(float deltaTime)
{
	static std::list<PhysicsObject*> dirty; //remove

	// static variable will maintain its value between calls
	static float accumulatedTime = 0.0f;
	accumulatedTime += deltaTime;

	// while loop is used because it could be possible to have multiple timesteps in one update loop
	while (accumulatedTime >= timeStep)
	{
		for (auto actor : actors)
		{
			actor->fixedUpdate(gravity, timeStep);
		}

		accumulatedTime -= timeStep;

		// Check for collisions
		for (auto actor : actors)
		{
			for (auto other : actors)
			{
				// dont check for collsion with itself
				if (actor == other)
				{
					continue;
				}

				// if objects are dirty, dont check them
				if (std::find(dirty.begin(), dirty.end(), actor) != dirty.end() &&
					std::find(dirty.begin(), dirty.end(), other) != dirty.end())
				{
					continue;
				}

				// if objects are colliding, apply force and make them dirty
				Rigidbody* rigid1 = dynamic_cast<Rigidbody*>(actor);
				Rigidbody* rigid2 = dynamic_cast<Rigidbody*>(other);
				if (rigid1->checkCollision(other))
				{
					rigid1->applyForceToOther(rigid2, rigid1->getVelocity() * rigid1->getMass());
					dirty.push_back(rigid1);
					dirty.push_back(rigid2);
				}
			}
		}

		dirty.clear();
	}
}

void PhysicsScene::draw()
{
	for (auto actor : actors)
	{
		actor->makeGizmo();
	}
}


void PhysicsScene::debugScene()
{
	int count = 0;

	for (auto actor : actors)
	{
		std::cout << count << " : ";
		actor->debug();

		count++;
	}
}