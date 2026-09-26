# CPU Rasterizer Learning Project

这是一个从零实现的 C++ CPU 光栅器，用于记录计算机图形学与现代实时渲染的学习过程。

## 当前进度

- 帧缓冲与 PPM 图像输出
- 三角形包围盒遍历
- 基于边函数的三角形覆盖判断
- 2×2 超采样
- `Vector3` / `Vector4`
- `Matrix4` 与矩阵乘法
- 平移、缩放、绕 X/Y/Z 轴旋转
- Model Matrix 组合
- `lookAt` View Matrix



## 构建

项目使用 CMake 和 C++17：

```bash
cmake -S . -B build
cmake --build build
```

运行程序后会在当前目录生成 `out.ppm`。

## 学习路线

1. 变换矩阵与相机
2. 透视投影与屏幕映射
3. 重心坐标与属性插值
4. 深度缓冲与遮挡
5. 齐次裁剪
6. 纹理、基础光照与 MSAA

