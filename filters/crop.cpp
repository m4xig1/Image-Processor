#include "filters.h"
namespace image_processor {
Crop::Crop(const size_t crop_width, const size_t crop_height) : crop_width_(crop_width), crop_height_(crop_height) {
}

void Crop::UseFilter(Image& img) const {
    const size_t min_height = std::min(img.height_, crop_height_);
    const size_t min_width = std::min(img.width_, crop_width_);
    ImgMatrix new_img;
    new_img.reserve(min_height);
    for (size_t h = 0; h < min_height; ++h) {
        std::vector<Color> buffer;
        buffer.reserve(min_width);
        for (size_t w = 0; w < min_width; ++w) {
            buffer.push_back(img.img_[img.height_ - min_height + h][w]);
        }
        new_img.push_back(std::move(buffer));
    }
    img.height_ = min_height;
    img.width_ = min_width;
    img.img_ = std::move(new_img);
}

std::string Crop::Name() {
    return "-crop";
}

std::string Crop::Help() {
    return " width height\n"
           "Обрезает изображение до заданных ширины и высоты. Используется верхняя левая часть изображения.\n"
           "Если запрошенные ширина или высота превышают размеры исходного изображения, выдается доступная часть "
           "изображения.\n";
}

}  // namespace image_processor
