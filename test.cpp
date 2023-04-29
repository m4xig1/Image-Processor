#include <catch.hpp>  // мем, конечно, что машина локально либу не видет

#include "filters/filters.h"
#include "image_processor.h"
#include "input_manipulator.h"

const double DELTA = 0.01;

bool ImageEqual(const image_processor::Image& rhs, const image_processor::Image& lhs) {
    if (rhs.GetSize() != lhs.GetSize()) {
        return false;
    }
    for (size_t h = 0; h < rhs.height_; ++h) {
        for (size_t w = 0; w < rhs.width_; ++w) {
            if (!(std::abs(rhs.img_[h][w].red - lhs.img_[h][w].red) <= DELTA &&
                  std::abs(rhs.img_[h][w].green - lhs.img_[h][w].green) <= DELTA &&
                  std::abs(rhs.img_[h][w].blue - lhs.img_[h][w].blue) <= DELTA)) {
                return false;
            }
        }
    }
    return true;
}

TEST_CASE("ImageProcessor") {

    { // тест на InputManipulator
        InputManipulator manipulator("input/path lenna.bmp lenna1.bmp -blur 7.5 -blur 3 q", 7);
        image_processor::Image img("lenna1.bmp");
        image_processor::Image test_blur_blur("../test_script/data/lenna_blur_blur.bmp");
        REQUIRE(ImageEqual(img, test_blur_blur));
    }

    {
        const size_t test_w = 999;
        const size_t test_h = 1999;
        image_processor::Image img("lenna.bmp");
        image_processor::Image test_crop("../test_script/data/lenna_crop.bmp");
        image_processor::Crop crop(test_w, test_h);
        crop.UseFilter(img);
        REQUIRE(ImageEqual(img, test_crop));
    }

    {
        image_processor::Image img("lenna.bmp");
        image_processor::Image test_gs("../test_script/data/lenna_gs.bmp");
        image_processor::Grayscale gs;
        gs.UseFilter(img);
        REQUIRE(ImageEqual(img, test_gs));
    }

    {
        image_processor::Image img("lenna.bmp");
        image_processor::Image test_neg("../test_script/data/lenna_gs.bmp");
        image_processor::Negative neg;
        neg.UseFilter(img);
        REQUIRE(ImageEqual(img, test_neg));
    }

    {
        image_processor::Image img("lenna.bmp");
        image_processor::Image test_sharp("../test_script/data/lenna_sharp.bmp");
        image_processor::Sharpening sharp;
        sharp.UseFilter(img);
        REQUIRE(ImageEqual(img, test_sharp));
    }

    {
        image_processor::Image img("lenna.bmp");
        image_processor::Image test_edge("../test_script/data/lenna_sharp.bmp");
        image_processor::EdgeDetection edge(1.0);
        edge.UseFilter(img);
        REQUIRE(ImageEqual(img, test_edge));
    }

    {
        const double sigma_test = 7.5;
        image_processor::Image img("lenna.bmp");
        image_processor::Image test_blur("../test_script/data/lenna_blur.bmp");
        image_processor::GaussianBlur blur(sigma_test);
        blur.UseFilter(img);
        REQUIRE(ImageEqual(img, test_blur));
    }
}
