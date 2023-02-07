#include <iostream>
#include <cmath>
#include <string>
#include "stage.hpp"
#include "easing.hpp"
#include "drawables.hpp"

#define LG_SIZE 61
#define LG_HALF (LG_SIZE / 2)

struct ConwaysState {
    Drawable::GridCell* _life_grid[LG_SIZE][LG_SIZE];
    Drawable::Group<Drawable::GridCell> neighbors;
    Drawable::Group<Drawable::GridCell> cells;
    Drawable::String subtitle_text;
    int simulation_speed = 10;

    Drawable::Group<Drawable::Arrow> arrows;

    Drawable::GridCell* life_grid(int x, int y) {
        return _life_grid[(y + LG_HALF) % LG_SIZE][(x + LG_HALF) % LG_SIZE];
    }

    ConwaysState() {}
    ConwaysState(ApplicationData app_data) {
        for (int i = 0; i < LG_SIZE; i++) {
            for (int j = 0; j < LG_SIZE; j++) {
                Drawable::GridCell* cell = new Drawable::GridCell(
                    Rectangle {
                        (float)(j - LG_HALF) * app_data.width / LG_SIZE + app_data.width / 2,
                        (float)(i - LG_HALF) * app_data.width / LG_SIZE + app_data.height / 2,
                        (float)app_data.width / LG_SIZE,
                        (float)app_data.width / LG_SIZE
                    }
                );
                cells.add(cell);
                _life_grid[i][j] = cell;
                cell->alive = rand() % 2 == 0 ? 1.0f : 0.0f;
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

struct SetNeighborsAlive: public Scene<ConwaysState> { using Scene::Scene;
    float target = -1;
    int target_amount_of_alive_cells = -1;

    SetNeighborsAlive* set_target(float _target) {
        target = _target;
        return this;
    }

    SetNeighborsAlive* set_target_amount_of_alive_cells(float _target_amount_of_alive_cells) {
        target_amount_of_alive_cells = _target_amount_of_alive_cells;
        return this;
    }

    void update_state(const float &t) override {

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
                    if (a->state._life_grid[i][j]->alive > 0.5f) {
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
                    if (a->state._life_grid[i][j]->alive > 0.5f) {
                        if (neighbor_counts[i + 1][j + 1] - 1 != 2 && neighbor_counts[i + 1][j + 1] - 1 != 3) {
                            a->state._life_grid[i][j]->alive = 0.0f;
                        }
                    } else {
                        if (neighbor_counts[i + 1][j + 1] == 3) {
                            a->state._life_grid[i][j]->alive = 1.0f;
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
    Conways() : Stage(1920 / 2, 1080 / 2, "Conways Game of Life") { };

    Font font;
    int font_size = 20;
    void state_setup() {
        font = LoadFontEx("demo-font.otf", font_size * 20, NULL, 0);
        state.subtitle_text.font = font;
        state.subtitle_text.font_size = font_size;
        // state.hint_text.font = font;
        // state.hint_text.font_size = font_size;
    }

    void scene_setup() {
        new_scene_group()->set_duration(1);
        for (int i = 0; i < LG_SIZE*LG_SIZE; i++) {
            add_scene_to_last(((Drawable::GridCell*)state.cells[i])->animate_visibility(1));//->wait(0.017f * (i % 10 == 0));
            add_scene_to_last(state.cells[i]->scale(0.8));
        }

        add_scene_after_last(state.cells.scale(4.0f)->set_duration(3));
        // merge_scene_with_last(Interpolate::interpolate<int>(&state.simulation_speed, 30, Interpolate::Mode::LINEAR, Interpolate::Behavior::STATIC));
        // add_scene_after_last(Interpolate::interpolate<int>(&state.simulation_speed, -1, Interpolate::Mode::LINEAR, Interpolate::Behavior::STATIC))->set_duration_frames(2);
        new_scene_group()->set_duration(2);
        for (int i = -LG_HALF; i <= LG_HALF; i++) {
            for (int j = -LG_HALF; j <= LG_HALF; j++) {
                if (!(abs(i) <= 1 && abs(j) <= 1))
                    merge_scene_with_last(state.life_grid(i, j)->animate_visibility(0));
            }
        }

        new_scene_group()->set_duration(2);
        merge_scene_with_last(state.neighbors.space_out(2.0f))->set_duration(1);
        for (int i = 0; i < 8; i++)
            merge_scene_with_last(state.arrows[i]->from(state.life_grid(0, 0))->to(state.neighbors[i])->appear());

        new_scene_group()->wait(1)->set_duration(2);
        merge_scene_with_last(state.neighbors.space_out(0.5f))->set_duration(1);
        for (int i = 0; i < 8; i++)
            merge_scene_with_last(state.arrows[i]->disappear());

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


//////  ! ~ USEFUL CODE ~ !


// void fill_cell(const Vector2 &center, const Rectangle &bounds, const float &cell_size, const int &i, const int &j) {
//     const float half_cell_size = cell_size / 2;
//     float x1 = center.x - half_cell_size + i * cell_size + 2;
//     float y1 = center.y - half_cell_size + j * cell_size + 2;
//     float x2 = center.x + half_cell_size + i * cell_size - 2;
//     float y2 = center.y + half_cell_size + j * cell_size - 2;

//     if (x2 > bounds.x && y2 > bounds.y && x1 < bounds.x + bounds.width && y1 < bounds.y + bounds.height) {
//         x1 = std::max(x1, bounds.x);
//         x2 = std::min(x2, bounds.x + bounds.width);
//         y1 = std::max(y1, bounds.y);
//         y2 = std::min(y2, bounds.y + bounds.height);

//         DrawRectangle(x1, y1, x2 - x1, y2 - y1, {100, 200, 255, 255});
//     }
// }