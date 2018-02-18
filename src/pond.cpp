#include "pond.h"
#include "main.h"

Pond::Pond(float x, float y, color_t color, float rad, int tr_num) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    this->radius_pond = rad;    
    GLfloat g_vertex_buffer_data[4000];
    int j=0,k=1;
    double pi = 3.14257;
    double xx=0.4;
    double yy = 0.0;
    for(int i=0;i<tr_num;i++)
    {
        g_vertex_buffer_data[j++] = 0.0f;
        g_vertex_buffer_data[j++] = 0.0f;
        g_vertex_buffer_data[j++] = 0.0f;

        g_vertex_buffer_data[j++] = xx;
        g_vertex_buffer_data[j++] = yy;
        g_vertex_buffer_data[j++] = 0.0f;

        xx= rad*cos(-(double)k*pi/360);
        yy= rad*sin(-(double)k*pi/360);
        k++;

        g_vertex_buffer_data[j++] = xx;
        g_vertex_buffer_data[j++] = yy;
        g_vertex_buffer_data[j++] = 0.0f;
    }

    this->object = create3DObject(GL_TRIANGLES, 3*tr_num, g_vertex_buffer_data, color, GL_FILL);
}

void Pond::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate          = rotate * glm::translate(glm::vec3(0, 0, 0)); //change gulshan
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Pond::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

bounding_box_t Pond::bounding_box()
{
    float x = this->position.x;
    float y = this->position.y;
    double radius_pond1 = this->radius_pond;
    bounding_box_t pond_box = { x, y, radius_pond1};
    return pond_box;
}