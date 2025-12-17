module;
#include <ncurses.h>

export module controller;

import <variant>;

import core.input_event;
import core.position;

export namespace age {

// Abstract Controller interface (MVC)
class Controller {
public:
    virtual ~Controller() = default;
    virtual InputEvent getInput() = 0;
};

// Ncurses-based input controller
class CursesController final : public Controller {
public:
    CursesController() = default;
    ~CursesController() override = default;

    InputEvent getInput() override;
};

}
