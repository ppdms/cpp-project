#include "level.h"

#include <graphics.h>

#include "Enemy.h"
#include "player.h"

/*
void Level::drawBlock(int i)
{
        Box& box = m_blocks[i];
        std::string& name = m_block_names[i];

        m_block_brush.texture = m_state->getFullAssetPath(name);

        graphics::drawRect(x, y, 1.8f * m_block_size, 1.8f * m_block_size,
m_block_brush);

        if (m_state->m_debugging)
                graphics::drawRect(x, y, m_block_size, m_block_size,
m_block_brush_debug);

}
*/

void Level::checkCollisions() {
    // Bullet collisions / Out of bounds check
    auto it = m_state->getPlayer()->bullets.begin();
    while (it != m_state->getPlayer()->bullets.end()) {
        if ((*it)->get_x() > 700 || (*it)->get_x() < 0 ||
            (*it)->get_y() > 450 || (*it)->get_y() < 0 ||
            (*it)->collision_detected(m_blocks) == true) {
            // Remove the bullet from the list
            delete *it;
            it = m_state->getPlayer()->bullets.erase(it);
        } else {
            ++it;
        }
    }
    // Sword collisions

    // Player, Enemy obstacle collisions
    for (Obstacle* ob : m_blocks) {
        if (m_state->getPlayer()->intersect(*ob)) {
            float belowCorrection = m_state->getPlayer()->intersectDown(*ob);
            if (belowCorrection != 0 && m_state->getPlayer()->jumping == true &&
                m_state->getPlayer()->velocityY <= 0) {
                // std::cout << "belowCorrection: "<<belowCorrection <<
                // std::endl;
                m_state->getPlayer()->m_pos_y -= belowCorrection;
                m_state->getPlayer()->sword_right->m_pos_y -= belowCorrection;
                m_state->getPlayer()->sword_left->m_pos_y -= belowCorrection;
                m_state->getPlayer()->velocityY = 0;
            }
        }
        for (auto it = m_dynamic_objects.begin(); it != m_dynamic_objects.end();
             ++it) {
            if (!((*it)->isActive())) continue;
            if (!((*it)->m_class == "Enemy")) continue;
            Enemy* g_ob = dynamic_cast<Enemy*>(*it);
            if (g_ob->intersect(*ob)) {
                float belowCorrection = g_ob->intersectDown(*ob);
                if (belowCorrection != 0 && g_ob->jumping == true &&
                    g_ob->velocityY <= 0) {
                    // std::cout << "belowCorrection: "<<belowCorrection <<
                    // std::endl;
                    g_ob->m_pos_y -= belowCorrection;
                    g_ob->sword_right->m_pos_y -= belowCorrection;
                    g_ob->sword_left->m_pos_y -= belowCorrection;
                    g_ob->velocityY = 0;
                }
            }
        }
    }

    // Player <-> Enemy collisions (we move Enemy)
    // we are ignoring Enemy <-> Enemy collisions since they're on the same "team"

    for (auto it = m_dynamic_objects.begin(); it != m_dynamic_objects.end();
         ++it) {
        if (!((*it)->isActive())) continue;
        if (!((*it)->m_class == "Enemy")) continue;
        Enemy* g_ob = dynamic_cast<Enemy*>(*it);
        if (g_ob->intersect(*m_state->getPlayer())) {
             float horizCorrection = g_ob->intersectSideways(*m_state->getPlayer());
                if (horizCorrection != 0) {
                    g_ob->m_pos_x += horizCorrection;
                    g_ob->sword_right->m_pos_x += horizCorrection;
                    g_ob->sword_left->m_pos_x += horizCorrection;
                }
        }
    }


    for (Obstacle* ob : m_blocks) {
        if (m_state->getPlayer()->intersect(*ob)) {
            float vertCorrection = m_state->getPlayer()->intersectAbove(*ob);
            if (vertCorrection != 0) {
                m_state->getPlayer()->m_pos_y += vertCorrection;
                m_state->getPlayer()->sword_right->m_pos_y += vertCorrection;
                m_state->getPlayer()->sword_left->m_pos_y += vertCorrection;
                m_state->getPlayer()->velocityY = 0;
                m_state->getPlayer()->jumping = false;
            }
        }
        for (auto it = m_dynamic_objects.begin(); it != m_dynamic_objects.end();
             ++it) {
            if (!((*it)->isActive())) continue;
            if (!((*it)->m_class == "Enemy")) continue;
            Enemy* g_ob = dynamic_cast<Enemy*>(*it);
            if (g_ob->intersect(*ob)) {
                float vertCorrection = g_ob->intersectAbove(*ob);
                if (vertCorrection != 0) {
                    g_ob->m_pos_y += vertCorrection;
                    g_ob->sword_right->m_pos_y += vertCorrection;
                    g_ob->sword_left->m_pos_y += vertCorrection;
                    g_ob->velocityY = 0;
                    g_ob->jumping = false;
                }
            }
        }
    }
    for (Obstacle* ob : m_blocks) {
        if (m_state->getPlayer()->intersect(*ob)) {
            float horizCorrection =
                m_state->getPlayer()->intersectSideways(*ob);
            if (horizCorrection != 0) {
                m_state->getPlayer()->m_pos_x += horizCorrection;
                m_state->getPlayer()->sword_right->m_pos_x += horizCorrection;
                m_state->getPlayer()->sword_left->m_pos_x += horizCorrection;
            }
        }
        for (auto it = m_dynamic_objects.begin(); it != m_dynamic_objects.end();
             ++it) {
            if (!((*it)->isActive())) continue;
            if (!((*it)->m_class == "Enemy")) continue;
            Enemy* g_ob = dynamic_cast<Enemy*>(*it);
            if (g_ob->intersect(*ob)) {
                float horizCorrection = g_ob->intersectSideways(*ob);
                if (horizCorrection != 0) {
                    g_ob->m_pos_x += horizCorrection;
                    g_ob->sword_right->m_pos_x += horizCorrection;
                    g_ob->sword_left->m_pos_x += horizCorrection;
                }
            }
        }
    }
    /*
    for (auto& block : m_blocks)
    {
            float offset = 0.0f;
            if (offset = m_state->getPlayer()->intersectDown(*block))
            {
                    m_state->getPlayer()->m_pos_y += offset;

                    // add sound event
                    if (m_state->getPlayer()->velocityY > 1.0f)
                            graphics::playSound(m_state->getFullAssetPath("Metal2.wav"),
    1.0f);

                    m_state->getPlayer()->velocityY = 0.0f;

                    break;
            }
    }

    for (auto& block : m_blocks)
    {
            float offset = 0.0f;
            if (offset = m_state->getPlayer()->intersectSideways(*block))
            {
                    m_state->getPlayer()->m_pos_x += offset;

                    m_state->getPlayer()->m_vx = 0.0f;
                    break;
            }

    }
    */
}

