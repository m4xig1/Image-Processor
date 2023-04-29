#include "filters.h"

namespace image_processor {
void Sharpening::UseFilter(Image& img) const {
    Convolution(img, kernel_, kernel_size_);
}

std::string Sharpening::Name() {
    return "-sharp";
}

std::string Sharpening::Help() {
    return "\nПовышение резкости\n";
}
}  // namespace image_processor