#include "main.h"

#ifndef PLANE_H
#define PLANE_H


class Plane {
public:
    Plane() {}
    Plane(float x, float y, color_t color);
    glm::vec3 position;
    float rotationX;
    float rotationY;
    float rotationZ;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    void tiltL();
    void tiltR();
    void rotateR();
    void rotateL();
    double speed;
private:
    VAO *object;
};

#endif // BALL_H
