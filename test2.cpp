#include <iostream>
#include <windows.h> // For GetModuleFileNameA and related functions
#include <tinyxml2.h>
#include <string>

using namespace tinyxml2;

void create_xml_file(const char *filepath) {
    // 获取程序所在目录
    char modulePath[MAX_PATH];
    if (GetModuleFileNameA(nullptr, modulePath, MAX_PATH) == 0) {
        std::cerr << "Failed to get module path" << std::endl;
        return;
    }

    // 找到最后一个反斜杠的位置，提取目录
    std::string moduleDir(modulePath);
    size_t pos = moduleDir.find_last_of("\\/");
    if (pos != std::string::npos) {
        moduleDir = moduleDir.substr(0, pos); // 保留目录部分
    }

    // 创建一个 XML 文档
    tinyxml2::XMLDocument doc;

    // 声明 XML 声明
    tinyxml2::XMLNode *decl = doc.NewDeclaration("xml version=\"1.0\" encoding=\"UTF-8\"");
    doc.InsertFirstChild(decl);

    // 创建根节点
    tinyxml2::XMLElement *root = doc.NewElement("ImplementationManifest");
    root->SetAttribute("xsi:schemaLocation", "http://www.asam.net/XIL/ImplementationManifest/2.0 ImplementationManifest.xsd");
    root->SetAttribute("xmlns", "http://www.asam.net/XIL/ImplementationManifest/2.0");
    root->SetAttribute("xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance");
    doc.InsertEndChild(root);

    // 创建子节点 NetTestbenchImplementation
    tinyxml2::XMLElement *netTestbench = doc.NewElement("NetTestbenchImplementation");
    netTestbench->SetAttribute("VendorName", "Hirain GmbH");
    netTestbench->SetAttribute("ProductName", "EES");
    netTestbench->SetAttribute("ProductVersion", "2.2.0");
    netTestbench->SetAttribute("XilVersion", "2.1.0");
    netTestbench->SetAttribute("TechnologyVersion", "4.8");
    root->InsertEndChild(netTestbench);

    // 添加子节点 AssemblyName
    tinyxml2::XMLElement *assemblyName = doc.NewElement("AssemblyName");
    assemblyName->SetText("ASAM.XIL.Implementation.Hirain.TestBench");
    netTestbench->InsertEndChild(assemblyName);

    // 添加子节点 AssemblyPath
    tinyxml2::XMLElement *assemblyPath = doc.NewElement("AssemblyPath");
    assemblyPath->SetText(moduleDir.c_str()); // 使用程序目录
    netTestbench->InsertEndChild(assemblyPath);

    // 添加子节点 AssemblyVersion
    tinyxml2::XMLElement *assemblyVersion = doc.NewElement("AssemblyVersion");
    assemblyVersion->SetText("1.0.0.0");
    netTestbench->InsertEndChild(assemblyVersion);

    // 添加子节点 AssemblyKeyToken
    tinyxml2::XMLElement *assemblyKeyToken = doc.NewElement("AssemblyKeyToken");
    assemblyKeyToken->SetText("f09dff00f4fc384f");
    netTestbench->InsertEndChild(assemblyKeyToken);

    // 添加子节点 AssemblyCulture
    tinyxml2::XMLElement *assemblyCulture = doc.NewElement("AssemblyCulture");
    assemblyCulture->SetText("neutral");
    netTestbench->InsertEndChild(assemblyCulture);

    // 添加子节点 TestbenchClassName
    tinyxml2::XMLElement *testbenchClassName = doc.NewElement("TestbenchClassName");
    testbenchClassName->SetText("ASAM.XIL.Implementation.Hirain.TestBench.HirainTestBench");
    netTestbench->InsertEndChild(testbenchClassName);

    // 保存 XML 文件
    if (doc.SaveFile(filepath) != tinyxml2::XML_SUCCESS) {
        std::cerr << "Failed to save XML file: " << filepath << std::endl;
    } else {
        std::cout << "XML file created successfully: " << filepath << std::endl;
    }
}

int main(int argc, char *argv[]) {
    const char *output_file = "HirainEESImplementationManifest_2_2_0.imf";
    create_xml_file(output_file);
    return 0;
}
