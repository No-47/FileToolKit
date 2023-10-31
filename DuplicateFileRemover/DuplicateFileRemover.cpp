// DuplicateFileRemover.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include <windows.h>
#include "DuplicateFileRemover.h"
#include "LogSystem.h"
#include "Crc64.h"


void DuplicateFileRemover::start_work()
{
	do
	{
        const int value_to_add = MAX_PROGRESS_VALUE / 3;
        if (is_directory_exists(out_put_dir) == false)
        {
            if (CreateDirectoryA(out_put_dir.c_str(), nullptr) == false)
            {
                DBGOUT("CreateDirectoryA Failed. error:%d", GetLastError());
                break;
            }
            DBGOUT("CreateDirectoryA successfully:%s", out_put_dir.c_str());
        }
        else
        {
            DBGOUT("out_put_dir already existed:%s, start to clear this dir...", out_put_dir.c_str());
            if(clear_dir(out_put_dir) == false)
            {
                DBGOUT("clear_dir(%s) failed...", out_put_dir.c_str());
            }
        }
        std::list<FileInfor> all_files_infor;
        find_all_files(root_dir, all_files_infor);
        update_progress(value_to_add);
        if (all_files_infor.empty() == true)
        {
            DBGOUT("no files found in %s...", root_dir.c_str());
            break;
        }
        delete_duplicate_files(all_files_infor);
        update_progress(value_to_add);
        for (auto my_pos = files_infor_map.begin(); my_pos != files_infor_map.end(); my_pos++)
        {
            DBGOUT("moving file %s to output %s", my_pos->second.full_path.c_str(), out_put_dir.c_str());
            if (move_file_to_output_dir(my_pos->second) == false)
            {
                DBGOUT("move_file_to_output_dir failed. file:%s", my_pos->second.full_path.c_str());
            }
        }
        update_progress(MAX_PROGRESS_VALUE - value_to_add * 2);
	}
	while (false);
}

void DuplicateFileRemover::find_all_files(const std::string& path, std::list<FileInfor>& output)
{
    HANDLE h_find = INVALID_HANDLE_VALUE;
    do
    {
        WIN32_FIND_DATAA find_data;
        std::string file_spec = path + "\\*";
        h_find = FindFirstFileA(file_spec.c_str(), &find_data);

        if (h_find == INVALID_HANDLE_VALUE)
        {
            DBGOUT("无法打开目录：%s   error:%d", file_spec.c_str(), GetLastError());
            break;
        }

        do
        {
            if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                if (strcmp(find_data.cFileName, ".") != 0 && strcmp(find_data.cFileName, "..") != 0)
                {
                    DBGOUT("目录：%s", find_data.cFileName);
                    find_all_files(path + "\\" + find_data.cFileName, output);
                }
            }
            else
            {
                std::string full_path(path);
                full_path += "\\";
                full_path += find_data.cFileName;
                DBGOUT("full_path:%s", full_path.c_str());
                DBGOUT("文件名：%s", find_data.cFileName);
                FileInfor infor_to_save(full_path, find_data.cFileName);
                output.push_back(infor_to_save);
            }
        } while (FindNextFileA(h_find, &find_data) != 0);
    } while (false);
    if (h_find != INVALID_HANDLE_VALUE)
    {
        FindClose(h_find);
    }
}

