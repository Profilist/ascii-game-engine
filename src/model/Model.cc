export module model;

import <string>;
import <vector>;

import controller;
import render.drawable;
import view;

export namespace age {

// Abstract Model base class (MVC)
class Model {
public:
    virtual ~Model() = default;

    // View management (observer pattern)
    void addView(View* v);

    void removeView(View* v);

    // Controller management
    void setController(Controller* c);

    Controller* controller() const noexcept;

    // Notify all registered Views (called by Model when state changes)
    void notifyViews();

    // Pure virtual methods for concrete Models to implement
    virtual std::vector<Drawable> collectDrawables() = 0;
    virtual std::vector<std::string> collectStatus() = 0;
    virtual void run() = 0;

protected:
    std::vector<View*> views_;
    Controller* controller_{nullptr};
};

}
