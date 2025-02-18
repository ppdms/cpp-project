#define _USE_MATH_DEFINES
#include <cmath>
#include "Enemy.h"
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#ifdef _WIN64
#include "sgg/graphics.h"
#endif
#elif __APPLE__
#include <TargetConditionals.h>
#if TARGET_OS_MAC
#include "graphics.h"
#endif
#elif __linux__
#include "graphics.h"
#else
#error "Unknown compiler"
#endif
#include <iostream>
#include "Player.h"
#include "util.h"
#include <cstdlib>

Enemy::Enemy(float x, float y, float w, float h, std::string name, DecFn func, bool restrict_movement) : Box(x, y, w, h), GameObject(name, "Enemy"), movement(func), restrict_movement(restrict_movement)
{
	sword_right = new Sword(m_pos_x + 30, m_pos_y, 25.0f, 7.0f, "right sword");
	sword_left = new Sword(m_pos_x - 30, m_pos_y, 25.0f, 7.0f, "left sword");
	// Movement sprites
	sprites.push_back(std::string(ASSET_PATH) + "/Player/2.png");
	sprites.push_back(std::string(ASSET_PATH) + "/Player/3.png");
	sprites.push_back(std::string(ASSET_PATH) + "/Player/4.png");
	sprites.push_back(std::string(ASSET_PATH) + "/Player/5.png");
	sprites.push_back(std::string(ASSET_PATH) + "/Player/2_left.png");
	sprites.push_back(std::string(ASSET_PATH) + "/Player/3_left.png");
	sprites.push_back(std::string(ASSET_PATH) + "/Player/4_left.png");
	sprites.push_back(std::string(ASSET_PATH) + "/Player/5_left.png");

	// Standing sprites
	standing_sprites.push_back(std::string(ASSET_PATH) + "/Player/6.png");
	standing_sprites.push_back(std::string(ASSET_PATH) + "/Player/6_left.png");

	// Attack sprites
	attack_sprites.push_back(std::string(ASSET_PATH) + "/Player/7.png");
	attack_sprites.push_back(std::string(ASSET_PATH) + "/Player/8.png");
	attack_sprites.push_back(std::string(ASSET_PATH) + "/Player/9.png");
	attack_sprites.push_back(std::string(ASSET_PATH) + "/Player/10.png");
	attack_sprites.push_back(std::string(ASSET_PATH) + "/Player/11.png");
	attack_sprites.push_back(std::string(ASSET_PATH) + "/Player/12.png");
	attack_sprites.push_back(std::string(ASSET_PATH) + "/Player/13.png");
	attack_sprites.push_back(std::string(ASSET_PATH) + "/Player/14.png");
	attack_sprites.push_back(std::string(ASSET_PATH) + "/Player/7_left.png");
	attack_sprites.push_back(std::string(ASSET_PATH) + "/Player/8_left.png");
	attack_sprites.push_back(std::string(ASSET_PATH) + "/Player/9_left.png");
	attack_sprites.push_back(std::string(ASSET_PATH) + "/Player/10_left.png");
	attack_sprites.push_back(std::string(ASSET_PATH) + "/Player/11_left.png");
	attack_sprites.push_back(std::string(ASSET_PATH) + "/Player/12_left.png");
	attack_sprites.push_back(std::string(ASSET_PATH) + "/Player/13_left.png");
	attack_sprites.push_back(std::string(ASSET_PATH) + "/Player/14_left.png");

	// Jumping sprites
	jumping_sprites.push_back(std::string(ASSET_PATH) + "/Player/15.png");
	jumping_sprites.push_back(std::string(ASSET_PATH) + "/Player/16.png");
	jumping_sprites.push_back(std::string(ASSET_PATH) + "/Player/15_left.png");
	jumping_sprites.push_back(std::string(ASSET_PATH) + "/Player/16_left.png");
}

