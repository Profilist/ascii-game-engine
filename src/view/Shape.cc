export module render.shape;

import <iostream>;
import <string>;
import <vector>;

export namespace age {

class Shape {
public:
    Shape() = default;
    Shape(std::string spriteId, std::vector<std::string> pixels);

    const std::string& spriteId() const noexcept;
    int width() const noexcept;
    int height() const noexcept;

    const std::vector<std::string>& rows() const noexcept;

    char at(int row, int col) const;

private:
    std::string spriteId_;
    std::vector<std::string> pixels_;
    int width_;
    int height_;
};

}
