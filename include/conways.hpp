#include <iostream>
#include <cmath>
#include <string>
#include "stage.hpp"
#include "easing.hpp"
#include "drawing_functions.hpp"

struct ConwaysState {
    std::vector<Drawable::GridCell*> cells;

    Drawable::String text;
    
    ConwaysState() {}
    ConwaysState(const ApplicationData &app_data) {
        float w = app_data.width, h = app_data.height;
        cells.push_back(new Drawable::GridCell(
            Rectangle { w / 2, h / 2, h / 3 - 20, h / 3 - 20 }
        ));
        for (int j = -1; j <= 1; j++) {
            for (int i = -1; i <= 1; i++) {
                if (!(i == 0 && j == 0)) {
                    cells.push_back(new Drawable::GridCell(
                        Rectangle { 
                            w / 2 + i * h / 3, 
                            h / 2 + j * h / 3, 
                            h / 3 - 20,
                            h / 3 - 20
                        }
                    ));
                }
            }
        }

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

Scene<ConwaysState>* write_text(const std::string &target_text) {
    WriteText* res = new WriteText(1); 
    res->set_target_text(target_text);
    return res->set_duration(target_text.size() / 10.0f);
}

namespace Scenes {
    struct ShowInitialCell: public Scene<ConwaysState> { using Scene::Scene;
        void update_state(ConwaysState &s, const float &t) {
            s.cells[0]->t = Easing::cubic_cubic(0, 1, t);
        }
    };

    struct ToggleNeighborVisibility: public Scene<ConwaysState> { using Scene::Scene;
        float target;
        
        void start() {
            target = (initial_state.cells[1]->t > 0.5f) ? 0.0f : 1.0f;
        }

        void update_state(ConwaysState &s, const float &t) {
            for (int i = 1; i < 9; i++) {
                s.cells[i]->t = Easing::cubic_cubic(1.0f - target, target, t);
            }
        }
    };
}

class Conways: public Stage<ConwaysState> {
public:
    Conways() : Stage(1920 / 2, 1080 / 2, "Conways Game of Life") { };

    void scene_setup() {
        add_scene(new Scenes::ShowInitialCell(1, 1.0f));

        add_scene_after_last(write_text("This is a cell."))->set_duration(1.0f);

        add_scene_after_last(Drawable::GridCell::animate_alive<ConwaysState>(state.cells[0], 1))->wait(1.0f);
        add_scene_with_last(write_text("Cells can be alive"));

        add_scene_after_last(Drawable::GridCell::animate_alive<ConwaysState>(state.cells[0], 0))->wait(1.0f);
        add_scene_with_last(write_text("... or dead.")->set_duration(1.f));

        add_scene_after_last(new Scenes::ToggleNeighborVisibility(1.0f))->wait(1);
        add_scenes_with_last(Drawable::scale_together<ConwaysState, Drawable::GridCell>(state.cells, Vector2{0.5f, 0.5f}));
        add_scenes_with_last(Drawable::translate_together<ConwaysState, Drawable::GridCell>(state.cells, Vector2{0, (float)-app_data.height / 6}));
        
        add_scene_with_last(write_text("Cells have neighbors"));

        // add_scene_after_last(new Scenes::SomeAliveSomeDead(2.0f))->wait(1.0f);
        add_scene_after_last(Drawable::GridCell::animate_alive<ConwaysState>(state.cells[1], 1))->wait(1.0f);
        add_scene_with_last(Drawable::GridCell::animate_alive<ConwaysState>(state.cells[2], 1));
        add_scene_with_last(Drawable::GridCell::animate_alive<ConwaysState>(state.cells[5], 1));
        add_scene_with_last(write_text("some alive, some dead."));

        add_scene_after_last(write_text("If the number of alive\nneighbors is exactly 3..."))->wait(1.0f);
        add_scene_after_last(write_text("the cell will come alive."))->wait(3.0f);
        add_scene_after_last(Drawable::GridCell::animate_alive<ConwaysState>(state.cells[0], 1))->wait(1.0f);
    }

    Font font;
    int font_size = 20;
    void state_setup() {
        font = LoadFontEx("demo-font.otf", font_size * 20, NULL, 0);
        state.text.font = font;
        state.text.font_size = font_size;
    }

    void background_update() { };


    void draw() {
        ClearBackground(BLACK);

        for (int i = 0; i < 9; i++) {
            draw_rectangle_circle_bounded(
                state.cells[i]->get_stroke_rect(), 
                state.cells[i]->get_occluder(), 
                state.cells[i]->get_stroke_col()
            );
            DrawRectangleRec(
                state.cells[i]->get_fill_rect(), 
                state.cells[i]->get_fill_col()
            );
        }

        DrawTextEx(
            font, 
            state.text.get_string().c_str(), 
            state.text.get_pos_centered_at({ (float)app_data.width / 2, (float)4 * app_data.height / 5 }), 
            font_size, 
            0, 
            WHITE
        );
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