Enemy::~Enemy()
{
	delete sword_left;
	delete sword_right;

	auto s_it = this->sword_hits.begin();
	while (s_it != this->sword_hits.end())
	{
		delete* s_it;
		this->sword_hits.pop_front();
		s_it = this->sword_hits.begin();
	}
	sword_hits.clear();

	for (auto arrow : arrows)
		delete arrow;
}

void Enemy::update(float dt)
{
	dt_sum += dt;

	if ((std::rand() < (RAND_MAX * 0.2)))
	{
		if (Enemy::should_I_shoot())
		{
			// Create the arrow and add it to the arrow list
			Arrow* b = new Arrow(m_pos_x, m_pos_y, m_width, m_height, atan((-(m_state->getPlayer()->m_pos_y - m_pos_y)) / (m_state->getPlayer()->m_pos_x - m_pos_x)), "arrow");

			if (m_state->getPlayer()->m_pos_x - m_pos_x >= 0)
			{
				b = new Arrow(m_pos_x, m_pos_y, m_width, m_height, atan((-(m_state->getPlayer()->m_pos_y - m_pos_y)) / (m_state->getPlayer()->m_pos_x - m_pos_x)), "arrow"); // WHY DOES SGG COUNT Y BACKWARDS???
			}
			else
			{
				b = new Arrow(m_pos_x, m_pos_y, m_width, m_height, M_PI + atan((-(m_state->getPlayer()->m_pos_y - m_pos_y)) / (m_state->getPlayer()->m_pos_x - m_pos_x)), "arrow");
			}

			b->setMouse_x(m_state->getPlayer()->m_pos_x);
			b->setMouse_y(m_state->getPlayer()->m_pos_y);
			arrows.push_back(b);
		}
		if (Enemy::should_I_thrust())
		{
			if (looking_right)
				sword_hits.push_back(new Box(m_pos_x + 30, m_pos_y, 25.0f, 7.0f));
			else
				sword_hits.push_back(new Box(m_pos_x - 30, m_pos_y, 25.0f, 7.0f));
		}
		attacking = true;
	}

	if (!attacking)
	{
		if (should_I_thrust())
		{
			sword_selected = true;
			gun_selected = false;
		}
		else
		{
			sword_selected = false;
			gun_selected = true;
		}
	}

	for (Arrow* arrow : arrows)
	{
		if (arrow->get_shot() == false)
		{
			arrow->shoot(dt);
		}

		arrow->update(dt);
		arrow->set_shot(true);
	}

	float prevPosX = m_pos_x;
	float move = 0.0f;
	if (((this->*movement)().first == graphics::SCANCODE_A && !attacking))
	{ // Call the movement member function correctly
		looking_left = true;
		looking_right = false;
		move = -1.0f;
	}
	if ((this->*movement)().first == graphics::SCANCODE_D && !attacking)
	{ // Call the movement member function correctly
		looking_left = false;
		looking_right = true;
		move = 1.0f;
	}
	if (restrict_movement == false)
	{
		m_vx = std::min<float>(m_max_velocity, m_vx + dt * move * m_accel_horizontal); 
		m_vx = std::max<float>(-m_max_velocity, m_vx);

		// friction
		m_vx -= 0.2f * m_vx / (0.1f + fabs(m_vx));

		// apply static friction threshold
		if (fabs(m_vx) < 0.01f)
			m_vx = 0.0f;

		m_pos_x += m_vx * dt / 20.0f;
		sword_right->m_pos_x += m_vx * dt / 20.0f;
		sword_left->m_pos_x += m_vx * dt / 20.0f;

		if (((this->*movement)().second == graphics::SCANCODE_W) && jumping == false && falling == false && !attacking)
		{
			velocityY = -4.0;
			jumping = true;
		}

		// Apply gravity until terminal velocity is reached
		if (velocityY < 3.5)
			velocityY += gravity;
		m_pos_y += velocityY * dt / 10.0f;
		sword_right->m_pos_y += velocityY * dt / 10.0f;
		sword_left->m_pos_y += velocityY * dt / 10.0f;

		if (dt_sum > 600)
			dt_sum = 0;
	}
}

