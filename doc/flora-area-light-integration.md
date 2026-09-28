# Flora 矩形光与延迟光照接口

此改动为 Flora 的逐灯可见性积分提供接口。`FLORA_lights_area` v1 节点扩展携带
`type="rect"`、`color`、`radiance`、`halfU` 和 `halfV`；辐亮度与面积分开保存。
节点变换作用于中心与半边向量，发光面法线为 `-normalize(cross(halfU, halfV))`。
节点不能同时挂载网格或点光源。扩展不是 Khronos 标准。

`RectLight` 可被场景工厂创建、克隆和导入。延迟光照增加独立的面积光漫反射与
镜面反射输入，未提供输入时绑定黑色纹理；最多支持 32 个灯及对应阴影通道。
世界坐标先在视图空间完成深度重建，再变换到世界空间，以降低平移场景的消减误差。
面积光积分由 Flora 提供，Donut 中的点光源循环不会再次累加矩形光能量。

现有 `deferred_lighting_cs.hlsl` 的 ShaderMake 注册继续覆盖此修改，无新增入口。
CPU/HLSL 灯数量与常量布局必须同步；与 Flora 一起构建 `FloraRenderPyNative` 验证。

对应集成测试位于父工程 Flora，并已注册为 CTest GPU 测试：

- `tests/rect_area_light_smoke.py`：面积积分、变换、背面、距离和第 22/32 灯槽。
- `tests/rect_specular_sampling_smoke.py`：镜面能量与跨帧采样。
- `tests/deferred_shadow_precision_smoke.py`：平移与掠射角下的阴影重建。

运行 `ctest -C Release --output-on-failure -R "flora-(rect_|deferred_)"`。
这些测试通过无窗口 Vulkan 渲染检查实际导入与着色，不依赖手工搭建场景。
