#include "filters.h"

namespace image_processor {
GaussianBlur::GaussianBlur(const double value) : sigma_(value) {
    // точки, находящиеся на расстоянии > 3sigma имеют пренебрежимо малое значение
    kernel_size_ = 2 * static_cast<size_t>(std::ceil((3 * std::abs(sigma_)))) + 1;
    const int delta = static_cast<int>(kernel_size_) / 2;
    kernel_.reserve(kernel_size_);
    for (int x = -delta; x <= delta; ++x) {
        kernel_.push_back(GaussianFunc(x));  // одномерное ядро
    }
}

double GaussianBlur::GaussianFunc(const int x) const {
    return std::exp(-static_cast<double>(x * x) / (2 * sigma_ * sigma_)) / std::sqrt(2 * sigma_ * sigma_ * M_PI);
}

void GaussianBlur::UseFilter(Image& img) const {
    // придется написать свертку для 1d ядра заново
    if (kernel_size_ / 2 == 0) {
        std::cout << "Make kernel_size odd" << std::endl;
        exit(0);
    }

    ImgMatrix temp;
    ImgMatrix new_img;
    int64_t height = static_cast<int64_t>(img.GetSize().first);
    int64_t width = static_cast<int64_t>(img.GetSize().second);
    temp.reserve(height);
    const int delta = static_cast<int>(kernel_size_) / 2;

    // 1d свертка вдоль h
    for (int64_t h = 0; h < height; ++h) {
        std::vector<Color> row;
        row.reserve(width);
        for (int64_t w = 0; w < width; ++w) {
            double value_r = 0;
            double value_g = 0;
            double value_b = 0;
            for (int delta_h = -delta; delta_h <= delta; ++delta_h) {
                int64_t coord_h = std::min(std::max(0L, h + delta_h), height - 1);
                value_r += img.img_[coord_h][w].red * kernel_[delta + delta_h];
                value_g += img.img_[coord_h][w].green * kernel_[delta + delta_h];
                value_b += img.img_[coord_h][w].blue * kernel_[delta + delta_h];
            }
            row.push_back(Color(std::min(std::max(0., value_r), 1.), std::min(std::max(0., value_g), 1.),
                                std::min(std::max(0., value_b), 1.)));
        }
        temp.push_back(std::move(row));
    }

    new_img.reserve(height);
    // 1d свертка вдоль w
    for (int64_t h = 0; h < height; ++h) {
        std::vector<Color> row;
        row.reserve(width);
        for (int64_t w = 0; w < width; ++w) {
            double value_r = 0;
            double value_g = 0;
            double value_b = 0;
            for (int delta_w = -delta; delta_w <= delta; ++delta_w) {
                int64_t coord_w = std::min(std::max(0L, w + delta_w), width - 1);
                value_r += temp[h][coord_w].red * kernel_[delta + delta_w];  // здесь уже используем значения из temp
                value_g += temp[h][coord_w].green * kernel_[delta + delta_w];
                value_b += temp[h][coord_w].blue * kernel_[delta + delta_w];
            }
            row.push_back(Color(std::min(std::max(0., value_r), 1.), std::min(std::max(0., value_g), 1.),
                                std::min(std::max(0., value_b), 1.)));
        }
        new_img.push_back(std::move(row));
    }
    img.img_ = std::move(new_img);
}

std::string GaussianBlur::Name() {
    return "-blur";
}

std::string GaussianBlur::Help() {
    return " sigma\n"
           "Гауссово размытие, параметр – сигма.\n";
}
}  // namespace image_processor