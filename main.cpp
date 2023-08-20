#include <cmath>
#include <cstddef>
#include <iostream>
#include <memory>

#include "geometry.h"
#include "model.h"
#include "tgaimage.h"

constexpr const char* DEFAULT_PATH = "../../imgs/output.tga";

const TGAColor white(255, 255, 255, 255);
const TGAColor red(255, 0, 0, 255);

constexpr int width = 800;
constexpr int height = 800;

void line(int x0, int y0, int x1, int y1, TGAImage& image, const TGAColor& color) {
    bool step = false;  // Marks if the absolute value of the current slope is greater than 1
    if (std::abs(y0 - y1) > std::abs(x0 - x1)) {
        std::swap(x0, y0);
        std::swap(x1, y1);
        step = true;
    }

    if (x0 > x1) {  // make it left-to-right
        std::swap(x0, x1);
        std::swap(y0, y1);
    }

    for (int x = x0; x <= x1; x++) {
        float t = 1.0f * (x - x0) / (x1 - x0);
        int y = y0 * (1.0 - t) + y1 * t;
        if (step) {
            image.set(y, x, color);
        } else {
            image.set(x, y, color);
        }
    }
}

int main(int argc, char** argv) {
    std::unique_ptr<Model> model{nullptr};
    // construct model
    if (argc == 2) {
        model = std::make_unique<Model>(argv[1]);
    } else {
        model = std::make_unique<Model>("../../obj/african_head.obj");
    }

    // construct .tga
    TGAImage image{width, height, TGAImage::RGB};
    // for each face
    for (int i = 0; i < model->nfaces(); i++) {
        // store the three vertexs of every face
        auto face = model->face(i);
        for (int j = 0; j < 3; j++) {
            // vertex v0
            auto v0 = model->vert(face[j]);
            // v1
            auto v1 = model->vert(face[(j + 1) % 3]);
            // Draw lines from the vertices v0 and v1
            // Convert model coordinates to screen coordinates.
            // [-1, 1]^2 to [0, width] X [0, height]
            int x0 = (v0.x + 1.0) * width / 2;
            int y0 = (v0.y + 1.0) * height / 2;
            int x1 = (v1.x + 1.0) * width / 2;
            int y1 = (v1.y + 1.0) * height / 2;
            // draw line
            line(x0, y0, x1, y1, image, white);
        }
    }

    // covert original point(left up) to left bottom
    image.flip_vertically();
    image.write_tga_file(DEFAULT_PATH);

    return 0;
}
