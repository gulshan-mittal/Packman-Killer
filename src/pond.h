#include "main.h"

#ifndef POND_H
#define POND_H


class Pond {
public:
    Pond() {}
    Pond(float x, float y, color_t color, float rad, int tr_num);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    double radius_pond;
    bounding_box_t bounding_box();
private:
    VAO *object;
};

#endif 
