#include "ball.h"
#include "main.h"

Ball::Ball(float x, float y, color_t color) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    this->speed = 0.05;
    // speed = 0.05;
    this->accel_x = 0;
    this->accel_y = 0.008;
    this->radius_ball = 0.32;
    this->score = 0;
    this->lives = 3;
    this->speed_x = 0.08;    
    GLfloat g_vertex_buffer_data[4000];
    int j=0,k=1;
    double pi = 3.14257;
    double xx=0.4;
    double yy = 0.0;
    for(int i=0;i<=360;i++)
    {
        g_vertex_buffer_data[j++] = 0.0f;
        g_vertex_buffer_data[j++] = 0.0f;
        g_vertex_buffer_data[j++] = 0.0f;

        g_vertex_buffer_data[j++] = xx;
        g_vertex_buffer_data[j++] = yy;
        g_vertex_buffer_data[j++] = 0.0f;

        xx= radius_ball*cos((double)k*pi/180);
        yy= radius_ball*sin((double)k*pi/180);
        k++;

        g_vertex_buffer_data[j++] = xx;
        g_vertex_buffer_data[j++] = yy;
        g_vertex_buffer_data[j++] = 0.0f;
    }

    this->object = create3DObject(GL_TRIANGLES, 1083, g_vertex_buffer_data, color, GL_FILL);
}

void Ball::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate          = rotate * glm::translate(glm::vec3(0, 0, 0)); //change gulshan
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Ball::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Ball::tick() {
    this->position.x -= speed;
}

void Ball:: accelearate()
{
    this->speed -= this->accel_y;
    this->position.y += this->speed - this->accel_y; 
    
}

void Ball::change_x()
{
    this->position.x += this->speed_x;
}

