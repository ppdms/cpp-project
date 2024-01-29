#define _USE_MATH_DEFINES
#include <cmath>
#include "Enemy.h"
#include "sgg/graphics.h"
#include <iostream>
#include "Player.h"
#include "util.h"
#include <cstdlib>

/*
Enemy::Enemy()
{

}
*/
// MOUSE POSITION WHEN WINDOW SIZE CHANGES NEEDS TO BE FIXED!!!!!!!!!!!

Enemy::Enemy(float x, float y, float w, float h, std::string name, DecFn func, bool restrict_movement) : Box(x, y, w, h), GameObject(name, "Enemy"), movement(func), restrict_movement(restrict_movement)
{
	sword_right = new Sword(m_pos_x + 30, m_pos_y, 25.0f, 7.0f, "right sword");
	sword_left = new Sword(m_pos_x - 30, m_pos_y, 25.0f, 7.0f, "left sword");
	// Movement sprites
	sprites.push_back(std::string(ASSET_PATH) + "/" + name + "/" + "2.png");
	sprites.push_back(std::string(ASSET_PATH) + "/" + name + "/" + "3.png");
	sprites.push_back(std::string(ASSET_PATH) + "/" + name + "/" + "4.png");
	sprites.push_back(std::string(ASSET_PATH) + "/" + name + "/" + "5.png");
	sprites.push_back(std::string(ASSET_PATH) + "/" + name + "/" + "2_left.png");
	sprites.push_back(std::string(ASSET_PATH) + "/" + name + "/" + "3_left.png");
	sprites.push_back(std::string(ASSET_PATH) + "/" + name + "/" + "4_left.png");
	sprites.push_back(std::string(ASSET_PATH) + "/" + name + "/" + "5_left.png");

	// Standing sprites
	standing_sprites.push_back(std::string(ASSET_PATH) + "/" + name + "/" + "6.png");
	standing_sprites.push_back(std::string(ASSET_PATH) + "/" + name + "/" + "6_left.png");

	// Attack sprites
	attack_sprites.push_back(std::string(ASSET_PATH) + "/" + name + "/" + "7.png");
	attack_sprites.push_back(std::string(ASSET_PATH) + "/" + name + "/" + "8.png");
	attack_sprites.push_back(std::string(ASSET_PATH) + "/" + name + "/" + "9.png");
	attack_sprites.push_back(std::string(ASSET_PATH) + "/" + name + "/" + "10.png");
	attack_sprites.push_back(std::string(ASSET_PATH) + "/" + name + "/" + "11.png");
	attack_sprites.push_back(std::string(ASSET_PATH) + "/" + name + "/" + "12.png");
	attack_sprites.push_back(std::string(ASSET_PATH) + "/" + name + "/" + "13.png");
	attack_sprites.push_back(std::string(ASSET_PATH) + "/" + name + "/" + "14.png");
	attack_sprites.push_back(std::string(ASSET_PATH) + "/" + name + "/" + "7_left.png");
	attack_sprites.push_back(std::string(ASSET_PATH) + "/" + name + "/" + "8_left.png");
	attack_sprites.push_back(std::string(ASSET_PATH) + "/" + name + "/" + "9_left.png");
	attack_sprites.push_back(std::string(ASSET_PATH) + "/" + name + "/" + "10_left.png");
	attack_sprites.push_back(std::string(ASSET_PATH) + "/" + name + "/" + "11_left.png");
	attack_sprites.push_back(std::string(ASSET_PATH) + "/" + name + "/" + "12_left.png");
	attack_sprites.push_back(std::string(ASSET_PATH) + "/" + name + "/" + "13_left.png");
	attack_sprites.push_back(std::string(ASSET_PATH) + "/" + name + "/" + "14_left.png");

	// Jumping sprites
	jumping_sprites.push_back(std::string(ASSET_PATH) + "/" + name + "/" + "15.png");
	jumping_sprites.push_back(std::string(ASSET_PATH) + "/" + name + "/" + "16.png");
	jumping_sprites.push_back(std::string(ASSET_PATH) + "/" + name + "/" + "15_left.png");
	jumping_sprites.push_back(std::string(ASSET_PATH) + "/" + name + "/" + "16_left.png");
}

