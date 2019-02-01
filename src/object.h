#include "main.h"

#ifndef OBJECT_H
#define OBJECT_H


class Object {
public:
    Object() {}
    Object(float x, float y, color_t color);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    double speed;
private:
    VAO *object;
};

#endif // BALL_H
