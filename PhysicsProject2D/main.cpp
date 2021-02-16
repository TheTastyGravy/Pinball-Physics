#include "PhysicsProject2DApp.h"

int main() {
	
	// allocation
	auto app = new PhysicsProject2DApp();

	// initialise and loop
	app->run("Pinball", 720, 900, false);

	// deallocation
	delete app;

	return 0;
}