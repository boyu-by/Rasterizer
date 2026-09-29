#pragma once
#include <string>
#include <vector>
#include "Color.h"

class Texture{
    private:
        int width;
        int height;
        std::vector<Color> pixels;
    public:
        Texture(const std::string& filename);
        Color sampleNearest(double u,double v) const;
        Color sampleBilinear(double u,double v) const;
};
