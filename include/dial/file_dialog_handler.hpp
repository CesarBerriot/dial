#pragma once

#include <functional>
#include <string>
#include <optional>

namespace dial::file_dialog_handler
{	typedef std::function<void(std::optional<std::string> path)> callback_t;
	bool is_running();
	void open_file_dialog(std::string label, callback_t, std::string extension = ".txt", std::string default_file_name = "");
	void open_directory_dialog(std::string label, callback_t, std::string default_directory_name = "");
	void draw();
}