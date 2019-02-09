#include "main.h"

#ifndef PARA_H
#define PARA_H


class Para {
public:
    Para() {}
    Para(float x, float y, float z, color_t color);
    glm::vec3 position;
    float rotationX;
    float rotationY;
    float rotationZ;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    double speed;
    bool collided;
private:
    VAO *object;
};

#endif // BALL_H
