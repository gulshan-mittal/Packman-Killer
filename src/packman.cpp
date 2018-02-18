#include "packman.h"
#include "main.h"

Packman::Packman(float x, float y, color_t color, double packradii) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    this->speed_x = 0.05;
    this->radius_pack = 0.5;
    this->life = 0;
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

        xx = packradii*cos((double)k*pi/180);
        yy = packradii*sin((double)k*pi/180);
        k++;

        g_vertex_buffer_data[j++] = xx;
        g_vertex_buffer_data[j++] = yy;
        g_vertex_buffer_data[j++] = 0.0f;
    }

    this->object = create3DObject(GL_TRIANGLES, 1083, g_vertex_buffer_data, color, GL_FILL);
}

void Packman::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate          = rotate * glm::translate(glm::vec3(0, 0, 0)); //change gulshan
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Packman::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Packman::tick() {
    this->position.x -= speed_x;
}

void Packman:: accelearate(){
    
    this->position.y += speed_x - 0.008; 
    speed_x -= 0.008;
}
