#include "filters.h"

namespace image_processor {
void Negative::UseFilter(Image& img) const {
    for (size_t h = 0; h < img.height_; ++h) {
        for (size_t w = 0; w < img.width_; ++w) {
            img.img_[h][w].red = 1 - img.img_[h][w].red;
            img.img_[h][w].green = 1 - img.img_[h][w].green;
            img.img_[h][w].blue = 1 - img.img_[h][w].blue;
        }
    }
}

std::string Negative::Name() {
    return "-neg";
}

std::string Negative::Help() {
    return "\nПреобразует изображение в негатив\n";
}
}  // namespace image_processor
