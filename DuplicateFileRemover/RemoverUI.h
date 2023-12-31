#ifndef __RemoverUI__
#define __RemoverUI__

/**
@file
Subclass of MainFrame, which is generated by wxFormBuilder.
*/
#include <wx/wx.h>
#include "GeneratedRemoverUI.h"

//// end generated include

/** Implementing MainFrame */
class DFRMainWindow : public MainFrame
{
	public:
	/** Constructor */
	DFRMainWindow( wxWindow* parent );
	//// end generated class members
	static bool is_empty_folder(const wxString& target_folder_dir);
	static void update_progress(void* wxGauge_obj_ptr, const int value_to_add);
protected:
	void on_work_dir_change(wxFileDirPickerEvent& event) override;
	void on_output_dir_change(wxFileDirPickerEvent& event) override;
	void on_backup_dir_change(wxFileDirPickerEvent& event) override;
	void on_start_work_button_click(wxCommandEvent& event) override;
	void on_close_button_click(wxCommandEvent& event) override;

private:
	bool start_backup(const wxString& source, const wxString& destination);
	
	wxString work_dir_path;
	wxString output_path;
	wxString backup_path;

};

class DFRApp : public wxApp
{
public:
	virtual bool OnInit();
};

#endif // __RemoverUI__
