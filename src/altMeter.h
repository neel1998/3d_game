#include "main.h"

#ifndef ALTMETER_H
#define ALTMETER_H


class AltMeter {
public:
    AltMeter() {}
    AltMeter(float x, float y, float z, color_t color);
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
