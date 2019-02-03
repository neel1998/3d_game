#include "main.h"

#ifndef INDICATOR_H
#define INDICATOR_H


class Indicator {
public:
    Indicator() {}
    Indicator(float x, float y, float z, color_t color);
    glm::vec3 position;
    float rotationX;
    float rotationY;
    float rotationZ;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    
    double speed;
private:
    VAO *object;
};

#endif // BALL_H
