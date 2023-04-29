#include "input_manipulator.h"

namespace image_processor {

InputManipulator::InputManipulator(const char** argv, const int argc) : argv_(argv), argc_(argc) {
}

void InputManipulator::UseFlags() {
    if (argc_ == 1) {
        Help();
        exit(0);
    }
    if (argc_ < 3) {
        std::cout << "Пожалуйста, введите обязательные параметры\n";
        exit(0);
    }

    const char* input_path = argv_[1];
    const char* output_path = argv_[2];
    Image img = Image(input_path);

    int i = 3;
    while (i < argc_) {
        UseFilter(i, img);
    }

    try {
        img.ExportBMP(output_path);
    } catch (...) {
        std::cout << "Ошибка при закрытии файла" << std::endl;
        exit(0);
    }
}

bool InputManipulator::IsNumber(const std::string val) {
    if (std::count(val.begin(), val.end(), '.') > 1) {
        return false;
    }
    return !val.empty() &&
           std::all_of(val.begin(), val.end(), [](const char c) { return std::isdigit(c) || c == '.'; });
}

void InputManipulator::UseFilter(int& num, Image& img) {
    std::string flag = argv_[num];
    if (flag.empty() || flag[0] != '-') {
        std::cout << "На вход вместо флага получен: " << flag << std::endl;
        exit(0);
    }
    Filter* obj = nullptr;  // указатель на объект базового класса
    if (zero_param_flag_.find(flag) != zero_param_flag_.end()) {  // 0 параметров
        if (flag == Grayscale::Name()) {
            obj = new Grayscale();
        } else if (flag == Negative::Name()) {
            obj = new Negative();
        } else {
            obj = new Sharpening();
        }
        ++num;
    } else if (one_param_flag_.find(flag) != one_param_flag_.end()) {  // 1 параметр
        if (argc_ <= num + 1) {
            std::cout << "Недостаточно параметров для " << flag << std::endl;
            exit(0);
        }
        if (!IsNumber(argv_[num + 1])) {
            std::cout << "На вход " << flag << " подано не число" << std::endl;
            exit(0);
        }
        if (flag == EdgeDetection::Name()) {
            obj = new EdgeDetection(std::stod(argv_[num + 1]));
        } else {
            obj = new GaussianBlur(std::stod(argv_[num + 1]));
        }
        num += 2;
    } else if (two_param_flag_.find(flag) != two_param_flag_.end()) {  // 2 параметра
        if (argc_ <= num + 2) {
            std::cout << "Недостаточно параметров для " << flag << std::endl;
            exit(0);
        }
        if (!IsNumber(argv_[num + 1]) || !IsNumber(argv_[num + 2])) {
            std::cout << "На вход " << flag << " подано не число" << std::endl;
            exit(0);
        }
        if (flag == Crop::Name()) {
            obj = new Crop(std::stoul(argv_[num + 1]), std::stoul(argv_[num + 2]));
        } else {
            obj = new GlassDistortion(std::stoi(argv_[num + 1]), std::stod(argv_[num + 2]));
        }
        num += 3;
    } else {
        std::cout << "На вход подан несуществующий флаг: " << flag << std::endl;
        exit(0);
    }
    try {
        obj->UseFilter(img);
    } catch (...) {
        std::cout << "Ошибка при вызове фильтра " << flag << std::endl;
    }
    delete obj;
}

void InputManipulator::Help() {
    std::stringstream help;
    help << HELP_TEXT;
    help << Crop::Name() << Crop::Help() << Grayscale::Name() << Grayscale::Help() << Negative::Name()
         << Negative::Help() << Sharpening::Name() << Sharpening::Help() << EdgeDetection::Name()
         << EdgeDetection::Help() << GaussianBlur::Name() << GaussianBlur::Help() << GlassDistortion::Name()
         << GlassDistortion::Help();
    std::cout << help.str();
}
}  // namespace image_processor