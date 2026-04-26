# OBJ to OSGB Modern Converter (obj2osg)

这是一个基于 OpenSceneGraph (OSG) 的轻量级、高性能 3D 模型转换工具。

针对现代 OpenGL 核心模式（Core Profile）和极高帧率的实时渲染需求（如光场显示等对吞吐量要求极高的场景），本工具不仅仅是简单的数据格式转换，更在底层进行了渲染状态的现代化升级与几何拓扑优化。

## ✨ 核心特性

- **一键打包独立资产**：将 `.obj` 模型、`.mtl` 材质文件以及外部的 `.jpg/.png` 贴图文件，完全内嵌打包成一个单一的二进制 `.osgb` 文件。
- **底层拓扑优化**：内置 `osgUtil::Optimizer`，在转换期间自动执行网格索引化（INDEX_MESH）和顶点预变换（VERTEX_PRETRANSFORM），降低 DrawCall，提升高吞吐场景下的实时渲染帧率。

## 🛠️ 环境依赖

本项目在 Ubuntu/Linux 环境下开发与测试，依赖以下基础组件：
- **C++ 14** 或更高版本
- **CMake** (>= 3.10)
- **OpenSceneGraph (OSG)** 核心库（需包含 `osg`, `osgDB`, `osgUtil`）

## 🚀 编译指南

在终端中执行以下命令进行快速编译：

```bash
# 1. 克隆仓库
git clone [https://github.com/donghu-li/obj2osg.git](https://github.com/donghu-li/obj2osg.git)
cd obj2osg

# 2. 创建构建目录并编译
mkdir build
cd build
cmake ..
make
```

编译成功后，会在 `build` 目录下生成可执行文件 `osg_converter`。

## 📖 使用说明

基本使用格式：

Bash

```
./osg_converter <输入.obj路径> <输出.osgb路径>
```

### 示例用法

Bash

```
./osg_converter ../source/model.obj ../output/model_optimized.osgb
```

### ⚠️ 重要提示：关于贴图丢失的排查

如果转换出的 `.osgb` 模型没有贴图，请务必使用文本编辑器打开原始的 `.mtl` 文件，检查其中的贴图路径配置是否正确。 OSG 底层依赖 `.mtl` 中记录的相对路径去寻找图片。

- **错误示范**：`map_Kd diffuse.jpg` （如果图片不在 `.mtl` 同级目录，将读取失败）
- **正确示范**：`map_Kd ../texture/diffuse.jpg` （确保相对路径正确指向图片文件） 只要路径正确，本工具会自动将所有外部贴图封存进生成的 `.osgb` 文件中。

## 📄 License

MIT Licens
