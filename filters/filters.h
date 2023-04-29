#pragma once

#include <random>

#include "../image_processor.h"
#include "../major_classes.h"

namespace image_processor {

class Filter {  // базовый класс фильтра
public:
    virtual void UseFilter(Image&) const = 0;  // pure virtual метод, который использует фильтр
    virtual ~Filter() = default;

protected:
    void Convolution(Image&, const std::vector<std::vector<double>>, const size_t) const;  // свертка
};

class Crop : public Filter {  // Crop (-crop width height)
public:
    Crop(const size_t crop_width, const size_t crop_height);
    void UseFilter(Image&) const override;
    static std::string Name();
    static std::string Help();

private:
    size_t crop_width_;
    size_t crop_height_;
};

class Grayscale : public Filter {  // Grayscale (-gs)
public:
    void UseFilter(Image&) const override;
    static std::string Name();
    static std::string Help();

private:
    const double scale_r_ = 0.299;
    const double scale_g_ = 0.587;
    const double scale_b_ = 0.114;
};

class Negative : public Filter {  // Negative (-neg)
public:
    void UseFilter(Image&) const override;
    static std::string Name();
    static std::string Help();
};

class Sharpening : public Filter {  // Sharpening (-sharp)
public:
    void UseFilter(Image&) const override;
    static std::string Name();
    static std::string Help();

private:
    const std::vector<std::vector<double>> kernel_ = {{0, -1, 0}, {-1, 5, -1}, {0, -1, 0}};
    const size_t kernel_size_ = 3;
};

class EdgeDetection : public Filter {  // Edge Detection (-edge threshold)
public:
    explicit EdgeDetection(const double);
    void UseFilter(Image&) const override;
    static std::string Name();
    static std::string Help();

private:
    const double threshold_;
    const std::vector<std::vector<double>> kernel_ = {{0, -1, 0}, {-1, 4, -1}, {0, -1, 0}};
    const size_t kernel_size_ = 3;
};

class GaussianBlur : public Filter {  // Gaussian Blur (-blur sigma)
public:
    explicit GaussianBlur(const double);
    void UseFilter(Image&) const override;
    static std::string Name();
    static std::string Help();

private:
    const double sigma_;
    double GaussianFunc(const int x) const;
    std::vector<double> kernel_;  // будет инициализировано в конструкторе
    size_t kernel_size_;
};

// Кастомный фильтр
class GlassDistortion : public Filter {  // Glass Distortion (-glass radius strength)
public:
    GlassDistortion(const int radius, const double strength);
    void UseFilter(Image&) const override;
    static std::string Name();
    static std::string Help();

private:
    const int radius_;
    const double strength_;
};
}  // namespace image_processor