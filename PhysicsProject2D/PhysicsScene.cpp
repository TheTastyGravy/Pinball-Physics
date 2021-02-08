#include "PhysicsScene.h"
#include "PhysicsObject.h"
#include "Rigidbody.h"
#include "Sphere.h"
#include "Plane.h"
#include <iostream>

// Function pointer array for handling our collisions
typedef bool(*fn)(PhysicsObject*, PhysicsObject*);
static fn collisioFunctionArray[] =
{
	PhysicsScene::Plane2Plane, PhysicsScene::Plane2Sphere, PhysicsScene::Sphere2Plane, PhysicsScene::Sphere2Sphere
};


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

		checkForCollision();
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


void PhysicsScene::checkForCollision()
{
	int actorCount = actors.size();

	for (int i = 0; i < actorCount - 1; i++)
	{
		for (int j = i + 1; j < actorCount; j++)
		{
			PhysicsObject* obj1 = actors[i];
			PhysicsObject* obj2 = actors[j];

			int shapeID1 = obj1->getShapeID();
			int shapeID2 = obj2->getShapeID();

			int funcIndex = (shapeID1 * SHAPE_COUNT) + shapeID2;
			fn collFuncPtr = collisioFunctionArray[funcIndex];

			if (collFuncPtr != nullptr)
			{
				collFuncPtr(obj1, obj2);
			}
		}
	}
}


bool PhysicsScene::Plane2Plane(PhysicsObject* objPlane1, PhysicsObject* objPlane2)
{
	return false;
}

bool PhysicsScene::Plane2Sphere(PhysicsObject* objPlane, PhysicsObject* objSphere)
{
	return Sphere2Plane(objSphere, objPlane);
}

bool PhysicsScene::Sphere2Plane(PhysicsObject* objSphere, PhysicsObject* objPlane)
{
	Sphere* sphere = dynamic_cast<Sphere*>(objSphere);
	Plane* plane = dynamic_cast<Plane*>(objPlane);

	//if these have a value continue below
	if (sphere != nullptr && plane != nullptr)
	{
		//distance between the sphere and plane
		float sphereToPlane = abs(glm::dot(plane->getNormal(), sphere->getPosition()) - plane->getDistance());
		//used to determine if the sphere is moving toward its normal
		float direction = glm::dot(sphere->getVelocity(), plane->getNormal());

		if (sphereToPlane < sphere->getRadius() && direction < 0)
		{
			glm::vec2 newVel = sphere->getVelocity() + ((-(1 + 1) * glm::dot(sphere->getVelocity(), plane->getNormal())) * plane->getNormal());
			glm::vec2 force = newVel - sphere->getVelocity();
			// Because applyForce has mass built in, the force needs it too
			force *= sphere->getMass();

			sphere->applyForce(force);
			return true;
		}
	}

	return false;
}

bool PhysicsScene::Sphere2Sphere(PhysicsObject* objSphere1, PhysicsObject* objSphere2)
{
	Sphere* sphere1 = dynamic_cast<Sphere*>(objSphere1);
	Sphere* sphere2 = dynamic_cast<Sphere*>(objSphere2);

	if (sphere1 != nullptr && sphere2 != nullptr)
	{
		float dist = glm::distance(sphere1->getPosition(), sphere2->getPosition());

		float penetration = sphere1->getRadius() + sphere2->getRadius() - dist;
		if (penetration > 0)
		{
			sphere1->resolveCollision(sphere2);
			return true;
		}
	}

	return false;
}