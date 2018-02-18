#include "main.h"

#ifndef PLANK_H
#define PLANK_H


class Plank
{
public:
    Plank(){};
    Plank(float x, float y, float speed, float rotation);
    glm::vec3 position;
    void draw(glm::mat4 VP);
    float maxi, mini ;
    void tick();
    float rotation;
    double speed_x;
    double speed_y;
private:
    VAO *object;
};

#endif 
