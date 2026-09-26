#include "FrameBuffer.h"
#include <fstream>
FrameBuffer::FrameBuffer(int w, int h) : width(w), height(h) {
    pixels.resize(height, vector<Color>(width));
}

void FrameBuffer::setPixel(int x, int y, Color c) {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        pixels[y][x] = c;
    }
}
void FrameBuffer::savePPM(const string& filename){
    ofstream out(filename);
    out<<"P3"<<endl;// P3表示ASCII编码的PPM格式
    out<<width<<" "<<height<<endl;
    out<<"255"<<endl;
    for(int y=0;y<height;y++){
        for(int x=0;x<width;x++){
            Color c=pixels[y][x];
            out<<c.r<<" "<<c.g<<" "<<c.b<<endl;
        }
    }
    out.close();
}
void FrameBuffer::clear(){
    for(int i=0;i<height;i++){
        for(int j=0;j<width;j++){
            pixels[i][j]={0,0,0};
        }
    }
}
int FrameBuffer::getWidth() const {
    return width;
}
int FrameBuffer::getHeight() const {
    return height;
}
