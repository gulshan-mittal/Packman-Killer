#include "porcupine.h"
#include "main.h"
#include "math.h"

Porcupine::Porcupine(float x, float y, double speed_x, float max, float min, color_t color)
{
    this->position = glm::vec3(x, y, 0);
    this->speed_x = speed_x;
    this->range.left_x = min;
    this->range.right_x = max;

    static const GLfloat vertex_buffer_data[] = 
    {
        0, 0, 0, // vertex 1
        0,  0.25, 0, // vertex 2
        0.15,  0, 0, // vertex 3

        0.15, 0, 0, // vertex 1
        0.30,  0.25, 0, // vertex 2
        0.45,  0, 0, // vertex 3

        0.45,  0, 0, // vertex 3
        0.60, 0.25, 0, // vertex 4
        0.75, 0, 0, // vertex 1

        0.75,  0, 0, // vertex 3
        0.9, 0.25, 0, // vertex 4
        0.9, 0, 0 // vertex 1
    };


    this->object = create3DObject(GL_TRIANGLES, 12, vertex_buffer_data, color, GL_FILL);
}

void Porcupine::draw(glm::mat4 VP) 
{
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);   
    glm::mat4 rotate    = glm::rotate((float) (0 * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Porcupine::tick() 
{
    this->position.x += this->speed_x;
    if (this->position.x <= this->range.left_x || this->position.x >=  this->range.right_x )
    {
        this->speed_x = - this->speed_x;
    }
}