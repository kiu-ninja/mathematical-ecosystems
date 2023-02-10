#include <iostream>
#include <cmath>
#include <string>
#include "stage.hpp"
#include "easing.hpp"
#include "drawables.hpp"

using namespace Drawables;

#define LG_SIZE 61
#define LG_HALF (LG_SIZE / 2)

struct ConwaysState {
    Group neighbors;
    Group cells;
    String subtitle_text;
    int simulation_speed = 30;

    Group arrows;

    GridCell* life_grid(int x, int y) {
        int i = (x + LG_HALF) % LG_SIZE;
        int j = (y + LG_HALF) % LG_SIZE;
        return cells.get<GridCell>(j * LG_SIZE + i);
    }

    ConwaysState() {}
    ConwaysState(ApplicationData app_data) {
        for (int i = 0; i < LG_SIZE; i++) {
            for (int j = 0; j < LG_SIZE; j++) {
                cells.add(new GridCell(
                    Rectangle {
                        (float)(j - LG_HALF) * app_data.width / LG_SIZE + app_data.width / 2,
                        (float)(i - LG_HALF) * app_data.width / LG_SIZE + app_data.height / 2,
                        (float)app_data.width / LG_SIZE,
                        (float)app_data.width / LG_SIZE
                    }
                ));
            }
        }
        life_grid(-3, -2)->alive = 1;
        life_grid(-2, -3)->alive = 1;
        life_grid(-3, -4)->alive = 1;
        life_grid(-2, -2)->alive = 1;
        life_grid(-4, -2)->alive = 1;

        neighbors.add(life_grid(-1, -1));
        neighbors.add(life_grid(0, -1));
        neighbors.add(life_grid(1, -1));
        neighbors.add(life_grid(1, 0));
        neighbors.add(life_grid(1, 1));
        neighbors.add(life_grid(0, 1));
        neighbors.add(life_grid(-1, 1));
        neighbors.add(life_grid(-1, 0));
    }
};