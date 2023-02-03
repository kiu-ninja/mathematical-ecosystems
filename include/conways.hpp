#include <iostream>
#include <cmath>
#include <string>
#include "stage.hpp"
#include "easing.hpp"
#include "drawing_functions.hpp"

struct ConwaysState {
    DrawableRectangle a_cell;
    unsigned char a_cell_fill_alpha = 0;

    DrawableRectangle neighbors[8];
    bool neighbor_states[8];
    unsigned char neighbor_states_fill_alpha[8];

    DrawableString text;
    
    ConwaysState() {}
    ConwaysState(const ApplicationData &app_data) {
        float w = app_data.width, h = app_data.height;
        a_cell = DrawableRectangle(
            Rectangle { w / 2, h / 2, h / 3 - 20, h / 3 - 20 }
        );
        int count = 0;
        for (int j = -1; j <= 1; j++) {
            for (int i = -1; i <= 1; i++) {
                if (!(i == 0 && j == 0)) {
                    neighbors[count] = DrawableRectangle(
                        Rectangle { 
                            w / 2 + i * h / 3, 
                            h / 2 + j * h / 3, 
                            h / 3 - 20,
                            h / 3 - 20
                        }
                    );
                    neighbor_states[count] = false;
                    neighbor_states_fill_alpha[count] = 0;
                    count++;
                }
            }
        }

        neighbor_states[0] = true;
        neighbor_states[1] = true;
        neighbor_states[4] = true;
    }
};

struct WriteText: public Scene<ConwaysState> { using Scene::Scene;
    std::string target_text = "NO TARGET TEXT";

    void set_target_text(const std::string &_target_text) {
        target_text = _target_text;
    }

    void start(ConwaysState &s) {
        s.text.update_text(target_text);
    }

    void update_state(ConwaysState &s, const float &t) {
        s.text.t = Easing::cubic(0, 1, t);
    }
};

WriteText* write_text(const std::string &target_text) {
    WriteText* res = new WriteText(1); 
    res->set_target_text(target_text);
    return res;
}

namespace Scenes {
    struct ShowInitialCell: public Scene<ConwaysState> { using Scene::Scene;
        void update_state(ConwaysState &s, const float &t) {
            s.a_cell.t = Easing::cubic_cubic(0, 1, t);
        }
    };

    struct MakeCellAlive: public Scene<ConwaysState> { using Scene::Scene;
        void update_state(ConwaysState &s, const float &t) {
            s.a_cell_fill_alpha = Easing::cubic_cubic(0, 255, t);
        }
    };

    struct MakeCellDead: public Scene<ConwaysState> { using Scene::Scene;
        void update_state(ConwaysState &s, const float &t) {
            s.a_cell_fill_alpha = Easing::cubic_cubic(255, 0, t);
        }
    };

    struct ShowNeighbors: public Scene<ConwaysState> { using Scene::Scene;
        void update_state(ConwaysState &s, const float &t) {
            for (int i = 0; i < 8; i++) {
                s.neighbors[i].t = Easing::cubic_cubic(0, 1, t);
            }
        }
    };

    struct HideNeighbors: public Scene<ConwaysState> { using Scene::Scene;
        void update_state(ConwaysState &s, const float &t) {
            for (int i = 0; i < 8; i++) {
                s.neighbors[i].t = Easing::cubic_cubic(1, 0, t);
            }
        }
    };

    struct TransformCells: public Scene<ConwaysState> { using Scene::Scene;
        void update_state(ConwaysState &s, const float &t) {
            for (int i = 0; i < 8; i++) {
                s.neighbors[i].rect.x = Easing::cubic_cubic(initial_state.neighbors[i].rect.x, app_data.width / 2 + (initial_state.neighbors[i].rect.x - app_data.width / 2) * 0.5, t);
                s.neighbors[i].rect.y = Easing::cubic_cubic(initial_state.neighbors[i].rect.y, (app_data.height / 2 + (initial_state.neighbors[i].rect.y - app_data.height / 2) / 2) - 50, t);
                s.neighbors[i].rect.width = Easing::cubic_cubic(initial_state.neighbors[i].rect.width, initial_state.neighbors[i].rect.width / 2, t);
                s.neighbors[i].rect.height = Easing::cubic_cubic(initial_state.neighbors[i].rect.height, initial_state.neighbors[i].rect.height / 2, t);
            }
            s.a_cell.rect.width = Easing::cubic_cubic(initial_state.a_cell.rect.width, initial_state.a_cell.rect.width / 2, t);
            s.a_cell.rect.height = Easing::cubic_cubic(initial_state.a_cell.rect.height, initial_state.a_cell.rect.height / 2, t);
            s.a_cell.rect.y = Easing::cubic_cubic(initial_state.a_cell.rect.y, initial_state.a_cell.rect.y - 50, t);
        }
    };

    struct SomeAliveSomeDead: public Scene<ConwaysState> { using Scene::Scene;
        float time_neighbor_states_fill_alpha[8];

