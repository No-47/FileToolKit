#pragma once
#include <map>
#include <string>
#include <vector>
#include <list>
#include <functional>

struct FileInfor
{
	FileInfor(const std::string& path, const std::string& name) : full_path(path), file_name(name) {};
	~FileInfor() = default;
	FileInfor(const FileInfor& obj)
	{
		full_path = obj.full_path;
		file_name = obj.file_name;
	}
	std::string full_path;
	std::string file_name;
};

class DuplicateFileRemover
{
public:
	DuplicateFileRemover(const std::string& root, const std::string& output) : root_dir(root), out_put_dir(output), ui_progress_bar_obj_ptr(nullptr) {};
	DuplicateFileRemover(const std::string& root, const std::string& output, void* ui_bar_ptr, std::function<void(void*, const int)> call_back) : root_dir(root),
	out_put_dir(output), ui_progress_bar_obj_ptr(ui_bar_ptr), on_update_progress(call_back) {};
	~DuplicateFileRemover() = default;
	void start_work();
	static void find_all_files(const std::string& path, std::list<FileInfor>& output);
	static bool clear_dir(const std::string& target_dir, bool is_root_dir = true);
	static bool delete_file(const std::string& file_to_delete);
private:
	
	void delete_duplicate_files(const std::list<FileInfor>& file_infor_list);
	
	unsigned long long int calc_file_crc(const std::string& path);
	unsigned long long int calc_file_size(const std::string& path);
	std::vector<unsigned char> read_file_data(const std::string& path);
	bool move_file_to_output_dir(const FileInfor& source_file_infor) const;
	bool is_directory_exists(const std::string& path) const;
	
	bool need_to_update_progress_bar_ui() const;
	void update_progress(const int value_to_add) const;
	const std::string root_dir;
	const std::string out_put_dir;
	std::map<unsigned long long int, FileInfor> files_infor_map;
	void* ui_progress_bar_obj_ptr;
	std::function<void(void*, const int)> on_update_progress;
	static constexpr int MAX_PROGRESS_VALUE = 50;		// ui进度条最大值
};

#ifdef USE_DBG
void Test_TraverseDirectory(std::string path);
void test_DFR_tool();
#endif
