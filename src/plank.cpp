#include "plank.h"
#include "main.h"
#include "math.h"

Plank::Plank(float x, float y, float speed, float rotation)
{
    this->position = glm::vec3(x, y, 0);
    this->rotation = rotation;
    this->speed_x = speed;
    this->speed_y = 0;

    static const GLfloat vertex_buffer_data[] = 
    {
        0, 0, 0, // vertex 1
        0.5,  0, 0, // vertex 2
        0.5,  0.07, 0, // vertex 3

        0, 0.07, 0, // vertex 1
        0.5,  0.07, 0, // vertex 2
        0,  0, 0, // vertex 3

        0, 0, 0, // vertex 1
        -0.6,  0, 0, // vertex 2
        -0.6,  0.07, 0, // vertex 3

        0, 0.07, 0, // vertex 1
        -0.6,  0.07, 0, // vertex 2
        0,  0, 0, // vertex 3

    };


    this->object = create3DObject(GL_TRIANGLES, 12, vertex_buffer_data, COLOR_GREY, GL_FILL);
}

void Plank::draw(glm::mat4 VP) 
{
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);   
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Plank::tick()
{
    this->position.x -= this->speed_x; 
}