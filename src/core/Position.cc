export module core.position;

export namespace age {

struct Position {
    int x{0};
    int y{0};

    Position() = default;
    Position(int x, int y);
};

}