void Enemy::debugDraw()
{
	SETCOLOR(m_brush_debug.fill_color, 1, 0.3f, 0);
	SETCOLOR(m_brush_debug.outline_color, 1, 0.1f, 0);
	m_brush_debug.fill_opacity = 0.1f;
	m_brush_debug.outline_opacity = 1.0f;
	graphics::drawRect(m_pos_x, m_pos_y, m_width, m_height, m_brush_debug);

	graphics::setFont(std::string(ASSET_PATH) + "JetBrainsMono-Thin.ttf");
	char x[10];
	char y[10];
	SPRINTF(x, "%5.2f", m_pos_x);
	SPRINTF(y, "%5.2f", m_pos_y);
	SETCOLOR(m_brush_debug.fill_color, 1, 0, 0);
	m_brush_debug.fill_opacity = 1.0f;
	graphics::drawText(m_pos_x - m_width / 2, m_pos_y + m_height / 2, 16, x, m_brush_debug);
	graphics::drawText(m_pos_x - m_width / 2, m_pos_y + m_height / 2 - 16, 16, y, m_brush_debug);
}

void Enemy::draw()
{
	// Draw Enemy
	m_brush.fill_color[0] = 0.5f;
	m_brush.fill_color[1] = 1.0f;
	m_brush.fill_color[2] = 0.5f;
	m_brush.fill_opacity = 1.0f;
	m_brush.outline_opacity = 0.0f;

	int sprite = previous_sprite;

	if (attacking)
	{
		if (looking_right)
		{
			if (gun_selected)
			{
				if (frameCounter < 18)
				{
					m_brush.texture = attack_sprites[3];
				}
				else if (frameCounter < 36)
				{
					m_brush.texture = attack_sprites[4];
				}
				else if (frameCounter < 54)
				{
					m_brush.texture = attack_sprites[5];
				}
				else if (frameCounter < 72)
				{
					m_brush.texture = attack_sprites[6];
				}
				else if (frameCounter < 90)
				{
					m_brush.texture = attack_sprites[7];
					frameCounter = 0;
					attacking = false;
				}
				frameCounter++;
			}
			else if (sword_selected)
			{
				if (frameCounter < 25)
				{
					m_brush.texture = attack_sprites[0];
				}
				else if (frameCounter < 50)
				{
					m_brush.texture = attack_sprites[1];
				}
				else if (frameCounter < 75)
				{
					m_brush.texture = attack_sprites[2];
					frameCounter = 0;
					attacking = false;
				}
				frameCounter++;
			}
		}
		else
		{
			if (gun_selected)
			{
				if (frameCounter < 18)
				{
					m_brush.texture = attack_sprites[11];
				}
				else if (frameCounter < 36)
				{
					m_brush.texture = attack_sprites[12];
				}
				else if (frameCounter < 54)
				{
					m_brush.texture = attack_sprites[13];
				}
				else if (frameCounter < 72)
				{
					m_brush.texture = attack_sprites[14];
				}
				else if (frameCounter < 90)
				{
					m_brush.texture = attack_sprites[15];
					frameCounter = 0;
					attacking = false;
				}
				frameCounter++;
			}
			else if (sword_selected)
			{
				if (frameCounter < 25)
				{
					m_brush.texture = attack_sprites[8];
				}
				else if (frameCounter < 50)
				{
					m_brush.texture = attack_sprites[9];
				}
				else if (frameCounter < 75)
				{
					m_brush.texture = attack_sprites[10];
					frameCounter = 0;
					attacking = false;
				}
				frameCounter++;
			}
		}
	}
	else if (jumping)
	{
		if (looking_right)
		{
			if (velocityY < 0)
			{
				m_brush.texture = jumping_sprites[0];
			}
			else
			{
				m_brush.texture = jumping_sprites[1];
			}
		}
		else
		{
			if (velocityY < 0)
			{
				m_brush.texture = jumping_sprites[2];
			}
			else
			{
				m_brush.texture = jumping_sprites[3];
			}
		}
	}

	else
	{
		if (looking_right)
		{
			if (m_vx == 0)
			{
				sprite = 0;
				m_brush.texture = standing_sprites[sprite];
			}
			else
			{
				same_counter = 0;
				if (frameCounter < 12)
				{
					m_brush.texture = sprites[previous_sprite];
					frameCounter++;
				}
				else
				{

					m_brush.texture = sprites[sprite];

					previous_sprite = sprite;
					frameCounter = 0;
				}
			}
		}

		else
		{

			if (m_vx == 0)
			{
				sprite = 1;
				m_brush.texture = standing_sprites[sprite];
			}
			else
			{
				same_counter = 0;
				if (frameCounter < 12)
				{
					m_brush.texture = sprites[previous_sprite];
					frameCounter++;
				}
				else
				{
					sprite = ((int)fmod(100.0f - m_pos_x * 9.0f, sprites.size()) + 7) % 4 + 4;

					m_brush.texture = sprites[sprite];

					previous_sprite = sprite;
					frameCounter = 0;
				}
			}
		}
	}

	graphics::drawRect(m_pos_x, m_pos_y, 25.0f, 50.0f, m_brush);

	// Draw Sword
	sword_right->draw();
	sword_left->draw();

	// Draw arrows
	for (Arrow* arrow : arrows)
	{
		arrow->draw();
	}

	if (m_state->m_debugging)
		debugDraw();
}

