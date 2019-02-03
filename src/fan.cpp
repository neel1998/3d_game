#include "main.h"
#include "fan.h"

Fan::Fan(float x, float y, float z, color_t color) {
    this->position = glm::vec3(x, y, z);
    this->rotationX = 0;
    this->rotationY = 0;
    this->rotationZ = 0;
    speed = 1;
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    GLfloat vertex_buffer_data[] = {
        -0.2f, -1.0f,0,
        -0.2f, 1.0f, 0,
         0.2f, -1.0f,0,
        -0.2f, 1.0f, 0,
         0.2f, -1.0f,0,
        0.2f, 1.0f, 0,
    }; 

    this->object = create3DObject(GL_TRIANGLES, 6, vertex_buffer_data, color, GL_FILL);
}

void Fan::draw(glm::mat4 VP) {
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

void Fan::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Fan::tick() {
    // this->rotationZ += 20.0f;
    // this->rotation += speed;
    // this->position.x -= speed;
    // this->position.y -= speed;
}
void Fan :: forward() {
    this->position.z -= 0.5*cos(this->rotationY * M_PI / 180.0f);
    this->position.x -= 0.5*sin(this->rotationY * M_PI / 180.0f);
    // this->position. += 0.1;
}
void Fan::tiltL() {
     if (this->rotationZ < 30) {
        this->rotationZ += 1;
    }
}
void Fan::tiltR() {
   if (this->rotationZ > -30) {
        this->rotationZ -= 1;
    }
}
void Fan::rotateR() {
    if (this->rotationZ > -30) {
        this->rotationZ -= 1;
    }
    this->rotationY -= 0.5f;
}
void Fan::rotateL() {
    if (this->rotationZ < 30) {
        this->rotationZ += 1;
    }
    this->rotationY += 0.5f;
}
void Fan :: boost() {
    this->position.y += 0.1f;
}