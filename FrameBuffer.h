#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include "Color.h"
using namespace std;
class FrameBuffer{
    private:
        int width;
        int height;
        vector<vector<Color>> pixels;
    public:
        FrameBuffer(int w, int h);
        //设置像素点颜色
        void setPixel(int x, int y, Color c);
        //保存为PPM格式
        void savePPM(const string& filename);
        void clear();
        int getWidth() const;
        int getHeight() const;
};
