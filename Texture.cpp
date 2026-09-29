#include "Texture.h"
#include <algorithm>
#include <cmath>
#include <fstream>
#include <stdexcept>

std::string readToken(std::istream& in){
    std::string token;
    while(in>>token){
        if(!token.empty()&&token[0]=='#'){
            std::string line;
            std::getline(in,line);
            continue;
        }
        return token;
    }
    throw std::runtime_error("unexpected end of texture file");
}

Texture::Texture(const std::string& filename):width(0),height(0){
    std::ifstream in(filename);
    if(!in){
        throw std::runtime_error("cannot open texture file");
    }
    if(readToken(in)!="P3"){
        throw std::runtime_error("texture must be a P3 PPM file");
    }
    width=std::stoi(readToken(in));
    height=std::stoi(readToken(in));
    int maxValue=std::stoi(readToken(in));
    if(width<=0||height<=0||maxValue<=0){
        throw std::runtime_error("invalid texture header");
    }
    pixels.reserve(width*height);
    for(int i=0;i<width*height;i++){
        int r=std::stoi(readToken(in));
        int g=std::stoi(readToken(in));
        int b=std::stoi(readToken(in));
        r=std::clamp(r,0,maxValue)*255/maxValue;
        g=std::clamp(g,0,maxValue)*255/maxValue;
        b=std::clamp(b,0,maxValue)*255/maxValue;
        pixels.emplace_back(r,g,b);
    }
}

Color Texture::sampleNearest(double u,double v) const{
    u=std::clamp(u,0.0,1.0);
    v=std::clamp(v,0.0,1.0);
    int x=(int)std::round(u*(width-1));
    int y=(int)std::round((1-v)*(height-1));
    return pixels[y*width+x];
}

Color Texture::sampleBilinear(double u,double v) const{
    u=std::clamp(u,0.0,1.0);
    v=std::clamp(v,0.0,1.0);

    double x=u*(width-1);
    double y=(1-v)*(height-1);
    int x0=(int)std::floor(x);
    int y0=(int)std::floor(y);
    int x1=std::min(x0+1,width-1);
    int y1=std::min(y0+1,height-1);
    double tx=x-x0;
    double ty=y-y0;

    Color c00=pixels[y0*width+x0];
    Color c10=pixels[y0*width+x1];
    Color c01=pixels[y1*width+x0];
    Color c11=pixels[y1*width+x1];

    double topRed=c00.r*(1-tx)+c10.r*tx;
    double topGreen=c00.g*(1-tx)+c10.g*tx;
    double topBlue=c00.b*(1-tx)+c10.b*tx;
    double bottomRed=c01.r*(1-tx)+c11.r*tx;
    double bottomGreen=c01.g*(1-tx)+c11.g*tx;
    double bottomBlue=c01.b*(1-tx)+c11.b*tx;

    return Color(
        (int)std::round(topRed*(1-ty)+bottomRed*ty),
        (int)std::round(topGreen*(1-ty)+bottomGreen*ty),
        (int)std::round(topBlue*(1-ty)+bottomBlue*ty)
    );
}
