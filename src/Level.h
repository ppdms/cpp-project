#pragma once

#include "GameObject.h"
#include "Obstacle.h"
#include <vector>
#include <list>
#include <string>
#include <graphics.h>
#include "Player.h"

class Level : public GameObject
{
	graphics::Brush m_brush_background;
	graphics::Brush m_brush_health;
	graphics::Brush m_brush_quiver;
	graphics::Brush m_brush_quiver_text;

	std::vector<GameObject *> *m_static_objects;
	std::list<GameObject *> *m_dynamic_objects;

	// detect collisions
	void checkCollisions();

public:
	void update(float dt) override;
	void draw() override;
	void init() override;

	Level(std::vector<GameObject *> *m_static_objects,
		  std::list<GameObject *> *m_dynamic_objects, const std::string &bg, const std::string &name = "Level0");
	~Level() override;
};
