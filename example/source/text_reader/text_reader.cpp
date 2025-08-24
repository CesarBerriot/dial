#include "text_reader.hpp"
#include <filesystem>
#include <fstream>
#include <imgui.h>
#include <make_string.hpp>
#include <dial/file_dialog_handler.hpp>

static text_reader instance;

void text_reader::draw()
{	if(ImGui::Button("Import Text"))
		dial::file_dialog_handler::open_file_dialog
		(	"Text File",
			[this](std::optional<std::string> path)
			{	if(!path)
					return;
				if(!std::filesystem::exists(*path))
					log(log_level::WARNING, make_string("No such file as '%s'.", path->c_str()));
				else
				{	std::ifstream stream(*path);
					if(stream.is_open())
						imported_text = (std::stringstream() << stream.rdbuf()).str();
					else
						log(log_level::WARNING, make_string("Failed to read file '%s'.", path->c_str()));
				}
			}
		);
	ImGui::Text("Imported Text :");
	if(ImGui::BeginChild("Imported Text", {}, ImGuiChildFlags_Borders | ImGuiChildFlags_AutoResizeY))
	{	ImGui::TextUnformatted(imported_text.c_str());
		ImGui::EndChild();
	}
}
