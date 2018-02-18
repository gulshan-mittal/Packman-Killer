#include "main.h"

#ifndef GROUND_H
#define GROUND_H


class Ground {
public:
    Ground(){}
    Ground(float x, float y, color_t color, float psuedo_pt, float psuedo_ht, float x1, float x2);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
private:
    VAO *object;
};

#endif // BALL_H

