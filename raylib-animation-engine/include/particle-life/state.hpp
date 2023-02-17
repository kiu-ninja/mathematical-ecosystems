#include <cmath>
#include <iostream>
#include <string>
// #include "stage.hpp"
#include "drawables.hpp"
#include "easing.hpp"
#include "colors.hpp"

using namespace Drawables;

#define PL_N 10000
#define PL_KINDS 20

#define PL_INTERACTION_RADIUS 15.0f
#define PL_KIND_RULE_START 5.0f

struct Particle : public Ellipse {
    Vector2 velocity;
    int kind;
    Particle() : Ellipse(0, 0, 1, 1) {};
    Particle(float x, float y, int _kind)
        : Ellipse(x, y, 1, 1)
        , kind(_kind)
    {
        fill_col = HSV_to_RGB((float)kind / PL_KINDS * 300, 100, 100);
        velocity = Vector2{0, 0};
    }
};

struct PLState {
    Group particles;
    Particle* jeff;
    float interaction_rules[PL_KINDS][PL_KINDS];

    PLState() { }
    PLState(ApplicationData app_data)
    {
        for (int i = 0; i < PL_KINDS; i++) {
            for (int j = 0; j < PL_KINDS; j++) {
                interaction_rules[i][j] = i == j ? 1.0f : i == (j + 1) % PL_KINDS ? 0.5 : 0;//randf(-2, 2);
                // interaction_rules[i][j] = i == j ? 0.0f : randf(-1, 1);
            }
        }
        for (int i = 0; i < PL_N; i++) {
            particles.add(new Particle(
                rand() % app_data.width,
                rand() % app_data.height,
                rand() % PL_KINDS));
        }
        jeff = particles.get<Particle>(0);
    }
};