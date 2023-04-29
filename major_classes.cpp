#include "major_classes.h"
namespace image_processor {

Color::Color() : red(0), green(0), blue(0) {
}
Color::Color(const double r, const double g, const double b) : red(r), green(g), blue(b) {
}

void Image::ChangeColor(const Color& clr, const size_t h, const size_t w) {
    img_[h][w].red = clr.red;
    img_[h][w].green = clr.green;
    img_[h][w].blue = clr.blue;
}

Image::Image(const char* path) {
    std::ifstream fin;
    fin.exceptions(std::ifstream::badbit | std::ifstream::failbit);
    BMPFile file_features;

    try {
        fin.open(path, std::ios::in | std::ios::binary);  // поток ввода
        std::cout << "Открытие успешное" << std::endl;
    } catch (const std::ifstream::failure& ex) {
        std::cout << "Ошибка при открытии: " << path << std::endl;
        std::cout << ex.code() << std::endl;
        exit(0);
    }

    fin.read(reinterpret_cast<char*>(file_features.bmp_header), BMP_HEADER_SIZE);  // приведение типов указателей
    if (file_features.bmp_header[0] != 'B' || file_features.bmp_header[1] != 'M') {  // первые 2 байта всегда "BM"
        std::cout << "На вход подан файл не с расширением BMP" << std::endl;
        fin.close();  // не забываем закрыть файл!
        return;
    }

    fin.read(reinterpret_cast<char*>(file_features.dib_header), DIB_HEADER_SIZE);
    const int bps = file_features.dib_header[14];  // возможно, надо сдвинуть еще 3 следующих бита
    const int bps_const = 24;
    if (bps != bps_const) {
        std::cout << "На вход подан файл не являющийся BMP-24" << std::endl;
        return;
    }
    is_open_ = true;  // flag
    const size_t byte = 8;
    // 4 байта под размер, высоту и ширину файла
    // join bits! << byte*n
    size_t start = 2;
    file_features.file_size = file_features.bmp_header[start] + (file_features.bmp_header[start + 1] << byte) +
                              (file_features.bmp_header[start + 2] << (byte * 2)) +
                              (file_features.bmp_header[start + 3] << (byte * 3));
    start = 4;  // вау, обожаю clang
    width_ = file_features.dib_header[start] + (file_features.dib_header[start + 1] << byte) +
             (file_features.dib_header[start + 2] << (byte * 2)) + (file_features.dib_header[start + 3] << (byte * 3));
    start += 4;
    height_ = file_features.dib_header[start] + (file_features.dib_header[start + 1] << byte) +
              (file_features.dib_header[start + 2] << (byte * 2)) + (file_features.dib_header[start + 3] << (byte * 3));

    file_features.padding = (4 - (static_cast<int>(width_) * 3) % 4) % 4;
    img_.reserve(height_);

    const double max_range = 255.0f;
    for (size_t h = 0; h < height_; ++h) {
        std::vector<Color> row(width_);
        for (size_t w = 0; w < width_; ++w) {
            unsigned char buffer[3];
            fin.read(reinterpret_cast<char*>(buffer), 3);             // unsig. to signed
            row[w].red = static_cast<double>(buffer[2]) / max_range;  // биты перевернуты
            row[w].green = static_cast<double>(buffer[1]) / max_range;
            row[w].blue = static_cast<double>(buffer[0]) / max_range;
        }
        fin.ignore(file_features.padding);
        img_.push_back(std::move(row));
    }
    fin.close();
}

void Image::ExportBMP(const char* path) const {
    if (!is_open_) {
        std::cout << "Файл не был открыт, поэтому не может быть преобразован в BMP\n";
        return;
    }
    std::ofstream fout;
    fout.exceptions(std::ifstream::badbit | std::ifstream::failbit);
    try {
        fout.open(path, std::ios::out | std::ios::binary);  // поток вывода
    } catch (const std::ifstream::failure& ex) {
        std::cout << "Ошибка при записи: " << path << std::endl;
        std::cout << ex.code() << std::endl;
        return;
    }

    // все эти данные могут измениться при применении фильтров, поэтому приходится заново все пересчитывать
    const int64_t padding = (4 - (static_cast<int>(width_) * 3) % 4) % 4;
    const size_t file_size = height_ * width_ * 3 + padding * width_ + BMP_HEADER_SIZE + DIB_HEADER_SIZE;

    BMPFile file;
    const size_t byte = 8;
    // дальше будем побайтово записывать информацию в файл, как она была там до этого:
    // BMP HEADER
    size_t i = 0;  // это не код нечитаемый, а clang тупой
    file.bmp_header[i++] = 'B';
    file.bmp_header[i++] = 'M';
    file.bmp_header[i++] = file_size;
    file.bmp_header[i++] = file_size >> byte;
    file.bmp_header[i++] = file_size >> (byte * 2);
    file.bmp_header[i++] = file_size >> (byte * 3);
    // unused
    file.bmp_header[i++] = 0;
    file.bmp_header[i++] = 0;
    file.bmp_header[i++] = 0;
    file.bmp_header[i++] = 0;
    // Offset where the pixel array (bitmap data) can be found
    file.bmp_header[i++] = BMP_HEADER_SIZE + DIB_HEADER_SIZE;  // < 255
    file.bmp_header[i++] = 0;
    file.bmp_header[i++] = 0;  // можем не сдвигать
    file.bmp_header[i++] = 0;
    // DIB HEADER
    i = 0;
    file.dib_header[i++] = DIB_HEADER_SIZE;  // < 255
    file.dib_header[i++] = 0;
    file.dib_header[i++] = 0;  // можем не сдвигать
    file.dib_header[i++] = 0;
    file.dib_header[i++] = width_;
    file.dib_header[i++] = width_ >> byte;
    file.dib_header[i++] = width_ >> (byte * 2);
    file.dib_header[i++] = width_ >> (byte * 3);
    file.dib_header[i++] = height_;
    file.dib_header[i++] = height_ >> byte;
    file.dib_header[i++] = height_ >> (byte * 2);
    file.dib_header[i++] = height_ >> (byte * 3);
    // Number of color planes being used
    file.dib_header[i++] = 1;
    file.dib_header[i++] = 0;
    // 	Number of bits per pixel
    const int bpp = 24;
    file.dib_header[i++] = bpp;
    file.dib_header[i++] = 0;
    // остальную информацию положим нулями
    const int mn = 16;
    const int mx = 40;
    for (size_t j = mn; j < mx; ++j) {  // [16, 40)
        file.dib_header[j] = 0;
    }
    fout.write(reinterpret_cast<char*>(file.bmp_header), BMP_HEADER_SIZE);
    fout.write(reinterpret_cast<char*>(file.dib_header), DIB_HEADER_SIZE);

    unsigned char padding_data[3] = {0, 0, 0};
    const double max_value = 255.;
    for (size_t h = 0; h < height_; ++h) {
        for (size_t w = 0; w < width_; ++w) {
            unsigned char r = static_cast<unsigned char>(img_[h][w].red * max_value);
            unsigned char g = static_cast<unsigned char>(img_[h][w].green * max_value);
            unsigned char b = static_cast<unsigned char>(img_[h][w].blue * max_value);
            unsigned char info[] = {b, g, r};
            fout.write(reinterpret_cast<char*>(info), 3);
        }
        fout.write(reinterpret_cast<char*>(padding_data), padding);
    }
    fout.close();
    std::cout << "Файл записан по адресу: " << path << std::endl;
}

std::pair<size_t, size_t> Image::GetSize() const {
    return std::make_pair(height_, width_);
}
}  // namespace image_processor