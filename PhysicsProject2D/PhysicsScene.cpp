#include "PhysicsScene.h"
#include "PhysicsObject.h"
#include "Rigidbody.h"
#include "Sphere.h"
#include "Plane.h"
#include "Box.h"
#include <iostream>

// Function pointer array for handling our collisions
typedef bool(*fn)(PhysicsObject*, PhysicsObject*);
static fn collisioFunctionArray[] =
{
	PhysicsScene::Plane2Plane, PhysicsScene::Plane2Sphere, PhysicsScene::Plane2Box, 
	PhysicsScene::Sphere2Plane, PhysicsScene::Sphere2Sphere, PhysicsScene::Sphere2Box, 
	PhysicsScene::Box2Plane, PhysicsScene::Box2Sphere, PhysicsScene::Box2Box
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

bool PhysicsScene::Plane2Box(PhysicsObject* objPlane, PhysicsObject* objBox)
{
	return Box2Plane(objBox, objPlane);
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
			glm::vec2 contact = sphere->getPosition() + (plane->getNormal() * -sphere->getRadius());
			plane->resolveCollision(sphere, contact);

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

		if (dist < sphere1->getRadius() + sphere2->getRadius())
		{
			// The contact is the point between them
			sphere1->resolveCollision(sphere2, 0.5f * (sphere1->getPosition() + sphere2->getPosition()));
			return true;
		}
	}

	return false;
}

bool PhysicsScene::Sphere2Box(PhysicsObject* objSphere, PhysicsObject* objBox)
{
	return Box2Sphere(objBox, objSphere);
}

bool PhysicsScene::Box2Plane(PhysicsObject* objBox, PhysicsObject* objPlane)
{
	Box* box = dynamic_cast<Box*>(objBox);
	Plane* plane = dynamic_cast<Plane*>(objPlane);

	// If successful, we test for collision
	if (box != nullptr && plane != nullptr)
	{
		int numContacts = 0;
		glm::vec2 contact(0);
		float contactV = 0;

		// Get a representative point on the plane
		glm::vec2 planeOrigin = plane->getNormal() * plane->getDistance();

		// Check all the corners for a collision with the plane
		for (float x = -box->getExtents().x; x < box->getWidth() * 2; x += box->getWidth() * 2)
		{
			for (float y = -box->getExtents().y; y < box->getHeight() * 2; y += box->getHeight() * 2)
			{
				// Get the position of the corners in world space
				glm::vec2 p = box->getPosition() + x * box->getLocalX() + y * box->getLocalY();
				float distFromPlane = glm::dot(p - planeOrigin, plane->getNormal());

				// This is the total velocity of the points in world space
				glm::vec2 displacement = x * box->getLocalX() + y * box->getLocalY();
				glm::vec2 pointVelocity = box->getVelocity() + box->getAngularVelocity() * glm::vec2(-displacement.y, displacement.x);
				// This is the ammount of the point velocity into the plane
				float velocityIntoPlane = glm::dot(pointVelocity, plane->getNormal());

				// Moving further in, we need to resolve the collision
				if (distFromPlane < 0 && velocityIntoPlane <= 0)
				{
					numContacts++;
					contact += p;
					contactV += velocityIntoPlane;
				}
			}
		}

		if (numContacts > 0)
		{
			plane->resolveCollision(box, contact / (float)numContacts);
			return true;
		}
	}

	return false;
}

bool PhysicsScene::Box2Sphere(PhysicsObject* objBox, PhysicsObject* objSphere)
{
	Box* box = dynamic_cast<Box*>(objBox);
	Sphere* sphere = dynamic_cast<Sphere*>(objSphere);

	if (box != nullptr && sphere != nullptr)
	{
		// Transform the cirle into the box's coordinate space
		glm::vec2 circlePosWorld = sphere->getPosition() - box->getPosition();
		glm::vec2 circlePosBox = glm::vec2(glm::dot(circlePosWorld, box->getLocalX()), glm::dot(circlePosWorld, box->getLocalY()));

		// Find the closest point to the circles center on the box 
		// by clamping the coordinates in the box-space to the boxs extents
		glm::vec2 closestPointOnBox = circlePosBox;
		glm::vec2 extents = box->getExtents();
		if (closestPointOnBox.x < -extents.x)
		{
			closestPointOnBox.x = -extents.x;
		}
		if (closestPointOnBox.x > extents.x)
		{
			closestPointOnBox.x = extents.x;
		}
		if (closestPointOnBox.y < -extents.y)
		{
			closestPointOnBox.y = -extents.y;
		}
		if (closestPointOnBox.y > extents.y)
		{
			closestPointOnBox.y = extents.y;
		}

		// Now convert it back into world coordinates
		glm::vec2 closestPointOnBoxWorld = box->getPosition() + closestPointOnBox.x * box->getLocalX() + closestPointOnBox.y * box->getLocalY();

		glm::vec2 circleToBox = sphere->getPosition() - closestPointOnBoxWorld;
		if (glm::length(circleToBox) < sphere->getRadius())
		{
			glm::vec2 dir = glm::normalize(circleToBox);
			glm::vec2 contact = closestPointOnBoxWorld;
			box->resolveCollision(sphere, contact, &dir);
			return true;
		}
	}

	return false;
}

bool PhysicsScene::Box2Box(PhysicsObject* objBox1, PhysicsObject* objBox2)
{
	Box* box1 = dynamic_cast<Box*>(objBox1);
	Box* box2 = dynamic_cast<Box*>(objBox2);

	if (box1 != nullptr && box2 != nullptr)
	{
		glm::vec2 norm(0);
		glm::vec2 contact(0);
		float pen = 0;
		int numContact = 0;

		box1->checkBoxCorners(*box2, contact, numContact, pen, norm);

		if (box2->checkBoxCorners(*box1, contact, numContact, pen, norm))
		{
			norm = -norm;
		}

		if (pen > 0)
		{
			box1->resolveCollision(box2, contact / float(numContact), &norm);
			return true;
		}
	}

	return false;
}