void Enemy::init()
{
}

void Enemy::jump()
{
}

std::pair<graphics::scancode_t, graphics::scancode_t> Enemy::noMovement(void)
{
	return std::pair<graphics::scancode_t, graphics::scancode_t> {graphics::SCANCODE_Q, graphics::SCANCODE_Q};
}


std::pair<graphics::scancode_t, graphics::scancode_t> Enemy::dumbMovement(void)
{
	std::pair<graphics::scancode_t, graphics::scancode_t> decision;

	int dx = m_state->getPlayer()->m_pos_x - this->m_pos_x;
	int dy = -(m_state->getPlayer()->m_pos_y - this->m_pos_y);

	if (dy > 0)
	{
		decision.second = graphics::SCANCODE_W;
		if (dx > m_width / 2)
		{
			decision.first = graphics::SCANCODE_D;
		}
		else if (dx < -m_width / 2)
		{
			decision.first = graphics::SCANCODE_A;
		}
		else
		{
			decision.first = graphics::SCANCODE_Q;
		}
	}
	else if (dy <= 0)
	{
		decision.second = graphics::SCANCODE_Q;
		if (dx > m_width / 2)
		{
			decision.first = graphics::SCANCODE_D;
		}
		else if (dx < -m_width / 2)
		{
			decision.first = graphics::SCANCODE_A;
		}
		else
		{
			decision.first = graphics::SCANCODE_Q;
		}
	}
	else
	{
		decision.second = graphics::SCANCODE_Q;
	}

	return decision;
}

// Simple calculation used to see if the enemy should attack or not (either by shooting or thrusting)
bool Enemy::should_I_shoot()
{
	if (dt_sum < 600)
		return false;
	if (fabs(this->m_pos_x - PLAYER->m_pos_x) < 4 * m_width)
		return false;
	return (0 <= this->m_pos_x) && (this->m_pos_x <= WINDOW_WIDTH) && (0 <= this->m_pos_y) && (this->m_pos_y <= WINDOW_HEIGHT);
}

bool Enemy::should_I_thrust()
{

	if (dt_sum < 600)
		return false;
	if (fabs(this->m_pos_y - PLAYER->m_pos_y) >= m_height)
	{
		return false;
	}
	if (fabs(this->m_pos_x - PLAYER->m_pos_x) >= 2 * m_width)
	{
		return false;
	}
	return (0 <= this->m_pos_x) && (this->m_pos_x <= WINDOW_WIDTH) && (0 <= this->m_pos_y) && (this->m_pos_y <= WINDOW_HEIGHT);
}