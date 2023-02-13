#include <iostream>
#include <raylib.h>

#pragma once

struct ApplicationData {
    int width, height;
};

class Application {
public:
    Application(int window_width, int window_height, const char* name) {
        app_data.width = window_width;
        app_data.height = window_height;

        InitWindow(app_data.width, app_data.height, name);

        SetTargetFPS(60);
        // SetTargetFPS(120);
        // SetTargetFPS(300);
    };

    ApplicationData app_data;
    int current_frame;

    void start() {
        current_frame = 0;
        setup();

        while (!WindowShouldClose()) {
            update();

            BeginDrawing();
                draw();
            EndDrawing();

            current_frame++;
        }
    }

    void go_fullscreen() {
        int display = GetCurrentMonitor();

        app_data.width = GetMonitorWidth(display);
        app_data.height = GetMonitorHeight(display);
        
        SetWindowSize(app_data.width, app_data.height);

        ToggleFullscreen();
    }

    virtual void setup() = 0;
    virtual void update() = 0;
    virtual void draw() = 0;
};