#pragma once
#include <sstream>
#include <unordered_map>

#include "filters/filters.h"
#include "image_processor.h"

namespace image_processor {

class InputManipulator {  // Манипулятор для обработки флагов
public:
    InputManipulator(const char**, const int);
    void UseFlags();

private:
    void Help();
    void UseFilter(int& num, Image& img);
    const char** argv_;
    const int argc_;
    bool IsNumber(const std::string val);
    const std::unordered_map<std::string, bool> zero_param_flag_ = {
        {Grayscale::Name(), 1}, {Negative::Name(), 1}, {Sharpening::Name(), 1}};
    const std::unordered_map<std::string, bool> one_param_flag_ = {{EdgeDetection::Name(), 1},
                                                                   {GaussianBlur::Name(), 1}};
    const std::unordered_map<std::string, bool> two_param_flag_ = {{Crop::Name(), 1}, {GlassDistortion::Name(), 1}};
};
}  // namespace image_processor

/* Создать новый фильтр.
 * Добавить флаг в соответсвующий map.
 * Добавить реализацию в UseFilter.
 * Добавить реализацию в Help.
 */