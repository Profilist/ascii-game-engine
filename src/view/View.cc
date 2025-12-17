module;
#include <ncurses.h>

export module view;

import <memory>;
import <string>;
import <vector>;
import <algorithm>;
import <clocale>;

import render.drawable;
import render.shape;

export namespace age {

// Custom deleter for ncurses WINDOW* to enable RAII via unique_ptr
struct WinDeleter {
    void operator()(WINDOW* w) const;
};
using WinPtr = std::unique_ptr<WINDOW, WinDeleter>;

// Abstract View interface (MVC)
class View {
public:
    virtual ~View() = default;
    virtual void notify(const std::vector<Drawable>& drawables, const std::vector<std::string>& statusLines) = 0;
};

// Ncurses-based rendering implementation
class CursesView final : public View {
public:
    explicit CursesView(int width = 80, int height = 25);
    ~CursesView() override;

    void notify(const std::vector<Drawable>& drawables, const std::vector<std::string>& statusLines) override;

private:
    // Layout constants
    static constexpr int borderThickness = 1;
    static constexpr int numStatusRows = 3;

    // Lifecycle (called by ctor/dtor only)
    void init();
    void shutdown();

    // Rendering helpers
    void ensureBuffers();
    void clearGameBuffer();
    void drawBorder() const;
    void drawDrawable(const Drawable& drawable);
    void drawStatus(const std::vector<std::string>& lines);
    void flushBuffer();

    // Dimensions
    int outerWidth_;
    int outerHeight_;
    int gameWidth_;
    int gameHeight_;

    // Ncurses windows (RAII via unique_ptr)
    WinPtr gameWindow_;
    WinPtr statusWindow_;

    // Double-buffering for efficient updates
    std::vector<std::string> prevBuffer_;
    std::vector<std::string> scratchBuffer_;
};

}