Enemy::~Enemy()
{
	delete sword_left;
	delete sword_right;

	for (auto arrow : arrows)
		delete arrow;
}

void Enemy::update(float dt)
{
	dt_sum += dt;
	/*
	for (Arrow* arrow : arrows) {
		if (arrow->get_x() > 700 || arrow->get_x() < 0 || arrow->get_y() > 450 || arrow->get_y() < 0 || arrow->collision_detected() == true) {
			std::cout << "deleted" << std::endl;
			// Remove the arrow from the active arrows vector
			auto it = std::find(arrows.begin(), arrows.end(), arrow);
			arrows.erase(it);
			// Delete it and release the heap space
			//delete arrow;
			//arrow = nullptr;
		}

	}
	*/

	if ((std::rand() < (RAND_MAX * 0.2)))
	{
		if (Enemy::should_I_shoot())
		{
			// std::cout << "shoot" << std::endl;
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

	/*
	if (graphics::getKeyState(graphics::SCANCODE_Q)) {
		// Invert the boolean values
		sword_selected = !sword_selected;
		gun_selected = !gun_selected;
		//if (sword_selected) std::cout << "sword" << std::endl;
		//else std::cout << "gun" << std::endl;

	}
	*/

	for (Arrow* arrow : arrows)
	{
		if (arrow->get_shot() == false)
		{
			arrow->shoot();
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
	if (restrict_movement == false) {
		m_vx = std::min<float>(m_max_velocity, m_vx + graphics::getDeltaTime() * move * m_accel_horizontal);
		m_vx = std::max<float>(-m_max_velocity, m_vx);

		// friction
		m_vx -= 0.2f * m_vx / (0.1f + fabs(m_vx));

		// apply static friction threshold
		if (fabs(m_vx) < 0.01f)
			m_vx = 0.0f;

		m_pos_x += m_vx * graphics::getDeltaTime() / 20.0f;
		sword_right->m_pos_x += m_vx * graphics::getDeltaTime() / 20.0f;
		sword_left->m_pos_x += m_vx * graphics::getDeltaTime() / 20.0f;

		if (((this->*movement)().second == graphics::SCANCODE_W) && jumping == false && falling == false && !attacking)
		{
			velocityY = -4.0;
			jumping = true;
			// posy_dummy = posy;
		}

		// Apply gravity until terminal velocity is reached
		if (velocityY < 3.5)
			velocityY += gravity;
		m_pos_y += velocityY * graphics::getDeltaTime() / 10.0f;
		sword_right->m_pos_y += velocityY * graphics::getDeltaTime() / 10.0f;
		sword_left->m_pos_y += velocityY * graphics::getDeltaTime() / 10.0f;

		if (dt_sum > 600)
			dt_sum = 0;
	}
	
	/*
	m_pos_x -= speed * graphics::getDeltaTime() / 20.0f;
	if (looking_right) sword->m_pos_x += -2 * 30;
	looking_left = true;
	looking_right = false;
	sword->m_pos_x -= speed * graphics::getDeltaTime() / 20.0f;
	*/
	/*
	m_pos_x += speed * graphics::getDeltaTime() / 20.0f;
	if (looking_left) sword->m_pos_x += 2 * 30;
	looking_left = false;
	looking_right = true;
	sword->m_pos_x += speed * graphics::getDeltaTime() / 20.0f;
	*/
	

	/*
	if (jumping) {
		if (velocityY < 3.5) velocityY += gravity;
		m_pos_y += velocityY;

		if (m_pos_y >= WINDOW_HEIGHT / 2) jumping = false;
		//posy -= up_velocity * graphics::getDeltaTime() / 7.0f;
		//up_velocity -= gravity;
		//std::cout << up_velocity << std::endl;
		//if (up_velocity <= -speed){
		//	jumping = false;
		//	up_velocity = speed;
		//}
	}
	*/
	/*
	// Check if the Enemy collides with any of the Obstacles and perform needed changes in position
	for (Obstacle* ob : obstaclesList) {
		if (intersect(*ob)) {
			float belowCorrection = intersectDown(*ob);
			if (belowCorrection != 0 && jumping == true && velocityY <= 0) {
				//std::cout << "belowCorrection: "<<belowCorrection << std::endl;
				m_pos_y -= belowCorrection;
				sword->m_pos_y -= belowCorrection;
				velocityY = 0;
			}
		}
	}
	for (Obstacle* ob : obstaclesList) {
		if (intersect(*ob)) {
			float vertCorrection = intersectAbove(*ob);
			if (vertCorrection != 0) {
				m_pos_y += vertCorrection;
				sword->m_pos_y += vertCorrection;
				velocityY = 0;
				jumping = false;
			}

		}
	}
	for (Obstacle* ob : obstaclesList) {
		if (intersect(*ob)) {
			float horizCorrection = intersectSideways(*ob);
			if (horizCorrection != 0 && prevPosX != m_pos_x) {
				m_pos_x += horizCorrection;
				sword->m_pos_x += horizCorrection;
			}
		}
	}
	*/

	/*
	if ((posx > posx_ - width / 2 && posx < posx_ + width / 2) && (posy > posy_ - height / 2 && posy < posy_ + height / 2)) {
		if (jumping == true && up_velocity >= 0) {
			up_velocity = 0;
			posy += 1;
		}
		else if (jumping == true && up_velocity < 0) {
			jumping = false;
			falling = false;
			up_velocity = speed;
		}
		else if (posy < posy_) {
			falling = false;
		}
	}
	else {
		falling = true;
	}
	if (falling && !shot) {
		posy += gravity * graphics::getDeltaTime() / 3.0f;
	}
	*/
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
	sprintf_s(x, "%5.2f", m_pos_x);
	sprintf_s(y, "%5.2f", m_pos_y);
	SETCOLOR(m_brush_debug.fill_color, 1, 0, 0);
	m_brush_debug.fill_opacity = 1.0f;
	graphics::drawText(m_pos_x - m_width / 2, m_pos_y + m_height / 2, 16, x, m_brush_debug);
	graphics::drawText(m_pos_x - m_width / 2, m_pos_y + m_height / 2 - 16, 16, y, m_brush_debug);
}

void Enemy::draw()
{
	// Draw Enemy
	m_brush.fill_color[0] = 1.0f;
	m_brush.fill_color[1] = 1.0f;
	m_brush.fill_color[2] = 1.0f;
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
					sprite = ((int)fmod(100.0f - m_pos_x * 9.0f, sprites.size()) + 7) % 4;
					// std::cout << sprite << std::endl;

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
					// std::cout << sprite << std::endl;

					m_brush.texture = sprites[sprite];

					previous_sprite = sprite;
					frameCounter = 0;
				}
			}
		}
	}
	// int sprite = ((int)fmod(100.0f - m_pos_x * 9.0f, sprites.size()) + 9)%5;
	// std::cout << sprite << std::endl;

	// if he is facing to the left
	// 25.0f, 50.0f

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

bool Enemy::should_I_shoot()
{
	return false;
	if (dt_sum < 600)
		return false;
	if (fabs(this->m_pos_x - PLAYER->m_pos_x) < 4 * m_width)
		return false;
	return (0 <= this->m_pos_x) && (this->m_pos_x <= WINDOW_WIDTH) && (0 <= this->m_pos_y) && (this->m_pos_y <= WINDOW_HEIGHT);
}

bool Enemy::should_I_thrust()
{
	return false;
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