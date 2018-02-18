#include "main.h"

#ifndef ENEMY_H
#define ENEMY_H


class Packman {
public:
    Packman() {}
    Packman(float x, float y, color_t color, double packradii);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    void accelearate();
    double speed_x;
    double radius_pack;
    int life;
    bounding_box_t bounding_box();
private:
    VAO *object;
};

#endif 
