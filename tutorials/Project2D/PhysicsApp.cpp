#include "PhysicsApp.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include "PhysicsScene.h"
#include <Gizmos.h>
#include <glm/ext.hpp>
#include "Sphere.h"
#include "AABB.h"
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
	Sphere* ball = new Sphere({ -40, 0 }, { 80, 10 }, 6.0f, 2, { 1, 0.1, 0.4, 1 });
	Sphere* ball2 = new Sphere({ 40, 0 }, { -80, 10 }, 12.0f, 4, { 1, 0, 0, 1 });
	Sphere* ball3 = new Sphere({ 60, 0 }, {10,0}, 24.0F, 32, { 0, 0, 1, 1 });
	AABB* box = new AABB({ 20, 20 }, { -5,5 }, 4, { 6,6 }, {1,0,0,1});
	AABB* box2 = new AABB({ -20, 20 }, { 204,6 }, 3, { 5,5 }, { 1,0.4,0.4,1 });
	AABB* box3 = new AABB({ -30, -20 }, { 0,0 }, 1, { 10,10 }, { 0,0,1,1 });
	AABB* box4 = new AABB({ -30, 20 }, { 0,0 }, 1, { 10,10 }, { 0,0,1,1 });
	box3->setStatic(true);
	box4->setStatic(true);
	ball3->setStatic(true);
	ball2->setAngularVelocity(10.0F);
	physicsScene->addActor(ball);
	physicsScene->addActor(ball2);
	physicsScene->addActor(ball3);
	physicsScene->addActor(box);
	physicsScene->addActor(box2);
	physicsScene->addActor(box3);
	physicsScene->addActor(box4);

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