#include <cmath>
#include <iostream>

#include "tgaimage.h"

constexpr const char* DEFAULT_PATH = "../../imgs/output.tga";

const TGAColor white(255, 255, 255, 255);
const TGAColor red(255, 0, 0, 255);

void line(int x0, int y0, int x1, int y1, TGAImage& image,
          const TGAColor& color) {
    bool steep = false;
    if (std::abs(y0 - y1) > std::abs(x0 - x1)) {
        std::swap(x0, y0);
        std::swap(x1, y1);
        steep = true;
    }

    if (x0 > x1) {  // make it left-to-right
        std::swap(x0, x1);
        std::swap(y0, y1);
    }

    for (int x = x0; x <= x1; x++) {
        float t = 1.0f * (x - x0) / (x1 - x0);
        int y = y0 * (1.0 - t) + y1 * t;
        if (steep) {
            image.set(y, x, color);
        } else {
            image.set(x, y, color);
        }
    }
}

int main(int argc, char** argv) {
    TGAImage image(100, 100, TGAImage::RGB);
    line(13, 20, 80, 40, image, white);
    line(20, 13, 40, 80, image, red);
    line(80, 40, 13, 20, image, red);
    image.flip_vertically();  // change origin point to left bottom
    image.write_tga_file(DEFAULT_PATH);

    return 0;
}
