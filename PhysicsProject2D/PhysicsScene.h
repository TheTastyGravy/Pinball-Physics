#pragma once
#include <glm/glm.hpp>
#include <vector>

class PhysicsObject;

class PhysicsScene
{
public:
	PhysicsScene();
	~PhysicsScene();

	void addActor(PhysicsObject* actor);
	void removeActor(PhysicsObject* actor);

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



protected:
	glm::vec2 gravity;
	float timeStep;


	std::vector<PhysicsObject*> actors;

};