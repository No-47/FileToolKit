///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.10.1-0-g8feb16b3)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/filepicker.h>
#include <wx/sizer.h>
#include <wx/gauge.h>
#include <wx/button.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class MainFrame
///////////////////////////////////////////////////////////////////////////////
class MainFrame : public wxFrame
{
	private:

	protected:
		wxStaticText* input_label_text;
		wxDirPickerCtrl* work_dir_picker;
		wxStaticText* output_label_text;
		wxDirPickerCtrl* output_dir_picker;
		wxStaticText* backup_label_text;
		wxDirPickerCtrl* backup_dir_picker;
		wxGauge* work_progress_bar;
		wxButton* start_work_button;
		wxButton* close_button;

		// Virtual event handlers, override them in your derived class
		virtual void on_work_dir_change( wxFileDirPickerEvent& event ) { event.Skip(); }
		virtual void on_output_dir_change( wxFileDirPickerEvent& event ) { event.Skip(); }
		virtual void on_backup_dir_change( wxFileDirPickerEvent& event ) { event.Skip(); }
		virtual void on_start_work_button_click( wxCommandEvent& event ) { event.Skip(); }
		virtual void on_close_button_click( wxCommandEvent& event ) { event.Skip(); }


	public:

		MainFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("DuplicateFileRemover工具"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 343,203 ), long style = wxCAPTION|wxCLOSE_BOX|wxMINIMIZE_BOX|wxSYSTEM_MENU|wxTAB_TRAVERSAL );

		~MainFrame();

};

