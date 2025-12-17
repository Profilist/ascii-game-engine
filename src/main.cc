import <string>;

namespace age {

void runFlappyBird();
void runSpaceInvaders();

}

int main(int argc, char* argv[]) {
    std::string mode = "-g1"; // default to Flappy Bird
    if (argc >= 2) mode = argv[1];

    if (mode == "-g1") {
        age::runFlappyBird();
    } else if (mode == "-g2") {
        age::runSpaceInvaders();
    }

    return 0;
}
