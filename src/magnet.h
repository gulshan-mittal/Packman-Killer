#include "main.h"

#ifndef MAGNET_H
#define MAGNET_H


class Magnet {
public:
    Magnet() {}
    Magnet(float x, float y, color_t color, float rotor);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void tick();
    int num;
    bounding_box_t bounding_box();
private:
    VAO *object;
};

#endif 
