#include "main.h"

#ifndef PLANE_H
#define PLANE_H


class Plane {
public:
    Plane() {}
    Plane(float x, float y, float z, color_t color);
    glm::vec3 position;
    float rotationX;
    float rotationY;
    float rotationZ;
    float rotationFan;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    void boost();
    void forward();
    void tiltL();
    void tiltR();
    void rotateR();
    void rotateL();
    double speed;
    float acc;
private:
    VAO *object;
};

#endif // BALL_H
