#include <iostream>
#include <cmath>
#include <string>
#include "stage.hpp"
#include "easing.hpp"
#include "drawables.hpp"

#define LG_SIZE 61
#define LG_HALF (LG_SIZE / 2)

using namespace Drawable;

struct ConwaysState {
    Group<GridCell> neighbors;
    Group<GridCell> cells;
    String subtitle_text;
    int simulation_speed = 30;

    Group<Arrow> arrows;

    GridCell* life_grid(int x, int y) {
        int i = (x + LG_HALF) % LG_SIZE;
        int j = (y + LG_HALF) % LG_SIZE;
        return cells[j * LG_SIZE + i];
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

struct ConwaysSimulation: public Scene<ConwaysState> { using Scene::Scene;
    int frame_count = 0;

    void update_state(ApplicationWithState<ConwaysState> *a, const float &t) {
        if (a->state.simulation_speed > 0 && frame_count > a->state.simulation_speed) {
            int neighbor_counts[LG_SIZE + 2][LG_SIZE + 2];
            for (int i = 0; i < LG_SIZE + 2; i++) {
                for (int j = 0; j < LG_SIZE + 2; j++) {
                    neighbor_counts[i][j] = 0;
                }
            }

            for (int i = 0; i < LG_SIZE; i++) {
                for (int j = 0; j < LG_SIZE; j++) {
                    if (a->state.life_grid(i-LG_HALF, j-LG_HALF)->alive > 0.5f) {
                        for (int p = 0; p < 3; p++) {
                            for (int q = 0; q < 3; q++) {
                                neighbor_counts[i + p][j + q]++;
                            }
                        }
                    }
                }
            }

            for (int i = 0; i < LG_SIZE; i++) {
                for (int j = 0; j < LG_SIZE; j++) {
                    if (a->state.life_grid(i-LG_HALF, j-LG_HALF)->alive > 0.5f) {
                        if (neighbor_counts[i + 1][j + 1] - 1 != 2 && neighbor_counts[i + 1][j + 1] - 1 != 3) {
                            a->state.life_grid(i-LG_HALF, j-LG_HALF)->alive = 0;
                        }
                    } else {
                        if (neighbor_counts[i + 1][j + 1] == 3) {
                            a->state.life_grid(i-LG_HALF, j-LG_HALF)->alive = 1;
                        }
                    }
                }
            }
            frame_count = 0;
        }

        frame_count++;
    }
};

class Conways: public Stage<ConwaysState> {
public:
    Conways() : Stage(1920 / 2, 1080 / 2, "Conways Game of Life") { 
        srand(10);
    };

    Font font;
    int font_size = 10;
    void state_setup() {
        font = LoadFontEx("demo-font.otf", font_size * 20, NULL, 0);
        state.subtitle_text.font = font;
        state.subtitle_text.font_size = font_size;
    }

    void scene_setup() {
        add_scene_builder([=] (SceneGroup* sg) {
            for (int i = 0; i < LG_SIZE * LG_SIZE; i++) {
                sg->merge(state.cells[i]->animate_visibility(1));
                sg->merge(state.cells[i]->scale(0.9));
            }
            sg->insert(new DebugScene("Should be called once"));
        })->set_duration(1);

        add_scene_after_last(state.cells.scale(4))->wait(2)->set_duration(3);

        add_scene_after_last(Interpolate::interpolate<int>(&state.simulation_speed, -1, Interpolate::Mode::LINEAR, Interpolate::Behavior::STATIC))->set_duration_frames(2);
        
        new_scene_group()->set_duration(2);
        for (int i = -LG_HALF; i <= LG_HALF; i++) {
            for (int j = -LG_HALF; j <= LG_HALF; j++) {
                if (!(abs(i) <= 1 && abs(j) <= 1))
                    merge_scene_with_last(state.life_grid(i, j)->animate_visibility(0));
            }
        }

        new_scene_group()->set_duration(2);
        merge_scene_with_last(state.neighbors.space_out(2.0f))->set_duration(1);
        for (int i = 0; i < state.neighbors.size(); i++)
            merge_scene_with_last(state.arrows[i]->from(state.life_grid(0, 0))->to(state.neighbors[i])->appear());
        
        new_scene_group()->set_duration(1);

        StatelessScene* s = new_scene_group();
        for (int i = 0; i < 8; i++) {
            s = add_scene(state.arrows[i]->disappear())->with(s)->wait(0.1)->set_duration(0.5);
        }

        new_scene_group()->set_duration(1)->wait(0.5);
        merge_scene_with_last(state.neighbors.space_out(0.5f));

        // Background scenes
        add_scene((new ConwaysSimulation()))->set_duration(100000);
    }

    void background_update() { };

    void draw() {
        ClearBackground(BLACK);

        state.cells.draw();
        state.subtitle_text.draw();
        state.arrows.draw();
        // state.hint_text.draw();
    }
}; 