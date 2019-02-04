#include "main.h"

#ifndef BULLET_H
#define BULLET_H


class Bullet {
public:
    Bullet() {}
    Bullet(float x, float y, float z, float refX, float refY, float refZ, color_t color);
    glm::vec3 position;
    float rotationX;
    float rotationY;
    float rotationZ;

    float angle1;
    float angle2;
    float angle3;

    float refX;
    float refY;
    float refZ;

    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    void forward();
    void tiltL();
    void tiltR();
    void rotateR();
    void rotateL();
    void boost();
    double speed;
    float gravity;
private:
    VAO *object;
};

#endif // BALL_H
