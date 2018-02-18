#include "main.h"

#ifndef PORCUPINE_H
#define PORCUPINE_H


class Porcupine
{
public:
    Porcupine(){};
    Porcupine(float x, float y, double speed_x, float max, float min, color_t color);
    glm::vec3 position;
    void draw(glm::mat4 VP);
    void tick();
    double speed_x;
    struct range {
        float left_x;
        float right_x;
    }range;
private:
    VAO *object;
};

#endif 