void Level::update(float dt) {
    if (m_state->getPlayer()->isActive()) m_state->getPlayer()->update(dt);

    for (auto& g_ob : m_dynamic_objects) {
        if (g_ob->isActive()) g_ob->update(dt);
    }

    checkCollisions();

    GameObject::update(dt);
}

void Level::draw() {
    graphics::drawRect(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, WINDOW_WIDTH,
                       WINDOW_HEIGHT, m_brush_background);

    // draw player
    if (m_state->getPlayer()->isActive()) m_state->getPlayer()->draw();

    for (auto& g_ob : m_dynamic_objects) {
        if (g_ob->isActive()) g_ob->draw();
    }

    // draw obstacles

    for (int i = 0; i < m_blocks.size(); i++) {
        m_blocks[i]->draw();
    }
}

void Level::init() {
    // Stage 1

    for (auto p_gob : m_static_objects) {
        // std::cout << "here" << std::endl;
        if (p_gob) p_gob->init();
    }

    for (auto p_gob : m_dynamic_objects) {
        // std::cout << "there" << std::endl;
        if (p_gob) p_gob->init();
    }

    // initialize some collidable blocks here.
    // normally, you should build a Block class
    // and automate the process more.
    /*
    m_blocks.push_back(Box(5, 6, 1, 1));
    m_blocks.push_back(Box(4, 6, 1, 1));
    m_blocks.push_back(Box(3, 6, 1, 1));
    m_blocks.push_back(Box(2, 5, 1, 1));
    m_blocks.push_back(Box(6, 6, 1, 1));
    m_blocks.push_back(Box(7, 6, 1, 1));
    m_blocks.push_back(Box(7, 5, 1, 1));
    m_blocks.push_back(Box(3, 2, 1, 1));
    m_blocks.push_back(Box(4, 3, 1, 1));
    */

    m_blocks.push_back(
        new Obstacle(WINDOW_WIDTH / 2, 300, 700, 35, "obstacle1"));
    m_blocks.push_back(
        new Obstacle(WINDOW_WIDTH / 2 + 20, 300, 35, 200, "obstacle2"));
    m_blocks.push_back(
        new Obstacle(WINDOW_WIDTH / 2, 100, 400, 35, "obstacle3"));
    m_blocks.push_back(
        new Obstacle(WINDOW_WIDTH / 2 + 200, 300, 35, 200, "obstacle4"));
}

Level::Level(const std::string& name) : GameObject(name) {
    m_brush_background.outline_opacity = 0.0f;
    m_brush_background.texture = std::string(ASSET_PATH) + "background_lvl.png";

    m_dynamic_objects.push_back(new Enemy(WINDOW_WIDTH / 4, WINDOW_HEIGHT / 4,
                                          25, 50, "Enemy",
                                          &Enemy::dumbMovement));
    
    for (auto p_go : m_dynamic_objects)
        p_go->init();

}

Level::~Level() {
    for (auto p_go : m_static_objects) delete p_go;
    for (auto p_go : m_dynamic_objects) delete p_go;
}