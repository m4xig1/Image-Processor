#include "input_manipulator.h"

int main(int argc, const char** argv) {
    image_processor::InputManipulator manipulate(argv, argc);
    manipulate.UseFlags();
}
