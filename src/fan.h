#include "main.h"

#ifndef FAN_H
#define FAN_H


class Fan {
public:
    Fan() {}
    Fan(float x, float y, float z, color_t color);
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
private:
    VAO *object;
};

#endif // BALL_H
