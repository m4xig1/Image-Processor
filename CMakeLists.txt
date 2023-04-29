set(CMAKE_CXX_STANDARD 20)
add_executable(
        image_processor
        image_processor.cpp
        image_processor.h major_classes.cpp major_classes.h input_manipulator.cpp input_manipulator.h
        filters/filters.cpp filters/filters.h filters/crop.cpp filters/grayscale.cpp filters/negative.cpp filters/sharpening.cpp
        filters/edge_detection.cpp filters/gaussian_blur.cpp filters/glass_distortion.cpp)

