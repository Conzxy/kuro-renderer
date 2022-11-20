# Kuro Software Renderer
不依赖OpenGL API的基于CPU的软渲染器(Software Renderer)。

## Features
- [x] Bresenman's line drawing algorithm
- [x] Triangle drawing algorithm based on barycentric coordinates
- [x] View transformation
- [x] Prespective projection transformation
- [x] Orbit camera control(Like Three.js)
- [ ] Blinn-Phong shader

## TODO
* Handle wavefront obj file correctly
  - [x] process polygon 4 vertexes
  - [x] | Vertex coordinate | > 1
    * 通过Model Matrix调整模型的摆放
  - [ ] No normal or uv coordinate
  - [ ] Reference to MTL file
- [ ] homogenenous clipping
- [ ] perspective corrention interpolation
- [ ] 实现针对模型本身的缩放，旋转，平移（GUI上下文菜单互斥实现）
- [ ] camera变为全局环绕相机（待测试）
- [ ] 加载多个模型，包括模型的多个部位，记录模型矩阵

