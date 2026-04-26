#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osgDB/Registry>
#include <osgUtil/Optimizer>
#include <osgUtil/ShaderGen>
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char** argv) {
    // 检查参数
    if (argc < 3) {
        cout << "用法: ./osg_converter <输入.obj路径> <输出.osgb路径>" << endl;
        cout << "示例: ./osg_converter source/model.obj output_model.osgb" << endl;
        return 1;
    }

    string inputPath = argv[1];
    string outputPath = argv[2];

    cout << "正在加载模型: " << inputPath << " ..." << endl;

    // 1. 读取模型
    // 只要你的 .mtl 文件和 .obj 在同级目录，且 .mtl 里的贴图路径是正确的相对路径，
    // OSG 底层插件会自动去把 texture 文件夹下的 .jpg 读进来。
    osg::ref_ptr<osg::Node> model = osgDB::readNodeFile(inputPath);

    if (!model) {
        cerr << "错误: 模型加载失败，请检查路径或 obj/mtl 文件内容！" << endl;
        return 1;
    }

    // 2. 场景图优化
    // 这会将零散的几何体合并，移除冗余节点，极大提升后续光场渲染的帧率
    cout << "正在优化场景图拓扑结构..." << endl;
    osgUtil::Optimizer optimizer;
    optimizer.optimize(model.get(), 
        osgUtil::Optimizer::DEFAULT_OPTIMIZATIONS | 
        osgUtil::Optimizer::INDEX_MESH | 
        osgUtil::Optimizer::VERTEX_PRETRANSFORM);

    // 3. 预处理现代 Shader (一劳永逸解决 Material::apply 报错)
    // 提前在转换阶段把固定管线材质转换为 GLSL 状态
    cout << "正在注入现代 GLSL Shader 状态..." << endl;
    osgUtil::ShaderGenVisitor sgv;
    model->accept(sgv);

    // 4. 写入为 OSGB 二进制文件，并内嵌纹理贴图
    cout << "正在打包并写入: " << outputPath << " ..." << endl;
    
    // 配置写入选项：WriteImageHint=IncludeData 意味着把外部的 jpg 数据直接压缩进 osgb 文件里
    osg::ref_ptr<osgDB::Options> writeOptions = new osgDB::Options("WriteImageHint=IncludeData");
    
    bool success = osgDB::writeNodeFile(*model, outputPath, writeOptions.get());

    if (success) {
        cout << "转换成功！现在你只需要加载 " << outputPath << " 即可。" << endl;
    } else {
        cerr << "错误: 写入文件失败！" << endl;
    }

    return 0;
}
