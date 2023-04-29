#include "filters.h"

namespace image_processor {
EdgeDetection::EdgeDetection(const double value) : threshold_(value) {
}

void EdgeDetection::UseFilter(Image& img) const {
    Grayscale gray;
    gray.UseFilter(img);
    Convolution(img, kernel_, kernel_size_);
    for (size_t h = 0; h < img.height_; ++h) {
        for (size_t w = 0; w < img.width_; ++w) {
            if (img.img_[h][w].red > threshold_) {  // у каждой компоненты одинаковые значения
                img.ChangeColor(Color(1, 1, 1), h, w);
            } else {
                img.ChangeColor(Color(0, 0, 0), h, w);
            }
        }
    }
}
std::string EdgeDetection::Name() {
    return "-edge";
}
std::string EdgeDetection::Help() {
    return " threshold\n"
           "Выделение границ. Пиксели со значением, превысившим threshold, окрашиваются в белый, остальные – в "
           "черный.\n";
}
}  // namespace image_processor