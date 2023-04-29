#include "filters.h"
namespace image_processor {
GlassDistortion::GlassDistortion(const int radius, const double strength)
    : radius_(radius), strength_(std::max(std::min(strength, 0.), 1.)) {
}

void GlassDistortion::UseFilter(Image& img) const {
    ImgMatrix new_img;
    new_img.reserve(img.height_);

    std::random_device rd;                                               // seed
    std::mt19937 gen(rd());                                              // Standard mersenne_twister_engine
    std::uniform_int_distribution<int> distribution(-radius_, radius_);  // генератор нового положения пикселя
    for (size_t h = 0; h < img.height_; ++h) {
        std::vector<Color> row;
        row.reserve(img.width_);
        for (size_t w = 0; w < img.width_; ++w) {
            int new_h = static_cast<int>(h) + distribution(gen);
            int new_w = static_cast<int>(w) + distribution(gen);
            double value_r = img.img_[h][w].red;
            double value_g = img.img_[h][w].green;
            double value_b = img.img_[h][w].blue;
            if (new_h >= 0 && new_h < img.height_ && new_w >= 0 && new_w < img.width_) {
                value_r = img.img_[new_h][new_w].red * strength_ + value_r * (1 - strength_);
                value_g = img.img_[new_h][new_w].green * strength_ + value_g * (1 - strength_);
                value_b = img.img_[new_h][new_w].blue * strength_ + value_b * (1 - strength_);
            }
            row.push_back(Color(std::min(std::max(0., value_r), 1.), std::min(std::max(0., value_g), 1.),
                                std::min(std::max(0., value_b), 1.)));
        }
        new_img.push_back(std::move(row));
    }
    img.img_ = std::move(new_img);
}

std::string GlassDistortion::Name() {
    return "-glass";
}

std::string GlassDistortion::Help() {
    return " radius strength\n"
           "Фильтр Glass Distortion - это тип эффекта искажения изображения, который имитирует появление стекла.\n "
           "radius "
           "определяет максимальное расстояние, на которое пиксель может быть перемещен из его исходного положения,\n"
           "strength определяет, насколько сильно применяется смещение к пикселю.";
}
}  // namespace image_processor