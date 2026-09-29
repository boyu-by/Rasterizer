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

Texture::Texture(const std::string& filename){
    std::ifstream in(filename);
    if(!in){
        throw std::runtime_error("cannot open texture file");
    }
    if(readToken(in)!="P3"){
        throw std::runtime_error("texture must be a P3 PPM file");
    }
    MipmapLevel original;
    original.width=std::stoi(readToken(in));
    original.height=std::stoi(readToken(in));
    int maxValue=std::stoi(readToken(in));
    if(original.width<=0||original.height<=0||maxValue<=0){
        throw std::runtime_error("invalid texture header");
    }
    original.pixels.reserve(original.width*original.height);
    for(int i=0;i<original.width*original.height;i++){
        int r=std::stoi(readToken(in));
        int g=std::stoi(readToken(in));
        int b=std::stoi(readToken(in));
        r=std::clamp(r,0,maxValue)*255/maxValue;
        g=std::clamp(g,0,maxValue)*255/maxValue;
        b=std::clamp(b,0,maxValue)*255/maxValue;
        original.pixels.emplace_back(r,g,b);
    }
    mipmaps.push_back(original);
    generateMipmaps();
}

void Texture::generateMipmaps(){
    while(mipmaps.back().width>1||mipmaps.back().height>1){
        const MipmapLevel& previous=mipmaps.back();
        MipmapLevel current;
        current.width=std::max(1,(previous.width+1)/2);
        current.height=std::max(1,(previous.height+1)/2);
        current.pixels.resize(current.width*current.height);

        for(int y=0;y<current.height;y++){
            for(int x=0;x<current.width;x++){
                int sourceX=x*2;
                int sourceY=y*2;
                int sourceX1=std::min(sourceX+1,previous.width-1);
                int sourceY1=std::min(sourceY+1,previous.height-1);

                Color c00=previous.pixels[sourceY*previous.width+sourceX];
                Color c10=previous.pixels[sourceY*previous.width+sourceX1];
                Color c01=previous.pixels[sourceY1*previous.width+sourceX];
                Color c11=previous.pixels[sourceY1*previous.width+sourceX1];

                current.pixels[y*current.width+x]=Color(
                    (c00.r+c10.r+c01.r+c11.r)/4,
                    (c00.g+c10.g+c01.g+c11.g)/4,
                    (c00.b+c10.b+c01.b+c11.b)/4,
                    (c00.a+c10.a+c01.a+c11.a)/4
                );
            }
        }

        mipmaps.push_back(current);
    }
}

Color Texture::sampleNearest(double u,double v) const{
    u=std::clamp(u,0.0,1.0);
    v=std::clamp(v,0.0,1.0);
    const MipmapLevel& mipmap=mipmaps[0];
    int x=(int)std::round(u*(mipmap.width-1));
    int y=(int)std::round((1-v)*(mipmap.height-1));
    return mipmap.pixels[y*mipmap.width+x];
}

Color Texture::sampleBilinear(double u,double v,int level) const{
    u=std::clamp(u,0.0,1.0);
    v=std::clamp(v,0.0,1.0);
    level=std::clamp(level,0,(int)mipmaps.size()-1);
    const MipmapLevel& mipmap=mipmaps[level];

    double x=u*(mipmap.width-1);
    double y=(1-v)*(mipmap.height-1);
    int x0=(int)std::floor(x);
    int y0=(int)std::floor(y);
    int x1=std::min(x0+1,mipmap.width-1);
    int y1=std::min(y0+1,mipmap.height-1);
    double tx=x-x0;
    double ty=y-y0;

    Color c00=mipmap.pixels[y0*mipmap.width+x0];
    Color c10=mipmap.pixels[y0*mipmap.width+x1];
    Color c01=mipmap.pixels[y1*mipmap.width+x0];
    Color c11=mipmap.pixels[y1*mipmap.width+x1];

    double topRed=c00.r*(1-tx)+c10.r*tx;
    double topGreen=c00.g*(1-tx)+c10.g*tx;
    double topBlue=c00.b*(1-tx)+c10.b*tx;
    double topAlpha=c00.a*(1-tx)+c10.a*tx;
    double bottomRed=c01.r*(1-tx)+c11.r*tx;
    double bottomGreen=c01.g*(1-tx)+c11.g*tx;
    double bottomBlue=c01.b*(1-tx)+c11.b*tx;
    double bottomAlpha=c01.a*(1-tx)+c11.a*tx;

    return Color(
        (int)std::round(topRed*(1-ty)+bottomRed*ty),
        (int)std::round(topGreen*(1-ty)+bottomGreen*ty),
        (int)std::round(topBlue*(1-ty)+bottomBlue*ty),
        (int)std::round(topAlpha*(1-ty)+bottomAlpha*ty)
    );
}

Color Texture::sampleTrilinear(double u,double v,double dudx,double dvdx,double dudy,double dvdy) const{
    const MipmapLevel& original=mipmaps[0];
    double dx=std::sqrt(
        dudx*dudx*original.width*original.width+
        dvdx*dvdx*original.height*original.height
    );
    double dy=std::sqrt(
        dudy*dudy*original.width*original.width+
        dvdy*dvdy*original.height*original.height
    );
    double level=std::log2(std::max(std::max(dx,dy),1.0));
    level=std::clamp(level,0.0,(double)mipmaps.size()-1);

    int level0=(int)std::floor(level);
    int level1=std::min(level0+1,(int)mipmaps.size()-1);
    double t=level-level0;
    Color color0=sampleBilinear(u,v,level0);
    Color color1=sampleBilinear(u,v,level1);

    return Color(
        (int)std::round(color0.r*(1-t)+color1.r*t),
        (int)std::round(color0.g*(1-t)+color1.g*t),
        (int)std::round(color0.b*(1-t)+color1.b*t),
        (int)std::round(color0.a*(1-t)+color1.a*t)
    );
}
