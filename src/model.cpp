#include "model.h"

#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "geometry.h"

Model::Model(const char* filename) : verts_(), faces_() {
    std::ifstream in;
    in.open(filename, std::ifstream::in);
    if (in.fail()) {
        return;
    }

    std::string line;
    while (!in.eof()) {
        std::getline(in, line);
        std::istringstream iss(line.c_str());
        char trash;
        if (!line.compare(0, 2, "v ")) {
            iss >> trash;
            Vec3f v{};
            for (int i = 0; i < 3; i++) {
                iss >> v.raw[i];
            }
            verts_.push_back(v);
        } else if (!line.compare(0, 2, "f ")) {
            std::vector<int> f;
            int itrash, idx;
            iss >> trash;
            while (iss >> idx >> trash >> itrash >> trash >> itrash) {
                idx--;  // in wavefront obj all indices start at 1, not zero
                f.push_back(idx);
            }
            // move temp variable
            faces_.push_back(std::move(f));
        }
    }

    std::cerr << "# v#" << verts_.size() << " f#" << faces_.size() << std::endl;
}

Model::~Model() {}

int Model::nverts() { return static_cast<int>(verts_.size()); }

int Model::nfaces() { return static_cast<int>(faces_.size()); }

Vec3f Model::vert(int i) { return verts_[i]; }

std::vector<int> Model::face(int index) { return faces_[index]; }
