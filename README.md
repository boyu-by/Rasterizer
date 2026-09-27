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
- Model → World → View 变换链
- 透视投影、透视除法与 Viewport 变换
- Clip Space 六平面齐次裁剪
- Sutherland–Hodgman 多边形裁剪
- 裁剪结果的三角扇拆分

当前变换管线：

```text
Object Space
→ Model
→ World Space
→ View
→ View Space
→ Projection
→ Clip Space
→ 齐次裁剪
→ 透视除法
→ NDC
→ Viewport
→ Screen Space
→ 光栅化
```

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
3. 齐次裁剪
4. 重心坐标与属性插值
5. 深度缓冲与遮挡
6. 纹理、基础光照与 MSAA

下一步：实现深度缓冲，处理多个三角形之间的前后遮挡。
