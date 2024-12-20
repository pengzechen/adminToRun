#include <stdio.h>
#include <tinyxml2.h>

using namespace tinyxml2;

void create_xml_file(const char *filepath, const char *path_value) {
    // 创建一个 XML 文档
    XMLDocument doc;

    // 声明 XML 声明
    XMLNode *decl = doc.NewDeclaration("xml version=\"1.0\" encoding=\"UTF-8\""); 
    doc.InsertFirstChild(decl);

    // 创建根节点
    XMLElement *root = doc.NewElement("ImplementationManifest");
    root->SetAttribute("xsi:schemaLocation", "http://www.asam.net/XIL/ImplementationManifest/2.0 ImplementationManifest.xsd");
    root->SetAttribute("xmlns", "http://www.asam.net/XIL/ImplementationManifest/2.0");
    root->SetAttribute("xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance");
    doc.InsertEndChild(root);

    // 创建子节点 NetTestbenchImplementation
    XMLElement *netTestbench = doc.NewElement("NetTestbenchImplementation");
    netTestbench->SetAttribute("VendorName", "Hirain GmbH");
    netTestbench->SetAttribute("ProductName", "EES");
    netTestbench->SetAttribute("ProductVersion", "2.2.0");
    netTestbench->SetAttribute("XilVersion", "2.1.0");
    netTestbench->SetAttribute("TechnologyVersion", "4.8");
    root->InsertEndChild(netTestbench);

    // 添加子节点 AssemblyName
    XMLElement *assemblyName = doc.NewElement("AssemblyName");
    assemblyName->SetText("ASAM.XIL.Implementation.Hirain.TestBench");
    netTestbench->InsertEndChild(assemblyName);

    // 添加子节点 AssemblyPath
    XMLElement *assemblyPath = doc.NewElement("AssemblyPath");
    assemblyPath->SetText(path_value);
    netTestbench->InsertEndChild(assemblyPath);

    // 添加子节点 AssemblyVersion
    XMLElement *assemblyVersion = doc.NewElement("AssemblyVersion");
    assemblyVersion->SetText("1.0.0.0");
    netTestbench->InsertEndChild(assemblyVersion);

    // 添加子节点 AssemblyKeyToken
    XMLElement *assemblyKeyToken = doc.NewElement("AssemblyKeyToken");
    assemblyKeyToken->SetText("f09dff00f4fc384f");
    netTestbench->InsertEndChild(assemblyKeyToken);

    // 添加子节点 AssemblyCulture
    XMLElement *assemblyCulture = doc.NewElement("AssemblyCulture");
    assemblyCulture->SetText("neutral");
    netTestbench->InsertEndChild(assemblyCulture);

    // 添加子节点 TestbenchClassName
    XMLElement *testbenchClassName = doc.NewElement("TestbenchClassName");
    testbenchClassName->SetText("ASAM.XIL.Implementation.Hirain.TestBench.HirainTestBench");
    netTestbench->InsertEndChild(testbenchClassName);

    // 保存 XML 文件
    if (doc.SaveFile(filepath) != XML_SUCCESS) {
        printf("Failed to save XML file: %s\n", filepath);
    } else {
        printf("XML file created successfully: %s\n", filepath);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <path_value>\n", argv[0]);
        return 1;
    }

    const char *output_file = "HirainEESImplementationManifest_2_2_0.imf";
    const char *path_value = argv[1];

    create_xml_file(output_file, path_value);
    return 0;
}