bool DuplicateFileRemover::clear_dir(const std::string& target_dir, bool is_root_dir /*= true*/)
{
    bool clear_result = false;
    HANDLE h_find = INVALID_HANDLE_VALUE;
    do
    {
        /*if (RemoveDirectoryA(target_dir.c_str()) == TRUE)
        {
            clear_result = true;
            break;
        }*/
        DWORD error = GetLastError();
        if (error == ERROR_DIR_NOT_EMPTY)
        {
            // 文件夹非空，需要先删除文件夹中的内容
            // 递归删除文件夹中的文件和子文件夹
            std::string search_path(target_dir);
            search_path += "\\*.*";
            //wchar_t searchPath[MAX_PATH];
            //wcscpy_s(searchPath, target_dir);
            //wcscat_s(searchPath, L"\\*.*");

            WIN32_FIND_DATAA find_Data;
            h_find = FindFirstFileA(search_path.c_str(), &find_Data);
            if (h_find == INVALID_HANDLE_VALUE)
            {
                DBGOUT("无法打开目录：%s   error:%d", search_path.c_str(), GetLastError());
                break;
            }
            do
            {
                if (strcmp(find_Data.cFileName, ".") != 0 && strcmp(find_Data.cFileName, "..") != 0)
                {
                    std::string file_path(target_dir);
                    file_path += "\\";
                    file_path += find_Data.cFileName;
                    //wchar_t filePath[MAX_PATH];
                    //wcscpy_s(filePath, target_dir);
                    //wcscat_s(filePath, L"\\");
                    //wcscat_s(filePath, find_Data.cFileName);

                    if (find_Data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                    {
                        // 子项是一个子文件夹，递归删除
                        clear_dir(file_path, false);    // 子文件第二个参数设置为false
                    }
                    else
                    {
                        // 子项是一个文件，直接删除
                        DeleteFileA(file_path.c_str());
                    }
                }
            } while (FindNextFileA(h_find, &find_Data));
            // 删除空的子文件夹，根目录不删除
            if (is_root_dir == false)
            {
                if (RemoveDirectoryA(target_dir.c_str()))
                {
                    clear_result = true;
                    break;
                }
            }
            
        }
        else
        {
            DBGOUT("RemoveDirectoryA(%s) failed. error:%d", target_dir.c_str(), error);
        }
    }
    while (false);
    if (h_find != INVALID_HANDLE_VALUE)
    {
        FindClose(h_find);
    }
    return clear_result;
    
}

bool DuplicateFileRemover::delete_file(const std::string& file_to_delete)
{
    bool ret = false;
    do
    {
        if (file_to_delete.empty() == true)
        {
            DBGOUT("file_to_delete can not be empty");
            break;
        }
        if (DeleteFileA(file_to_delete.c_str()))
        {
            DBGOUT("delete file:%s", file_to_delete.c_str());
        }
        else
        {
            DBGOUT("DeleteFileA failed target:%s    error:%d", file_to_delete.c_str(), GetLastError());
        }
        ret = true;
    } while (false);
    return ret;

}

void DuplicateFileRemover::delete_duplicate_files(const std::list<FileInfor>& file_infor_list)
{
    for (auto my_pos = file_infor_list.begin(); my_pos != file_infor_list.end(); ++my_pos)
    {
        std::string file_full_path(my_pos->full_path);
        const unsigned long long int file_crc = calc_file_crc(file_full_path);
        auto find_it = files_infor_map.find(file_crc);
        if (find_it != files_infor_map.end())
        {
            // 找到了相同文件就删除
            if (delete_file(file_full_path) == false)
            {
                DBGOUT("delete_file failed. target delete full_path:%s", file_full_path.c_str());
            }
        }
        else
        {
            // 文件不相同就保存信息
            files_infor_map.insert(std::make_pair(file_crc, *my_pos));
        }
    }
}

unsigned long long int DuplicateFileRemover::calc_file_crc(const std::string& path)
{
    unsigned long long int crc_64 = static_cast<unsigned long long int>(-1);
    do
    {
	    if (path.empty() == true)
	    {
            DBGOUT("path is empty...");
            break;
	    }
        const std::vector<unsigned char> file_data = read_file_data(path);
	    if (file_data.empty() == true)
	    {
            DBGOUT("read_file_data failed. return empty data...");
		    break;
	    }
        crc_64 = crc64(0, file_data.data(), file_data.size());
        DBGOUT("calc file crc64 :%llu", crc_64);
    }
    while (false);
    return crc_64;
}

unsigned long long int DuplicateFileRemover::calc_file_size(const std::string& path)
{
    uint64_t ret_result = 0;
    //std::ifstream file_stream;
    FILE* file_fp = nullptr;
    do
    {
        if (path.empty() == true)
        {
            break;
        }
        //file_stream.open(file_path, std::ios::binary);
        file_fp = fopen(path.c_str(), "rb");
        if (file_fp == nullptr)
        {
            break;
        }
        //file_stream.seekg(0, std::ios::end);
        fseek(file_fp, 0, SEEK_END);
        // ret_result = file_stream.tellg();
        ret_result = ftell(file_fp);
    } while (false);
    if (file_fp != nullptr)
    {
        fclose(file_fp);
    }
    return ret_result;
}

std::vector<unsigned char> DuplicateFileRemover::read_file_data(const std::string& path)
{
    FILE* target_file_fp = nullptr;
    std::vector<unsigned char> file_data;
    do
    {
        uint64_t file_size = calc_file_size(path);
        if (file_size == 0)
        {
            break;
        }
        file_data.assign(file_size, 0);
        target_file_fp = fopen(path.c_str(), "rb");
        if (target_file_fp == nullptr)
        {
            break;
        }
        fread(file_data.data(), file_data.size(), 1, target_file_fp);

    } while (false);
    if (target_file_fp != nullptr)
    {
        fclose(target_file_fp);
    }
    return file_data;
}

bool DuplicateFileRemover::move_file_to_output_dir(const FileInfor& source_file_infor) const
{
    bool move_result = false;
    do
    {
	    if (source_file_infor.full_path.empty() == true || source_file_infor.file_name.empty() == true)
	    {
            DBGOUT("source_file_path can not be empty");
            break;
	    }
    	std::string output_path(out_put_dir);
        output_path += "\\";
        output_path += source_file_infor.file_name;
        DBGOUT("MoveFileA from %s to %s", source_file_infor.full_path.c_str(), output_path.c_str());
        move_result = MoveFileA(source_file_infor.full_path.c_str(), output_path.c_str());
	    if (move_result == false)
	    {
            DBGOUT("MoveFileA Failed. error:%d", GetLastError());
            break;
	    }
    }
    while (false);
    return move_result;
}

bool DuplicateFileRemover::is_directory_exists(const std::string& path) const
{
    bool check_result = false;
    do
    {
	    if (path.empty() == true)
	    {
            DBGOUT("param path can is empty");
		    break;
	    }
        WIN32_FIND_DATAA find_data;
        const std::string file_spec = path;
        const HANDLE h_find = FindFirstFileA(file_spec.c_str(), &find_data);

        if ((h_find != INVALID_HANDLE_VALUE) && (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
        {
            check_result = true;
            FindClose(h_find);
        }
        // int ret = _access(path.c_str(), 0);
    }
    while (false);
    return check_result;
    
}

bool DuplicateFileRemover::need_to_update_progress_bar_ui() const
{
    return (on_update_progress != nullptr) && (ui_progress_bar_obj_ptr != nullptr);
}

void DuplicateFileRemover::update_progress(const int value_to_add) const
{
	if (value_to_add < 0)
	{
        DBGOUT("param value_to_add must bigger than zero");
	}
	else
	{
        if (need_to_update_progress_bar_ui() == true)
        {
            on_update_progress(ui_progress_bar_obj_ptr, value_to_add);
        }
	}
	
}

#ifdef USE_DBG
void Test_TraverseDirectory(std::string path)
{
    WIN32_FIND_DATAA findData;
    HANDLE hFind = INVALID_HANDLE_VALUE;
    std::string fileSpec = path + "\\*";

    hFind = FindFirstFileA(fileSpec.c_str(), &findData);

    if (hFind == INVALID_HANDLE_VALUE)
    {
        DBGOUT("无法打开目录：%s", path.c_str());
        return;
    }

    do
    {
        if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            if (strcmp(findData.cFileName, ".") != 0 && strcmp(findData.cFileName, "..") != 0)
            {
                DBGOUT("目录：%s", findData.cFileName);
                Test_TraverseDirectory(path + "\\" + findData.cFileName);
            }
        }
        else
        {
            std::string full_path(path);
            full_path += "\\";
            full_path += findData.cFileName;
            DBGOUT("full_path:%s", full_path.c_str());
            DBGOUT("文件名：%s", findData.cFileName);
        }
    } while (FindNextFileA(hFind, &findData) != 0);

    FindClose(hFind);
}

void test_DFR_tool()
{
    char buffer[MAX_PATH] = { 0 };
    GetCurrentDirectoryA(MAX_PATH, buffer);
    DBGOUT("当前工作路径:%s", buffer);
    std::string current_dir(buffer);
    current_dir += "\\remover_output";
    DuplicateFileRemover my_remover("C:\\Users\\Administrator\\Desktop\\test", current_dir);
    my_remover.start_work();
}

#endif      // USE_DBG