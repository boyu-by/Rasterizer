# CPU Rasterizer Learning Project

这是一个从零实现的 C++ CPU 光栅器，用于记录计算机图形学与现代实时渲染的学习过程。

## 当前进度

- 帧缓冲与 PPM 图像输出
- 三角形包围盒遍历
- 基于边函数的三角形覆盖判断
- 2×2 超采样
- `Vector2` / `Vector3` / `Vector4`
- `Matrix4` 与矩阵乘法
- 平移、缩放、绕 X/Y/Z 轴旋转
- Model Matrix 组合
- `lookAt` View Matrix
- Model → World → View 变换链
- 透视投影、透视除法与 Viewport 变换
- Clip Space 六平面齐次裁剪
- Sutherland–Hodgman 多边形裁剪
- 裁剪结果的三角扇拆分
- 重心坐标计算与三角形内外判断
- 顶点颜色属性
- 裁剪阶段的颜色插值
- 基于重心坐标的顶点颜色插值
- 保留 Clip Space 中的 `1/w`
- 透视正确的顶点颜色插值
- RGB 渐变三角形
- UV 纹理坐标
- 裁剪阶段的 UV 插值
- 透视正确的 UV 插值
- `Texture` 纹理类
- P3 PPM 图片纹理读取
- UV Clamp 与最近邻采样
- 双线性纹理过滤
- Mipmap 自动生成
- 基于 UV 屏幕变化的 LOD 计算
- 三线性纹理过滤
- 每个 MSAA 采样点独立保存颜色与深度
- NDC 深度插值与深度测试
- 2×2 MSAA Resolve
- 多三角形遮挡与绘制顺序验证
- Alpha 属性的裁剪与透视正确插值
- Source Over 透明度混合
- 不透明与透明渲染阶段
- 透明三角形从远到近排序
- 透明片元开启深度测试并关闭深度写入
- 屏幕空间背面剔除

基础 CPU Rasterizer 第一阶段已完成。

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
→ 背面剔除
→ 2×2 MSAA 覆盖测试
→ 屏幕空间重心坐标
→ 使用 1/w 修正插值权重
→ 颜色与 UV 顶点属性插值
→ 计算纹理 LOD
→ Mipmap 三线性采样
→ 不透明片元深度测试与写入
→ 透明三角形从远到近排序
→ 透明片元深度测试与 Alpha 混合
→ MSAA Resolve
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
5. UV 纹理坐标与透视正确插值
6. 纹理采样与过滤
7. 深度缓冲、遮挡与透明度
8. 基础光照与 MSAA
