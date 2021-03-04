#pragma once

#include "Application.h"
#include "Renderer2D.h"

class PhysicsApp : public aie::Application {
public:

	PhysicsApp();
	static float lerp(float start, float end, float power) { return start + (end - start) * power; }
	virtual ~PhysicsApp();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

protected:
	class PhysicsScene* physicsScene;
	aie::Renderer2D*	m_2dRenderer;
	aie::Texture*		m_texture;
	aie::Texture*		m_shipTexture;
	aie::Font*			m_font;

	float m_timer;
};