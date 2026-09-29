#pragma once
#include <string>
#include <vector>
#include "Color.h"

struct MipmapLevel{
    int width;
    int height;
    std::vector<Color> pixels;
};

class Texture{
    private:
        std::vector<MipmapLevel> mipmaps;
        void generateMipmaps();
    public:
        Texture(const std::string& filename);
        Color sampleNearest(double u,double v) const;
        Color sampleBilinear(double u,double v,int level=0) const;
        Color sampleTrilinear(double u,double v,double dudx,double dvdx,double dudy,double dvdy) const;
};
