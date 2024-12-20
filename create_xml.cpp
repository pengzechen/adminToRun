#include <iostream>
#include <windows.h> // For Windows API functions
#include <tinyxml2.h>
#include <string>
#include <string.h>

using namespace tinyxml2;

void create_directory(const std::string &path) {
    size_t pos = 0;
    std::string partialPath;

    // 遍历每一级目录，逐级创建
    while ((pos = path.find_first_of("\\/", pos)) != std::string::npos) {
        partialPath = path.substr(0, pos++);
        if (!partialPath.empty() && GetFileAttributesA(partialPath.c_str()) == INVALID_FILE_ATTRIBUTES) {
            if (!CreateDirectoryA(partialPath.c_str(), NULL)) {
                std::cerr << "Failed to create directory: " << partialPath << std::endl;
                return;
            }
        }
    }

    // 创建最后一级目录
    if (!path.empty() && GetFileAttributesA(path.c_str()) == INVALID_FILE_ATTRIBUTES) {
        if (!CreateDirectoryA(path.c_str(), NULL)) {
            std::cerr << "Failed to create directory: " << path << std::endl;
        }
    }
}

void create_xml_file(const char *directory) {
    // 指定目标目录
    std::string outputDir = directory;

    // 检查并创建目录
    create_directory(outputDir);

    // 目标文件路径
    std::string outputFile = outputDir + "\\HirainEESImplementationManifest_2_2_0.imf";

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
    if (doc.SaveFile(outputFile.c_str()) != tinyxml2::XML_SUCCESS) {
        std::cerr << "Failed to save XML file: " << outputFile << std::endl;
    } else {
        std::cout << "XML file created successfully: " << outputFile << std::endl;
    }
}

bool is_admin() {
    BOOL isAdmin = FALSE;
    PSID administratorsGroup = NULL;

    SID_IDENTIFIER_AUTHORITY NtAuthority = SECURITY_NT_AUTHORITY;
    if (AllocateAndInitializeSid(&NtAuthority, 2, SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0, &administratorsGroup)) {
        CheckTokenMembership(NULL, administratorsGroup, &isAdmin);
        FreeSid(administratorsGroup);
    }
    return isAdmin == TRUE;
}

void request_admin_permissions() {
    // 获取当前程序路径
    char modulePath[MAX_PATH];
    if (GetModuleFileNameA(NULL, modulePath, MAX_PATH) == 0) {
        std::cerr << "Failed to get module path" << std::endl;
        return;
    }

    // 创建 SHELLEXECUTEINFO 结构体
    SHELLEXECUTEINFO seInfo;
    ZeroMemory(&seInfo, sizeof(seInfo));
    seInfo.cbSize = sizeof(seInfo);
    seInfo.fMask = SEE_MASK_DEFAULT;
    seInfo.hwnd = NULL;
    seInfo.lpVerb = "runas"; // 请求管理员权限
    seInfo.lpFile = modulePath;
    seInfo.lpParameters = ""; // 可选参数
    seInfo.lpDirectory = NULL;
    seInfo.nShow = SW_SHOWNORMAL;

    if (!ShellExecuteEx(&seInfo)) {
        std::cerr << "Failed to request admin privileges" << std::endl;
    }
    exit(0); // 请求权限后退出当前进程
}

int main() {
	// 如果没有管理员权限，提示用户请求管理员权限
    if (!is_admin()) {
        std::cout << "This program requires administrator privileges to run. Please allow the program to restart with elevated privileges." << std::endl;
        // request_admin_permissions();
    }
	
    const char *directory = "C:\\ProgramData\\ASAM\\XIL\\Implementation";
    create_xml_file(directory);
	std::cout << "press Enter to exit." << std::endl;
	getchar();
    return 0;
}
