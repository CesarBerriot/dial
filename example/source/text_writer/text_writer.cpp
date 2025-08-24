#include "text_writer.hpp"
#include <filesystem>
#include <fstream>
#include <imgui.h>
#include <make_string.hpp>
#include <dial/file_dialog_handler.hpp>

#include "dial/file_save_handler.hpp"

static text_writer instance;

void text_writer::draw()
{	if(ImGui::Button("Export Text"))
		dial::file_save_handler::open_file_save_dialog
		(	dial::file_save_handler::mode::over_write,
			"Text File",
			exported_text
		);
	ImGui::Text("Exported Text :");
	ImGui::InputTextMultiline("##Exported Text", exported_text, exported_text_buffer_size);
}
