#include "filters.h"
namespace image_processor {
void Grayscale::UseFilter(Image& img) const {
    for (size_t h = 0; h < img.height_; ++h) {
        for (size_t w = 0; w < img.width_; ++w) {
            double value =
                img.img_[h][w].red * scale_r_ + img.img_[h][w].green * scale_g_ + img.img_[h][w].blue * scale_b_;
            img.ChangeColor(Color(value, value, value), h, w);
        }
    }
}

std::string Grayscale::Name() {
    return "-gs";
}

std::string Grayscale::Help() {
    return "\nПреобразует изображение в оттенки серого\n";
}
}  // namespace image_processor
