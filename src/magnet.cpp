#include "magnet.h"
#include "main.h"

Magnet::Magnet(float x, float y, color_t color, float rotor) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = rotor;
    this->num  = 0;

    float rad1= 0.6, rad2 = 0.4;
    static GLfloat vertex_buffer_data[4000];
    float curr_i = rad1, curr_j = 0.0, curr_a = rad2, curr_b = 0.0f;
    float i = curr_i, j = curr_j, a = rad2, b = 0.0f;
    float temp_i = i, temp_j = j, temp_a = a, temp_b = b;
    int vertex_count = 0, k = -1;
    for(k=-1;k> -181;k--)
    {
        temp_i = i, temp_j = j, temp_a = a, temp_b = b;
        i = rad1 * cos((float)k*M_PI/180);
        j = rad1 * sin((float)k*M_PI/180);
        a = rad2 * cos((float)k*M_PI/180);
        b = rad2 * sin((float)k*M_PI/180);

        
        vertex_buffer_data[vertex_count] = temp_i;
        vertex_count++;
        vertex_buffer_data[vertex_count] = temp_j;
        vertex_count++;
        vertex_buffer_data[vertex_count] = 0.0f;
        vertex_count++;
        
        vertex_buffer_data[vertex_count] = temp_a ,  vertex_count++;
        vertex_buffer_data[vertex_count] = temp_b,  vertex_count++;
        vertex_buffer_data[vertex_count] = 0.0f ,  vertex_count++;

       
        vertex_buffer_data[vertex_count] = a , vertex_count++;
        vertex_buffer_data[vertex_count] = b ,  vertex_count++;
        vertex_buffer_data[vertex_count] = 0.0f , vertex_count++;

        
        vertex_buffer_data[vertex_count] = temp_i,  vertex_count++;
        vertex_buffer_data[vertex_count] = temp_j ,  vertex_count++;
        vertex_buffer_data[vertex_count] = 0.0f,  vertex_count++;

        
        vertex_buffer_data[vertex_count] = i , vertex_count++;
        vertex_buffer_data[vertex_count] = j , vertex_count++;
        vertex_buffer_data[vertex_count] = 0.0f , vertex_count++;

        
        vertex_buffer_data[vertex_count] = a , vertex_count++;
        vertex_buffer_data[vertex_count] = b,  vertex_count++;
        vertex_buffer_data[vertex_count] = 0.0f,  vertex_count++;

    }
   
    vertex_buffer_data[vertex_count] = -curr_i;
    vertex_count++;
    vertex_buffer_data[vertex_count] = curr_j;
    vertex_count++;
    vertex_buffer_data[vertex_count] = 0.0f;
    vertex_count++;

    vertex_buffer_data[vertex_count] = -curr_a;
    vertex_count++ ;
    vertex_buffer_data[vertex_count] = curr_b;
    vertex_count++ ;
    vertex_buffer_data[vertex_count] = 0.0f;
    vertex_count++;
    
    vertex_buffer_data[vertex_count] = -curr_i;
    vertex_count++;
    vertex_buffer_data[vertex_count] = 0.6f;
    vertex_count++;
    vertex_buffer_data[vertex_count] = 0.0f;
    vertex_count++;

    vertex_buffer_data[vertex_count] = -curr_i;
    vertex_count++;
    vertex_buffer_data[vertex_count] = 0.6f;
    vertex_count++;
    vertex_buffer_data[vertex_count] = 0.0f;
    vertex_count++;

    vertex_buffer_data[vertex_count] = -curr_a;
    vertex_count++;
    vertex_buffer_data[vertex_count] = curr_b;
    vertex_count++;
    vertex_buffer_data[vertex_count] = 0.0f;
    vertex_count++;

    vertex_buffer_data[vertex_count] = -curr_a;
    vertex_count++;
    vertex_buffer_data[vertex_count] = 0.6f;
    vertex_count++;
    vertex_buffer_data[vertex_count] = 0.0f;
    vertex_count++;
    
    vertex_buffer_data[vertex_count] =  curr_i;
    vertex_count++;
    vertex_buffer_data[vertex_count] = curr_j;
    vertex_count++;
    vertex_buffer_data[vertex_count] = 0.0f;
    vertex_count++;
    vertex_buffer_data[vertex_count] =  curr_a;
    vertex_count++;
    vertex_buffer_data[vertex_count] = curr_b;
    vertex_count++;
    vertex_buffer_data[vertex_count] = 0.0f;
    vertex_count++;
    vertex_buffer_data[vertex_count] =  curr_i;
    vertex_count++;
    vertex_buffer_data[vertex_count] = 0.6f;
    vertex_count++;
    vertex_buffer_data[vertex_count] = 0.0f;
    vertex_count++;
    
    vertex_buffer_data[vertex_count] =  curr_i;
    vertex_count++;
    vertex_buffer_data[vertex_count] = 0.6f;
    vertex_count++; 
    vertex_buffer_data[vertex_count] = 0.0f;
    vertex_count++;
    vertex_buffer_data[vertex_count] =  curr_a;
    vertex_count++;
    vertex_buffer_data[vertex_count] = curr_b;
    vertex_count++;
    vertex_buffer_data[vertex_count] = 0.0f;
    vertex_count++;
    vertex_buffer_data[vertex_count] =  curr_a;
    vertex_count++;
    vertex_buffer_data[vertex_count] = 0.6f;
    vertex_count++;
    vertex_buffer_data[vertex_count] = 0.0f;
    vertex_count++;

    this->object = create3DObject(GL_TRIANGLES, vertex_count/3, vertex_buffer_data, color, GL_FILL);
}

void Magnet::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}