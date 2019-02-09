#include "volcano.h"
#include "main.h"

Volcano::Volcano(float x, float y, float z, color_t color) {
    this->position = glm::vec3(x, y, z);
    this->rotation = 0;
    this->collided = false;
    speed = 1;

    // int z = rand()%1000 - 500;
    // int xx = rand()%1000 - 500;
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    float height = 10.0;
    float length = 10.0;
    float width = 20.0;

    
    GLfloat colors[36*3];
    for (int i = 6*3; i < 12*3 ; i += 3) {
        colors[i] = 117.0/255.0;
        colors[i + 1] = 64.0/255.0;
        colors[i + 2] = 3.0/255.0;
    }

    for (int i = 0; i < 6*3 ; i += 3) {
        colors[i] = 38.0/255.0;
        colors[i + 1] = 20.0/255.0;
        colors[i + 2] = 0;
    }

    static const GLfloat vertex_buffer_data[] = {
        -length,0,-length,
        length, 0, -length,
        0, height, 0,
       
        -length,0,length,
        length, 0, length,
        0, height, 0,

       -length,0,-length,
        -length, 0, length,
        0, height, 0,


        length,0,-length,
        length, 0, length,
        0, height, 0, 

    };

    
    this->object = create3DObject(GL_TRIANGLES, 12, vertex_buffer_data, colors, GL_FILL);
}

void Volcano::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(1, 0, 0));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Volcano::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Volcano::tick() {
    // this->rotation += speed;
    // this->position.x -= speed;
    // this->position.y -= speed;
}
