#include <iostream>
#include <cmath>
#include <string>
#include "stage.hpp"
#include "easing.hpp"
#include "drawables.hpp"

struct ConwaysState {
    Drawable::Group<Drawable::GridCell> cells;

    Drawable::String subtitle_text;
    Drawable::String hint_text;
    
    ConwaysState() {}
    ConwaysState(const ApplicationData &app_data) {
        float w = app_data.width, h = app_data.height;
        for (int j = -1; j <= 1; j++) {
            for (int i = -1; i <= 1; i++) {
                cells.add(new Drawable::GridCell(Rectangle { w / 2 + i * h / 3, h / 2 + j * h / 3, h / 3 - 20, h / 3 - 20 }));
            }
        }

        hint_text.position = Vector2 { (float)app_data.width / 2, (float)app_data.height / 8 };
        hint_text.col = RED;
        subtitle_text.position = Vector2 { (float)app_data.width / 2, (float)3 * app_data.height / 4 };
    }
};

struct ToggleThreeNeighbors: public Scene<ConwaysState> { using Scene::Scene;
    void update_state(ApplicationWithState<ConwaysState> &a, const float &t) {
        int count = 0;
        for (int i = 0; i < 9; i++) {
            if (IsMouseButtonPressed(0)) {
                if (CheckCollisionPointRec(GetMousePosition(), a.state.cells[i]->get_stroke_rect())) {
                    a.state.cells[i]->alive = 1 - a.state.cells[i]->alive;
                }
            }

            if (a.state.cells[i]->alive > 0.5) count++;
        }

        if (count < 3) a.current_frame--;
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
        state.hint_text.font = font;
        state.hint_text.font_size = font_size;
    }

    void scene_setup() {
        add_scene(state.cells[4]->animate_visibility(1))->wait(1)->set_duration(2);

        add_scene_after_last(state.subtitle_text.write("This is a cell."));

        add_scene_after_last(state.subtitle_text.write("A cell can be alive"))->wait(2);
        add_scene_with_last(state.cells[4]->animate_alive(1));

        add_scene_after_last(state.subtitle_text.write("... or dead."))->wait(2);
        add_scene_with_last(state.cells[4]->animate_alive(0));

        add_scene_after_last(state.subtitle_text.write("Cells have 8 neighbors"))->wait(2);
        for (int i = 0; i < 9; i++)
            add_scene_with_last(state.cells[i]->animate_visibility(1));
        add_scene_with_last(state.cells.scale(Vector2{ 0.5f, 0.5f }));
        add_scene_with_last(state.subtitle_text.translate(Vector2 { 0, (float)app_data.height / 8 }))->set_duration(2);
        
        add_scene_after_last(state.subtitle_text.write("they can be alive"))->wait(2);
        for (int i = 0; i < 9; i++) if (i != 4)
            add_scene_with_last(state.cells[i]->animate_alive(1));

        add_scene_after_last(state.subtitle_text.write("... or dead."))->wait(2);
        for (int i = 0; i < 9; i++) if (i != 4)
            add_scene_with_last(state.cells[i]->animate_alive(0));

        add_scene_after_last(state.hint_text.write("Click on neighbor cells to\ntoggle their state."));
        add_scene_after_last(new ToggleThreeNeighbors());
        add_scene_after_last(state.hint_text.disappear());

        add_scene_after_last(state.subtitle_text.write("If the number of alive\nneighbors is exactly 3..."))->wait(2);
        add_scene_after_last(state.subtitle_text.write("the cell will come alive."))->wait(2);
        add_scene_after_last(state.cells[4]->animate_alive(1))->wait(0);

         
    }

    void background_update() { };

    void draw() {
        ClearBackground(BLACK);

        for (int i = 0; i < 9; i++) {
            state.cells[i]->draw();
        }

        state.subtitle_text.draw();
        state.hint_text.draw();
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