        void start() {
            for (int i = 0; i < 8; i++) {
                time_neighbor_states_fill_alpha[i] = randf() / 2;
            }
        }

        void update_state(ConwaysState &s, const float &t) {
            for (int i = 0; i < 8; i++) {
                float t_i = clamp(t - time_neighbor_states_fill_alpha[i], 0.0f, 0.5f) * 2;
                s.neighbor_states_fill_alpha[i] = 255 * Easing::cubic_cubic(0, 1, t_i);
            }
        }
    };
}

class Conways: public Stage<ConwaysState> {
public:
    Conways() : Stage(1920 / 2, 1080 / 2, "Conways Game of Life") { };

    void scene_setup() {
        add_scene(new Scenes::ShowInitialCell(1, 2.0f));

        add_scene_after_last(write_text("This is a cell."))->set_duration(1.0f);

        add_scene_after_last(new Scenes::MakeCellAlive(1.0f))->wait(1.0f);
        add_scene_with_last(write_text("Cells can be alive"));

        add_scene_after_last(new Scenes::MakeCellDead(1.0f))->wait(1.0f);
        add_scene_with_last(write_text("... or dead."));

        add_scene_after_last(new Scenes::TransformCells(1.0f))->wait(1.0f);
        add_scene_after_last(new Scenes::ShowNeighbors(1.0f));
        add_scene_with_last(write_text("Cells have neighbors"));

        add_scene_after_last(new Scenes::SomeAliveSomeDead(2.0f))->wait(1.0f);
        add_scene_with_last(write_text("some alive, some dead."));

        add_scene_after_last(new Scenes::HideNeighbors(2.0f))->wait(1.0f);
        add_scene_with_last(write_text("If the number of alive\nneighbors is exactly 3..."));
        add_scene_after_last(new Scenes::MakeCellAlive(2.0f))->wait(1.0f);
        add_scene_with_last(write_text("... the cell comes alive."));
    }

    Font font;
    int font_size = 40;
    void state_setup() {
        font = LoadFontEx("demo-font.otf", font_size, NULL, 0);
        state.text.font = font;
        state.text.font_size = font_size;
    }

    void background_update() { };


    void draw() {
        ClearBackground(BLACK);

        draw_rectangle_circle_bounded(state.a_cell.get_rect(), state.a_cell.get_occluder(), {255, 255, 255, (unsigned char)(255 - state.a_cell_fill_alpha)});

        DrawRectangleRec(state.a_cell.get_rect_padded(3), {220, 255, 200, state.a_cell_fill_alpha});

        DrawTextEx(
            font, 
            state.text.get_string().c_str(), 
            state.text.get_pos_centered_at({ (float)app_data.width / 2, (float)3 * app_data.height / 4 }), 
            font_size, 
            0, 
            WHITE
        );

        for (int i = 0; i < 8; i++) {
            draw_rectangle_circle_bounded(state.neighbors[i].get_rect(), state.neighbors[i].get_occluder(), {255, 255, 255, (unsigned char)(255 - state.neighbor_states_fill_alpha[i] * state.neighbor_states[i])});

            if (state.neighbor_states[i]) {
                Rectangle r = state.neighbors[i].get_rect_padded(3);
                DrawRectangleRec(r, {220, 255, 200, state.neighbor_states_fill_alpha[i]});
            }
        }
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

// struct ConwaysSimulation: public Scene<ConwaysState> { using Scene::Scene;
//     int current_frame = 0;

//     void update_state(ConwaysState &s, const float &t) {
//         if (current_frame % 18 == 0) {
//             int neighbor_counts[LIFE_GRID_SIZE + 2][LIFE_GRID_SIZE + 2];
//             for (int i = 0; i < LIFE_GRID_SIZE + 2; i++) {
//                 for (int j = 0; j < LIFE_GRID_SIZE + 2; j++) {
//                     neighbor_counts[i][j] = 0;
//                 }
//             }

//             for (int i = 0; i < LIFE_GRID_SIZE; i++) {
//                 for (int j = 0; j < LIFE_GRID_SIZE; j++) {
//                     if (s.life_grid[i][j]) {
//                         for (int p = 0; p < 3; p++) {
//                             for (int q = 0; q < 3; q++) {
//                                 neighbor_counts[i + p][j + q]++;
//                             }
//                         }
//                     }
//                 }
//             }

//             for (int i = 0; i < LIFE_GRID_SIZE; i++) {
//                 for (int j = 0; j < LIFE_GRID_SIZE; j++) {
//                     if (s.life_grid[i][j]) {
//                         if (neighbor_counts[i + 1][j + 1] - 1 != 2 && neighbor_counts[i + 1][j + 1] - 1 != 3) {
//                             s.life_grid[i][j] = false;
//                         }
//                     } else {
//                         if (neighbor_counts[i + 1][j + 1] == 3) {
//                             s.life_grid[i][j] = true;
//                         }
//                     }
//                 }
//             }
//         }

//         current_frame++;
//     }
// };