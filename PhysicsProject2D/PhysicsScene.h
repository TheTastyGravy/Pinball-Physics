#pragma once
#include <glm/glm.hpp>
#include <vector>

class PhysicsObject;
class Rigidbody;

class PhysicsScene
{
public:
	PhysicsScene();
	~PhysicsScene();

	void addActor(PhysicsObject* actor);
	void removeActor(PhysicsObject* actor);

	void resetScene();

	// This will call the update of each PhysicsObject and updates internally
	// This will handle collision detection and resolution
	void update(float deltaTime);
	// Called once per frame and handles the rendering of PhysicsObjects
	// Will add to a loop of Gizmo objects to render
	void draw();

	void debugScene();

	void setGravity(const glm::vec2 gravity) { this->gravity = gravity; }
	glm::vec2 getGravity() const { return gravity; }

	void setTimeStep(const float timeStep) { this->timeStep = timeStep; }
	float getTimeStep() const { return timeStep; }

	void checkForCollision();
	static void applyContactForces(Rigidbody* actor1, Rigidbody* actor2, glm::vec2 collisionNorm, float pen);

	static bool Plane2Plane(PhysicsObject*, PhysicsObject*);
	static bool Plane2Sphere(PhysicsObject*, PhysicsObject*);
	static bool Plane2Box(PhysicsObject*, PhysicsObject*);

	static bool Sphere2Plane(PhysicsObject*, PhysicsObject*);
	static bool Sphere2Sphere(PhysicsObject*, PhysicsObject*);
	static bool Sphere2Box(PhysicsObject*, PhysicsObject*);

	static bool Box2Plane(PhysicsObject*, PhysicsObject*);
	static bool Box2Sphere(PhysicsObject*, PhysicsObject*);
	static bool Box2Box(PhysicsObject*, PhysicsObject*);

protected:
	glm::vec2 gravity;
	float timeStep;

	std::vector<PhysicsObject*> actors;

};