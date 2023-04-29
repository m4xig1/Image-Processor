#pragma once
#include "image_processor.h"

namespace image_processor {
struct Color {  // представление цвета
    double red, green, blue;
    Color();
    Color(const double, const double, const double);
};

static const int BMP_HEADER_SIZE = 14;  // количество байт под хедеры
static const int DIB_HEADER_SIZE = 40;

struct BMPFile {  // тут находится информация, не нужная непосредственно при работе с изображением
    int padding;
    size_t file_size;
    unsigned char bmp_header[BMP_HEADER_SIZE];
    unsigned char dib_header[DIB_HEADER_SIZE];
};

using ImgMatrix = std::vector<std::vector<Color>>;

class Image {  // представление изображения
public:
    explicit Image(const char*);                                 // BMP to Image
    void ChangeColor(const Color&, const size_t, const size_t);  // (height, width)
    //    Color GetColor(const size_t, const size_t) const;            // (height, width)
    void ExportBMP(const char*) const;
    std::pair<size_t, size_t> GetSize() const;  // (height, width)
    size_t height_;
    size_t width_;
    ImgMatrix img_;

private:
    bool is_open_ = false;
};
}  // namespace image_processor