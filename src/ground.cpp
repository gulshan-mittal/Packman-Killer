#include "ground.h"
#include "main.h"

Ground::Ground(float x, float y, color_t color, float psuedo_pt, float psuedo_ht, float x1, float x2)
{
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    GLfloat vertex_buffer_data[] = {
        x2, psuedo_pt + psuedo_ht, 0, // vertex 1
        x1,  psuedo_pt + psuedo_ht, 0, // vertex 2
        x2, psuedo_pt - psuedo_ht, 0, // vertex 3

        x1,  psuedo_pt - psuedo_ht, 0, // vertex 3
        x2, psuedo_pt - psuedo_ht, 0, // vertex 4
        x1,  psuedo_pt + psuedo_ht , 0 // vertex 1
    };

    this->object = create3DObject(GL_TRIANGLES, 6, vertex_buffer_data, color, GL_FILL);
}


void Ground::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate          = rotate * glm::translate(glm::vec3(0, 0, 0)); //change gulshan
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Ground::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}




