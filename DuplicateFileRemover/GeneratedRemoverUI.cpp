///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.10.1-0-g8feb16b3)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "GeneratedRemoverUI.h"

///////////////////////////////////////////////////////////////////////////

MainFrame::MainFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* main_box_sizer;
	main_box_sizer = new wxBoxSizer( wxVERTICAL );

	wxFlexGridSizer* top_fg_sizer;
	top_fg_sizer = new wxFlexGridSizer( 0, 2, 0, 0 );
	top_fg_sizer->SetFlexibleDirection( wxBOTH );
	top_fg_sizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	input_label_text = new wxStaticText( this, wxID_ANY, wxT("重复文件路径:"), wxDefaultPosition, wxDefaultSize, 0 );
	input_label_text->Wrap( -1 );
	top_fg_sizer->Add( input_label_text, 1, wxALL|wxEXPAND, 5 );

	work_dir_picker = new wxDirPickerCtrl( this, wxID_ANY, wxEmptyString, wxT("查找文件夹中的重复文件"), wxDefaultPosition, wxDefaultSize, wxDIRP_DEFAULT_STYLE );
	top_fg_sizer->Add( work_dir_picker, 0, wxALL, 5 );

	output_label_text = new wxStaticText( this, wxID_ANY, wxT("另存为:"), wxDefaultPosition, wxDefaultSize, 0 );
	output_label_text->Wrap( -1 );
	top_fg_sizer->Add( output_label_text, 1, wxALL|wxEXPAND, 5 );

	output_dir_picker = new wxDirPickerCtrl( this, wxID_ANY, wxEmptyString, wxT("去重后的文件将保存在此"), wxDefaultPosition, wxDefaultSize, wxDIRP_DEFAULT_STYLE );
	top_fg_sizer->Add( output_dir_picker, 0, wxALL|wxEXPAND, 5 );

	backup_label_text = new wxStaticText( this, wxID_ANY, wxT("备份路径: "), wxDefaultPosition, wxDefaultSize, 0 );
	backup_label_text->Wrap( -1 );
	top_fg_sizer->Add( backup_label_text, 1, wxALL|wxEXPAND, 5 );

	backup_dir_picker = new wxDirPickerCtrl( this, wxID_ANY, wxEmptyString, wxT("原文件夹会备份在此"), wxDefaultPosition, wxDefaultSize, wxDIRP_DEFAULT_STYLE );
	top_fg_sizer->Add( backup_dir_picker, 0, wxALL|wxEXPAND, 5 );


	main_box_sizer->Add( top_fg_sizer, 1, wxEXPAND, 5 );

	work_progress_bar = new wxGauge( this, wxID_ANY, 100, wxDefaultPosition, wxDefaultSize, wxGA_HORIZONTAL );
	work_progress_bar->SetValue( 0 );
	main_box_sizer->Add( work_progress_bar, 0, wxALL|wxEXPAND, 5 );

	wxGridSizer* bottom_gsizer;
	bottom_gsizer = new wxGridSizer( 0, 2, 0, 0 );

	start_work_button = new wxButton( this, wxID_ANY, wxT("开始"), wxDefaultPosition, wxDefaultSize, 0 );
	bottom_gsizer->Add( start_work_button, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );

	close_button = new wxButton( this, wxID_ANY, wxT("关闭"), wxDefaultPosition, wxDefaultSize, 0 );
	bottom_gsizer->Add( close_button, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );


	main_box_sizer->Add( bottom_gsizer, 1, wxEXPAND, 5 );


	this->SetSizer( main_box_sizer );
	this->Layout();

	this->Centre( wxBOTH );

	// Connect Events
	work_dir_picker->Connect( wxEVT_COMMAND_DIRPICKER_CHANGED, wxFileDirPickerEventHandler( MainFrame::on_work_dir_change ), NULL, this );
	output_dir_picker->Connect( wxEVT_COMMAND_DIRPICKER_CHANGED, wxFileDirPickerEventHandler( MainFrame::on_output_dir_change ), NULL, this );
	backup_dir_picker->Connect( wxEVT_COMMAND_DIRPICKER_CHANGED, wxFileDirPickerEventHandler( MainFrame::on_backup_dir_change ), NULL, this );
	start_work_button->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::on_start_work_button_click ), NULL, this );
	close_button->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::on_close_button_click ), NULL, this );
}

MainFrame::~MainFrame()
{
	// Disconnect Events
	work_dir_picker->Disconnect( wxEVT_COMMAND_DIRPICKER_CHANGED, wxFileDirPickerEventHandler( MainFrame::on_work_dir_change ), NULL, this );
	output_dir_picker->Disconnect( wxEVT_COMMAND_DIRPICKER_CHANGED, wxFileDirPickerEventHandler( MainFrame::on_output_dir_change ), NULL, this );
	backup_dir_picker->Disconnect( wxEVT_COMMAND_DIRPICKER_CHANGED, wxFileDirPickerEventHandler( MainFrame::on_backup_dir_change ), NULL, this );
	start_work_button->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::on_start_work_button_click ), NULL, this );
	close_button->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::on_close_button_click ), NULL, this );

}
