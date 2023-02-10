#pragma once
#include "application.hpp"

template <typename S>
class ApplicationWithState: public Application {
    public:
    S state;
    ApplicationWithState(int window_width, int window_height, const char* name) : Application(window_width, window_height, name) {
        this->state = S(this->app_data);
    }
    virtual ~ApplicationWithState() {};
};
