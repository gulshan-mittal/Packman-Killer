#include "main.h"

#ifndef TREE_H
#define TREE_H


class Trees
{
public:
    Trees(){};
    Trees(float x, float y, float max, float min);
    glm::vec3 position;
    void draw(glm::mat4 VP);
    double maxi, mini ;
private:
    VAO *object;
};

#endif 
