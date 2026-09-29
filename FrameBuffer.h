#pragma once
#include <array>
#include <iostream>
#include <limits>
#include <vector>
#include <fstream>
#include <string>
#include "Color.h"
using namespace std;

struct Sample{
    Color color;
    double depth;
    Sample():color(0,0,0),depth(numeric_limits<double>::infinity()){}
};

class FrameBuffer{
    private:
        int width;
        int height;
        vector<vector<array<Sample,4>>> samples;
    public:
        FrameBuffer(int w, int h);
        //设置采样点颜色和深度
        void setSample(int x,int y,int index,double depth,Color color);
        //保存为PPM格式
        void savePPM(const string& filename);
        void clear();
        int getWidth() const;
        int getHeight() const;
};
