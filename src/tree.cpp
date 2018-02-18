#include "tree.h"
#include "main.h"
#include "math.h"

Trees::Trees(float x, float y, float max, float min)
{
    this->position = glm::vec3(x, y, 0);
    this->mini = min;
    this->maxi = max;

    static const GLfloat vertex_buffer_data[] = 
    {
        0, 0, 0, // vertex 1
        0,  0.42, 0, // vertex 2
        0.11,  0, 0, // vertex 3

        0, 0, 0, // vertex 1
        0,  0.42, 0, // vertex 2
        -0.11,  0, 0, // vertex 3
    };


    this->object = create3DObject(GL_TRIANGLES, 6, vertex_buffer_data, COLOR_TREE, GL_FILL);
}

void Trees::draw(glm::mat4 VP) 
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
