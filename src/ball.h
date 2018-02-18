#include "main.h"

#ifndef BALL_H
#define BALL_H


class Ball {
public:
    Ball() {}
    Ball(float x, float y, color_t color);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    void accelearate();
    void change_x();
    double speed, speed_x;
    double radius_ball;
    double accel_x, accel_y;
    int score, lives;
    bounding_box_t bounding_box();
private:
    VAO *object;
};

#endif // BALL_H
