#include "main.h"
#include "missile.h"

Missile::Missile(float x, float y, float z, color_t color) {
    this->position = glm::vec3(x, y, z);
    this->rotationX = 0;
    this->rotationY = 0;
    this->rotationZ = 0;
    speed = 1;
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    int total = 0;
    int n = 100;
    float r = 0.2f;
    float phi = 0;
    float theta = (2*3.14)/n;
    GLfloat vertex_buffer_data[100000]; 
    GLfloat colors[100000];
    for (int i = 0; i < 9*n; i+=9){
        vertex_buffer_data[i] = 0.0f;
        vertex_buffer_data[i + 1] = 0.0f;
        vertex_buffer_data[i + 2] = -2.0f;

        vertex_buffer_data[i + 3] = r*cos(phi);
        vertex_buffer_data[i + 4] = r*sin(phi);
        vertex_buffer_data[i + 5] = 0.0f;

        vertex_buffer_data[i + 6] = r*cos(phi + theta);
        vertex_buffer_data[i + 7] = r*sin(phi + theta);
        vertex_buffer_data[i + 8] = 0.0f;

        phi += theta;            
    }
    total += n*3;
    phi = 0;
    theta = (2*3.14)/n;
    float length = 1.0f;
    for (int i = 9*n; i < 9*n + 18*n; i+=18){
        vertex_buffer_data[i] = r*cos(phi);
        vertex_buffer_data[i + 1] = r*sin(phi);
        vertex_buffer_data[i + 2] = length;

        vertex_buffer_data[i + 3] = r*cos(phi);
        vertex_buffer_data[i + 4] = r*sin(phi);
        vertex_buffer_data[i + 5] = 0.0f;

        vertex_buffer_data[i + 6] = r*cos(phi + theta);
        vertex_buffer_data[i + 7] = r*sin(phi + theta);
        vertex_buffer_data[i + 8] = 0.0f;

        vertex_buffer_data[i + 9] = r*cos(phi);
        vertex_buffer_data[i + 10] = r*sin(phi);
        vertex_buffer_data[i + 11] = length;

        vertex_buffer_data[i + 12] = r*cos(phi + theta);
        vertex_buffer_data[i + 13] = r*sin(phi + theta);
        vertex_buffer_data[i + 14] = 0.0f;

        vertex_buffer_data[i + 15] = r*cos(phi + theta);
        vertex_buffer_data[i + 16] = r*sin(phi + theta);
        vertex_buffer_data[i + 17] = length;


        phi += theta;            
    }
    total += n*6;

    this->object = create3DObject(GL_TRIANGLES, total, vertex_buffer_data, color, GL_FILL);
}

void Missile::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotationX * M_PI / 180.0f), glm::vec3(1, 0, 0));

    rotate *= glm::rotate((float) (this->rotationY * M_PI / 180.0f), glm::vec3(0, 1, 0));
    rotate *= glm::rotate((float) (this->rotationZ * M_PI / 180.0f), glm::vec3(0, 0, 1));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Missile::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Missile::tick() {
    this->forward();
    // this->rotationZ += 20.0f;
    // this->rotation += speed;
    // this->position.x -= speed;
    // this->position.y -= speed;
}
void Missile :: forward() {
    this->position.z -= 0.8*cos(this->rotationY * M_PI / 180.0f);
    this->position.x -= 0.8*sin(this->rotationY * M_PI / 180.0f);
    // this->position. += 0.1;
}
