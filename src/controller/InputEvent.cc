export module core.input_event;

import <variant>;

import core.position;

export namespace age {
    
struct NoInput {};

struct KeyboardInput {
    int key;

    KeyboardInput() = default;
    explicit KeyboardInput(int k);
};

using InputEvent = std::variant<NoInput, KeyboardInput>;

// Helper to check if input is empty
inline bool isNoInput(const InputEvent& ev) {
    return std::holds_alternative<NoInput>(ev);
}

inline bool isKeyboardInput(const InputEvent& ev) {
    return std::holds_alternative<KeyboardInput>(ev);
}

inline const KeyboardInput* getKeyboardInput(const InputEvent& ev) {
    return std::get_if<KeyboardInput>(&ev);
}

}
