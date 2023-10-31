#include <wx/dir.h>
#include <wx/file.h>
#include "RemoverUI.h"
#include "LogSystem.h"
#include "DuplicateFileRemover.h"


DFRMainWindow::DFRMainWindow( wxWindow* parent )
:
MainFrame( parent )
{
	DBGOUT("DFRMainWindow init....");
	SetIcon(wxICON(title_icon));
	work_dir_picker->GetTextCtrl()->SetEditable(false);
	output_dir_picker->GetTextCtrl()->SetEditable(false);
	backup_dir_picker->GetTextCtrl()->SetEditable(false);
}

bool DFRMainWindow::is_empty_folder(const wxString& target_folder_dir)
{
	bool check_result = true;
	do
	{
		const wxDir target_dir(target_folder_dir);
		if (target_dir.IsOpened() == false)
		{
			break;
		}
		wxString file_name;
		check_result = !target_dir.GetFirst(&file_name, wxEmptyString, wxDIR_FILES);
	} while (false);
	return check_result;
}

void DFRMainWindow::update_progress(void* wxGauge_obj_ptr, const int value_to_add)
{
	wxGauge* bar_obj_ptr = static_cast<wxGauge*>(wxGauge_obj_ptr);
	if(bar_obj_ptr->IsBeingDeleted() == false)
	{
		const int current_value = bar_obj_ptr->GetValue();
		const int max_range = bar_obj_ptr->GetRange();
		const int value_to_set = current_value + max_range;
		if (current_value + value_to_add > max_range)
		{
			bar_obj_ptr->SetValue(max_range);
		}
		else
		{
			bar_obj_ptr->SetValue(value_to_set);
		}
		
	}
	else
	{
		DBGOUT("wxGauge_obj_ptr is invalide....");
	}
	
}

void DFRMainWindow::on_work_dir_change(wxFileDirPickerEvent& event)
{
	work_dir_path = event.GetPath();
	if (wxDir::Exists(work_dir_path) == false)
	{
		DBGOUT("work_dir_path do not Exists");
		work_dir_path.Clear();
		wxMessageBox(wxT("工作路径不存在，请设置有效的路径"), wxT("无效的路径"), wxOK | wxICON_ERROR);
	}
	
}

void DFRMainWindow::on_output_dir_change(wxFileDirPickerEvent& event)
{
	output_path = event.GetPath();
	if (wxDir::Exists(output_path) == false)
	{
		DBGOUT("work_dir_path do not Exists");
		output_path.Clear();
		wxMessageBox(wxT("保存路径不存在，请设置有效的路径"), wxT("无效的路径"), wxOK | wxICON_ERROR);
	}
	if (is_empty_folder(output_path) == false)
	{
		wxMessageBox(wxT("该文件夹不为空，原有数据将会被删除，请提前备份以防止数据丢失"), wxT("警告"), wxOK | wxICON_WARNING);
	}
}

void DFRMainWindow::on_backup_dir_change(wxFileDirPickerEvent& event)
{
	backup_path = event.GetPath();
	if (wxDir::Exists(backup_path) == false)
	{
		DBGOUT("backup_path do not Exists");
		backup_path.Clear();
		wxMessageBox(wxT("备份路径不存在，请设置有效的路径"), wxT("无效的路径"), wxOK | wxICON_ERROR);
	}
	if (is_empty_folder(backup_path) == false)
	{
		wxMessageBox(wxT("该文件夹不为空，原有数据将会被删除，请提前备份以防止数据丢失"), wxT("警告"), wxOK | wxICON_WARNING);
	}
}

void DFRMainWindow::on_start_work_button_click(wxCommandEvent& event)
{
	work_progress_bar->SetValue(0);
	do
	{
		if (work_dir_path.empty() == true)
		{
			wxMessageBox(wxT("工作路径还未设置，请设置有效的路径"), wxT("无效的路径"), wxOK | wxICON_ERROR);
			break;
		}
		if (output_path.empty() == true)
		{
			wxMessageBox(wxT("保存路径还未设置，请设置有效的路径"), wxT("无效的路径"), wxOK | wxICON_ERROR);
			break;
		}
		if (backup_path.empty() == true)
		{
			wxMessageBox(wxT("备份路径还未设置，请设置有效的路径"), wxT("无效的路径"), wxOK | wxICON_ERROR);
			break;
		}
		if (is_empty_folder(backup_path) == false)
		{
			DBGOUT("backup_path is not empty, clear...");
			DuplicateFileRemover::clear_dir(backup_path.ToStdString());
		}
		DBGOUT("start work now.....");
		// 先备份
		if (start_backup(work_dir_path, backup_path) == false)
		{
			wxMessageBox(wxT("备份文件失败"), wxT("错误"), wxOK | wxICON_ERROR);
			break;
		}
		work_progress_bar->SetValue(50);
		DuplicateFileRemover my_remover(work_dir_path.ToStdString(), output_path.ToStdString());
		my_remover.start_work();
		work_progress_bar->SetValue(100);
		wxMessageBox(wxT("文件去重成功！！！！"), wxT("通知"), wxOK | wxICON_INFORMATION);
		// wxMessageBox用户点击ok之后，进度条重新归零
		work_progress_bar->SetValue(0);
	}
	while (false);
	// work_progress_bar->SetValue(100);
}

void DFRMainWindow::on_close_button_click(wxCommandEvent& event)
{
	Close();
}

bool DFRMainWindow::start_backup(const wxString& source, const wxString& destination)
{

	if (wxDir::Exists(source) == false)
	{
		DBGOUT("param source do not exists");
		return false;
	}

	if (wxDir::Exists(destination) == false)
	{
		if (wxMkdir(destination) == false)
		{
			DBGOUT("destination do not Exists and wxMkdir failed.");
			return false;
		}
		
	}

	wxString file_name;
	wxDir souce_dir_obj(source);
	bool ret_result = souce_dir_obj.GetFirst(&file_name, wxEmptyString, wxDIR_DIRS | wxDIR_FILES);
	while (ret_result)
	{
		wxString source_path = source + wxFileName::GetPathSeparator() + file_name;
		wxString destination_path = destination + wxFileName::GetPathSeparator() + file_name;

		//wxFileName current_file_name_obj(source_path);

		if (wxDir::Exists(source_path) == true)
		{
			// 复制子文件夹
			if (start_backup(source_path, destination_path) == false)
			{
				return false;
			}
		}
		else
		{
			// 复制文件
			if (wxCopyFile(source_path, destination_path) == false)
			{
				DBGOUT("wxCopyFile Failed.source_path:%s		destination_path:%s", source_path.ToStdString().c_str(), destination_path.ToStdString().c_str());
				return false;
			}
		}

		// 获取下一个文件/文件夹
		ret_result = souce_dir_obj.GetNext(&file_name);
	}

	return true;
}

bool DFRApp::OnInit()
{
	DFRMainWindow* app_window_ptr = new DFRMainWindow(nullptr);
	if (app_window_ptr == nullptr)
	{
		return false;
	}
	app_window_ptr->Show(true);
	return true;
}