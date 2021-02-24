#include "PhysicsApp.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include "PhysicsScene.h"
#include <Gizmos.h>
#include <glm/ext.hpp>
#include "Sphere.h"
#include "Plane.h"

PhysicsApp::PhysicsApp() 
{
}

PhysicsApp::~PhysicsApp() 
{
}

bool PhysicsApp::startup() {
	
	// increase the 2d line count to maximize the number of objects we can draw
	aie::Gizmos::create(255U, 255U, 65535U, 65535U);
	m_2dRenderer = new aie::Renderer2D();
	m_font = new aie::Font("./font/consolas.ttf", 32);
	physicsScene = new PhysicsScene();
	physicsScene->setGravity({ 0,-0.98F });
	physicsScene->setTimeStep(0.01f);
	Sphere* ball = new Sphere({ -40, 0 }, { 10, 10 }, 6.0f, 2, { 1, 0, 0, 1 });
	Sphere* ball2 = new Sphere({ 40, 0 }, { -10, 10 }, 12.0f, 4, { 1, 0, 0, 1 });
	physicsScene->addActor(ball);
	physicsScene->addActor(ball2);

	Plane* bottomPlane = new Plane({ 0,1 }, -45.0F, {0,0,1,1});
	Plane* leftPlane = new Plane({ 1,0 }, -90.0F, { 0,0,1,1 });
	Plane* rightPlane = new Plane({ -1,0 }, -90.0F, { 0,0,1,1 });
	Plane* topPlane = new Plane({ 0,-1 }, -45.0F, { 0,0,1,1 });
	physicsScene->addActor(bottomPlane);
	physicsScene->addActor(leftPlane);
	physicsScene->addActor(rightPlane);
	physicsScene->addActor(topPlane);
	return true;
}

void PhysicsApp::shutdown() {
	
	delete m_font;
	delete m_2dRenderer;
	delete physicsScene;
}

void PhysicsApp::update(float deltaTime) {

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

void PhysicsApp::draw() {

	// wipe the screen to the background colour
	clearScreen();
	// begin drawing sprites
	m_2dRenderer->begin();
	// draw your stuff here!
	static float aspectRatio = 16 / 9.f;
	aie::Gizmos::draw2D(glm::ortho<float>(-100, 100, -100 / aspectRatio, 100 / aspectRatio, -1.0f, 1.0f));
	// output some text, uses the last used colour
	m_2dRenderer->drawText(m_font, "Press ESC to quit", 0, 0);
	// done drawing sprites
	m_2dRenderer->end();

}