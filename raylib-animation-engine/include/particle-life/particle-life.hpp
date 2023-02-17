#include <cmath>
#include <iostream>
#include <string>

#include "application/application_with_state.hpp"
#include "drawables.hpp"
#include "easing.hpp"
#include "stage/scene.hpp"
#include "stage/stage.hpp"

#include "particle-life/state.hpp"
#include "particle-life/simulation.hpp"

using namespace Drawables;


class PLStage
    : public Stage,
      public ApplicationWithState<PLState> {
public:
    PLStage()
        : ApplicationWithState<PLState>(1920 / 2,
            1080 / 2,
            "Conways Game of Life")
    {
        // srand(10);
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
            state.particles.scale(2, Vector2 {0, 0}),
            1.0f,
            2.0f
        ));


        // Background scenes
        add_scene(Scene::Controllers::infinite(
            new Simulation(&state)));
    }

    void background_update() override {};

    void update() override { scene_update(); };

    void draw() override
    {
        ClearBackground(BLACK);

        state.particles.draw();
        // state.cells.draw();
        // state.subtitle_text.draw();
        // state.arrows.draw();
    }
};