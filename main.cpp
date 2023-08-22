#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <iostream>
#include <limits>
#include <memory>
#include <vector>

#include "geometry.h"
#include "model.h"
#include "tgaimage.h"

constexpr const char* DEFAULT_PATH = "../../imgs/output.tga";

const TGAColor white(255, 255, 255, 255);
const TGAColor red(255, 0, 0, 255);
const TGAColor green(0, 255, 0, 255);

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

bool inside_triangle(const Vec3f& p, const std::array<Vec3f, 3>& t) {
    bool clockwise = cross(t[1] - t[0], t[2] - t[0]).z > 0;
    Vec3f e0 = t[1] - t[0], e1 = t[2] - t[1], e2 = t[0] - t[2];
    Vec3f p0 = p - t[0], p1 = p - t[1], p2 = p - t[2];
    if (clockwise) {
        return cross(e0, p0).z > 0 && cross(e1, p1).z > 0 && cross(e2, p2).z > 0;
    }
    return cross(e0, p0).z < 0 && cross(e1, p1).z < 0 && cross(e2, p2).z < 0;
}

Vec3f world_to_screen(const Vec3f& v) {
    return Vec3f{(v.x + 1) * width / 2 + 0.5f, (v.y + 1) * height / 2 + 0.5f, v.z};
}

std::tuple<float, float, float> barycentric2D(float x, float y,
                                              const std::array<Vec3f, 3>& v) {
    float c1 =
        (x * (v[1].y - v[2].y) + (v[2].x - v[1].x) * y + v[1].x * v[2].y - v[2].x *
        v[1].y) / (v[0].x * (v[1].y - v[2].y) + (v[2].x - v[1].x) * v[0].y + v[1].x *
        v[2].y -
         v[2].x * v[1].y);
    float c2 =
        (x * (v[2].y - v[0].y) + (v[0].x - v[2].x) * y + v[2].x * v[0].y - v[0].x *
        v[2].y) / (v[1].x * (v[2].y - v[0].y) + (v[0].x - v[2].x) * v[1].y + v[2].x *
        v[0].y -
         v[0].x * v[2].y);
    float c3 =
        (x * (v[0].y - v[1].y) + (v[1].x - v[0].x) * y + v[0].x * v[1].y - v[1].x *
        v[0].y) / (v[2].x * (v[0].y - v[1].y) + (v[1].x - v[0].x) * v[2].y + v[0].x *
        v[1].y -
         v[1].x * v[0].y);
    return {c1, c2, c3};
}

void triangle(std::array<Vec3f, 3>& pts, std::vector<float>& zbuffer, TGAImage& image,
              const TGAColor& color) {
    // construct boundingbox
    int min_x = std::floor(std::max(0.0f, std::min({pts[0].x, pts[1].x, pts[2].x})));
    int min_y = std::floor(std::max(0.0f, std::min({pts[0].y, pts[1].y, pts[2].y})));
    int max_x = std::ceil(std::min(static_cast<float>(image.get_width() - 1),
                           std::max({pts[0].x, pts[1].x, pts[2].x})));
    int max_y = std::ceil(std::min(static_cast<float>(image.get_height() - 1),
                           std::max({pts[0].y, pts[1].y, pts[2].y})));

    for (int x = min_x; x <= max_x; x++) {
        for (int y = min_y; y <= max_y; y++) {
            if (!inside_triangle({x + 0.5f, y + 0.5f, 0}, pts)) {
                continue;
            }

            auto [alpha, beta, gamma] = barycentric2D(x + 0.5f, y + 0.5f, pts);
            float z = pts[0].z * alpha + pts[1].z * beta + pts[2].z * gamma;

            if (zbuffer[x + y * width] < z) {
                zbuffer[x + y * width] = z;
                image.set(x, y, color);
            }
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

    Vec3f light_dir{0, 0, -1};
    std::vector<float> zbuffer(width * height, std::numeric_limits<float>::lowest());

    TGAImage image{width, height, TGAImage::RGB};
    for (int i = 0; i < model->nfaces(); i++) {
        auto face = model->face(i);
        std::array<Vec3f, 3> screen_coords{};
        std::array<Vec3f, 3> world_coords{};
        for (int j = 0; j < 3; j++) {
            Vec3f v = model->vert(face[j]);
            screen_coords[j] = world_to_screen(model->vert(face[j]));
            world_coords[j] = v;
        }
        Vec3f n =
            cross((world_coords[2] - world_coords[0]), (world_coords[1] - world_coords[0]));
        n.normalize();

        float intensity = n * light_dir;
        if (intensity > 0) {
            triangle(screen_coords, zbuffer, image,
                     TGAColor(intensity * 255, intensity * 255, intensity * 255, 255));
        }
    }

    // covert original point(left up) to left bottom
    image.flip_vertically();
    image.write_tga_file(DEFAULT_PATH);

    return 0;
}
