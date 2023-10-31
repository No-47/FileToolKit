#ifdef HAS_GUI
#include "RemoverUI.h"
#endif  // HAS_GUI
#include <iostream>
#include <windows.h>
#include "DuplicateFileRemover.h"
#include "LogSystem.h"


void run_app_cmd()
{
    std::string root_dir;
    std::string output_dir;
    std::cout << "This is a duplicate file removal tool...." << std::endl;
    std::cout << "Please set the folder path to delete duplicate files：" << std::endl;
	std::getline(std::cin, root_dir);
    std::cout << "The path you entered is：" << root_dir << ".   The program will delete duplicate files in this folder" << std::endl;
    std::cout << "Please set the output folder directory (the deduplicated files will be saved in this folder). "
				 "If the input folder does not exist, it will be created automatically.：" << std::endl;
    std::getline(std::cin, output_dir);

    std::cout << "Start deleting duplicate files. If there are many files, it will take a while, please wait patiently. Don't close the program..........." << std::endl;
    DuplicateFileRemover my_remover(root_dir, output_dir);
    my_remover.start_work();
    std::cout << "The program execution is completed...the files after deduplication are saved in the path:" << output_dir << std::endl;
    std::cout << "You can now exit the program~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
}

#ifndef HAS_GUI
int main()
{
    DBGOUT("Hello World!");
#ifdef USE_DBG
    // 测试代码
    //Test_TraverseDirectory("C:\\Users\\Administrator\\Desktop\\test");
    //test_DFR_tool();
#endif      // USE_DBG
    run_app_cmd();
    system("pause");
}
#else
IMPLEMENT_APP(DFRApp)
#endif

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件