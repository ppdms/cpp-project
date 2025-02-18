#define _USE_MATH_DEFINES
#include <cmath>
#include "Arrow.h"
#include "Level.h"
#include <iostream>

void Arrow::update(float dt)
{
	// Depending on the position of the player's mouse, we change the position of the arrow to simulate it's firing
	if (up_right) {
		m_pos_y -= initialVelocityY*dt/10;
		m_pos_x += initialVelocityX*dt/10;
	}
	else if (up_left) {
		m_pos_y -= initialVelocityY*dt/10;
		m_pos_x -= initialVelocityX*dt/10;
	}
	else if (down_left) {
		m_pos_y += initialVelocityY*dt/10;
		m_pos_x -= initialVelocityX*dt/10;
	}
	else if (down_right) {
		m_pos_y += initialVelocityY*dt/10;
		m_pos_x += initialVelocityX*dt/10;
	}
}

void Arrow::draw()
{
	// Draw the arrow and set its orientation dependign on where the player is shooting
	graphics::Brush br;
	br.outline_opacity = 0.0f;
	br.texture = std::string(ASSET_PATH) + "Arrow1.png";
	graphics::setOrientation(theta * (180 / M_PI));
	graphics::drawRect(m_pos_x, m_pos_y, 121.0f / 3, 26.0f / 3, br);
	graphics::resetPose();
}

void Arrow::init()
{
}

Arrow::Arrow(float x, float y, float w, float h, float theta, std::string name) : Box(x, y, w, h), GameObject(name), theta(theta)
{
}

Arrow::~Arrow()
{
}

void Arrow::shoot(float dt)
{
	float direction_x = abs(mouse_x - m_pos_x);
	float direction_y = abs(mouse_y - m_pos_y);
	length = std::sqrt(direction_x * direction_x + direction_y * direction_y);


	// Normalize the direction vector (make it a unit vector)
	direction_x = direction_x / length;
	direction_y = direction_y / length;

	// Calculate the initial velocity components
	initialVelocityX = direction_x * speed * dt / 30.0f;
	initialVelocityY = direction_y * speed * dt / 30.0f; 

	tangent = abs(mouse_y - m_pos_y) / abs(mouse_x - m_pos_x); \

		// We set as true only the quadrant in which the mouse cursor is located
		if (mouse_x > m_pos_x && mouse_y < m_pos_y) {
			up_right = true;
			up_left = false;
			down_right = false;
			down_left = false;
		}
		else if (mouse_x < m_pos_x && mouse_y < m_pos_y) {
			up_right = false;
			up_left = true;
			down_right = false;
			down_left = false;
		}
		else if (mouse_x > m_pos_x && mouse_y > m_pos_y) {
			up_right = false;
			up_left = false;
			down_right = true;
			down_left = false;
		}
		else if (mouse_x < m_pos_x && mouse_y > m_pos_y) {
			up_right = false;
			up_left = false;
			down_right = false;
			down_left = true;
		}

}

void Arrow::setMouse_x(float pos_x)
{
	mouse_x = pos_x;
}

void Arrow::setMouse_y(float pos_y)
{
	mouse_y = pos_y;
}

float Arrow::get_x()
{
	return m_pos_x;
}
float Arrow::get_y()
{
	return m_pos_y;
}

void Arrow::set_shot(bool s)
{
	shot = true;
}

bool Arrow::get_shot()
{
	return shot;
}
