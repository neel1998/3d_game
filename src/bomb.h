#include "main.h"

#ifndef BOMB_H
#define BOMB_H


class Bomb {
public:
    Bomb() {}
    Bomb(float x, float y, float z, color_t color);
    glm::vec3 position;
    float rotationX;
    float rotationY;
    float rotationZ;
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
