#include "filters.h"

#include "../image_processor.h"
namespace image_processor {
void Filter::Convolution(Image& img, const std::vector<std::vector<double>> kernel, const size_t kernel_size) const {
    if (kernel_size / 2 == 0) {
        std::cout << "Make kernel_size odd" << std::endl;
        exit(0);
    }
    ImgMatrix new_img;
    int64_t height = static_cast<int64_t>(img.GetSize().first);
    int64_t width = static_cast<int64_t>(img.GetSize().second);
    new_img.reserve(height);
    const int delta = static_cast<int>(kernel_size) / 2;

    for (int64_t h = 0; h < height; ++h) {
        std::vector<Color> row;
        row.reserve(width);
        for (int64_t w = 0; w < width; ++w) {
            double value_r = 0;
            double value_g = 0;
            double value_b = 0;
            // conv для ядра нечетного размера
            for (int delta_h = -delta; delta_h <= delta; ++delta_h) {  // подсчитаем свертку на одном пикселе
                for (int delta_w = -delta; delta_w <= delta; ++delta_w) {
                    // не выходит за пределы изображения
                    int64_t coord_h = std::min(std::max(0L, h + delta_h), height - 1);
                    int64_t coord_w = std::min(std::max(0L, w + delta_w), width - 1);
                    value_r += img.img_[coord_h][coord_w].red * kernel[delta + delta_h][delta + delta_w];
                    value_g += img.img_[coord_h][coord_w].green * kernel[delta + delta_h][delta + delta_w];
                    value_b += img.img_[coord_h][coord_w].blue * kernel[delta + delta_h][delta + delta_w];
                }
            }
            row.push_back(Color(std::min(std::max(0., value_r), 1.), std::min(std::max(0., value_g), 1.),
                                std::min(std::max(0., value_b), 1.)));
        }
        new_img.push_back(std::move(row));
    }
    img.img_ = std::move(new_img);
}
}  // namespace image_processor