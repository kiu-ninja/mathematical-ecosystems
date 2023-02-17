#include <cmath>
#include <iostream>
#include <string>

#include "application/application_with_state.hpp"
#include "conways/state.hpp"
#include "drawables.hpp"
#include "easing.hpp"
#include "stage/scene.hpp"
#include "stage/stage.hpp"

using namespace Drawables;

struct ConwaysSimulation : public Scene::Scene {
    using Scene::Scene;
    int frame_count = 0;
    ConwaysState* cs;

    ConwaysSimulation(ConwaysState* new_cs) { cs = new_cs; }

    Scene* begin() override { return this; }
    Scene* finish() override { return this; }

    void act() override
    {
        if (cs->simulation_speed > 0 && frame_count > cs->simulation_speed) {
            int neighbor_counts[LG_SIZE + 2][LG_SIZE + 2];
            for (int i = 0; i < LG_SIZE + 2; i++) {
                for (int j = 0; j < LG_SIZE + 2; j++) {
                    neighbor_counts[i][j] = 0;
                }
            }

            for (int i = 0; i < LG_SIZE; i++) {
                for (int j = 0; j < LG_SIZE; j++) {
                    if (cs->life_grid(i - LG_HALF, j - LG_HALF)->alive > 0.5f) {
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
                    if (cs->life_grid(i - LG_HALF, j - LG_HALF)->alive > 0.5f) {
                        if (neighbor_counts[i + 1][j + 1] - 1 != 2 && neighbor_counts[i + 1][j + 1] - 1 != 3) {
                            cs->life_grid(i - LG_HALF, j - LG_HALF)->alive = 0;
                        }
                    } else {
                        if (neighbor_counts[i + 1][j + 1] == 3) {
                            cs->life_grid(i - LG_HALF, j - LG_HALF)->alive = 1;
                        }
                    }
                }
            }
            frame_count = 0;
        }

        frame_count++;
    }
};

class Conways
    : public Stage,
      public ApplicationWithState<ConwaysState> {
public:
    Conways()
        : ApplicationWithState<ConwaysState>(1920 / 2,
            1080 / 2,
            "Conways Game of Life")
    {
        srand(10);
    };

    // Font font;
    // int font_size = 10;
    void setup() override
    {
        // font = LoadFontEx("demo-font.otf", font_size * 20, NULL, 0);
        // state.subtitle_text.font = font;
        // state.subtitle_text.font_size = font_size;

        // ====

        add_scene(Scene::Controllers::timed(
            Scene::Builders::instant([=]() {
                Scene::Batch* sb = new Scene::Batch();
                for (Drawable* d : this->state.cells.objects) {
                    sb->add(((GridCell*)d)->animate_visibility(1.0f));
                    sb->add(d->scale(0.9f));
                }
                return sb;
            }),
            0.0f,
            1.0f));

        add_scene_after_last(Scene::Controllers::timed(
            state.cells.scale(4),
            2.0f,
            3.0f));

        add_scene_after_last(
            Scene::Builders::executor([=]() { state.simulation_speed = -1; }));

        add_scene_after_last(Scene::Controllers::timed(
            Scene::Builders::instant([=]() {
                Scene::Batch* sb = new Scene::Batch();
                for (int i = -LG_HALF; i <= LG_HALF; i++) {
                    for (int j = -LG_HALF; j <= LG_HALF; j++) {
                        if (!(abs(i) <= 1 && abs(j) <= 1))
                            sb->add(
                                state.life_grid(i, j)->animate_visibility(0));
                    }
                }
                return sb;
            }),
            0.0f,
            2.0f));

        add_scene_after_last(Scene::Controllers::timed(
            Scene::Builders::instant([=]() {
                Scene::Batch* sb = new Scene::Batch();

                sb->add(state.neighbors.space_out(2));

                for (int i = 0; i < state.neighbors.size(); i++)
                    sb->add(state.arrows.get<Arrow>(i)
                                ->from(state.life_grid(0, 0))
                                ->to(state.neighbors.get<GridCell>(i))
                                ->appear());

                return sb;
            }),
            0.0f,
            1.0f));

        add_scene_after_last(Scene::Controllers::timed(
            Scene::Builders::instant([=]() {
                Scene::Batch* sb = new Scene::Batch();

                for (int i = 0; i < state.arrows.size(); i++)
                    sb->add(state.arrows.get<Arrow>(i)->disappear());

                return sb;
            }),
            0.0f,
            1.0f));

        add_scene_after_last(Scene::Controllers::timed(
            state.neighbors.space_out(0.5f),
            0.0f,
            1.0f));

        // Background scenes
        add_scene(Scene::Controllers::infinite(
            new ConwaysSimulation(&state)));
    }

    void background_update() override {};

    void update() override { scene_update(); };

    void draw() override
    {
        ClearBackground(BLACK);

        state.cells.draw();
        state.subtitle_text.draw();
        state.arrows.draw();
    }
};