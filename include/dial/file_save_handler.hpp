#pragma once

#include <string>
#include "file_dialog_handler.hpp"

namespace dial::file_save_handler
{	enum class mode { over_write, append };
	void open_file_save_dialog
	(	mode,
		std::string label,
		std::string data,
		file_dialog_handler::callback_t = [](std::optional<std::string>){},
		std::string extension = ".txt",
		std::string default_file_name = ""
	);
	void save_to_file(mode, std::string path, std::string data);
}
