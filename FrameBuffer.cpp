#include "FrameBuffer.h"
#include <fstream>
FrameBuffer::FrameBuffer(int w, int h) : width(w), height(h) {
    samples.resize(height,vector<array<Sample,4>>(width));
}

void FrameBuffer::setSample(int x,int y,int index,double depth,Color color,bool blend,bool depthWrite){
    if(x>=0&&x<width&&y>=0&&y<height&&index>=0&&index<4){
        if(depth<samples[y][x][index].depth){
            if(blend){
                double alpha=color.a/255.0;
                Color destination=samples[y][x][index].color;
                samples[y][x][index].color=Color(
                    (int)(color.r*alpha+destination.r*(1-alpha)),
                    (int)(color.g*alpha+destination.g*(1-alpha)),
                    (int)(color.b*alpha+destination.b*(1-alpha)),
                    (int)(color.a+destination.a*(1-alpha))
                );
            }else{
                samples[y][x][index].color=color;
            }
            if(depthWrite){
                samples[y][x][index].depth=depth;
            }
        }
    }
}
void FrameBuffer::savePPM(const string& filename){
    ofstream out(filename);
    out<<"P3"<<endl;// P3表示ASCII编码的PPM格式
    out<<width<<" "<<height<<endl;
    out<<"255"<<endl;
    for(int y=0;y<height;y++){
        for(int x=0;x<width;x++){
            Color c(
                (samples[y][x][0].color.r+samples[y][x][1].color.r+samples[y][x][2].color.r+samples[y][x][3].color.r)/4,
                (samples[y][x][0].color.g+samples[y][x][1].color.g+samples[y][x][2].color.g+samples[y][x][3].color.g)/4,
                (samples[y][x][0].color.b+samples[y][x][1].color.b+samples[y][x][2].color.b+samples[y][x][3].color.b)/4
            );
            out<<c.r<<" "<<c.g<<" "<<c.b<<endl;
        }
    }
    out.close();
}
void FrameBuffer::clear(){
    for(int i=0;i<height;i++){
        for(int j=0;j<width;j++){
            for(int k=0;k<4;k++){
                samples[i][j][k].color=Color(0,0,0);
                samples[i][j][k].depth=numeric_limits<double>::infinity();
            }
        }
    }
}
int FrameBuffer::getWidth() const {
    return width;
}
int FrameBuffer::getHeight() const {
    return height;
}
