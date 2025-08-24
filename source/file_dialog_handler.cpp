#include "file_dialog_handler.hpp"
#include <optional>
#include <hard_assert.hpp>
#include <ImGuiFileDialog.h>

static bool running;
static std::string label, default_name;
static std::optional<std::string> extension;
static dial::file_dialog_handler::callback_t callback;

namespace dial::file_dialog_handler
{	bool is_running()
	{	return running;
	}

	static void open_dialog(std::string label_argument, callback_t callback_argument, std::optional<std::string> extension_argument, std::string default_name_argument)
	{	ha_assert(!running, MODULE_NAME, "file_dialog_handler::open_dialog called while already running.");
		label = label_argument;
		callback = callback_argument;
		extension = extension_argument;
		default_name = default_name_argument;
		running = true;
	}

	void open_file_dialog(std::string label, callback_t callback, std::string extension, std::string default_file_name)
	{	open_dialog(label, callback, extension, default_file_name);
	}

	void open_directory_dialog(std::string label, callback_t callback, std::string default_directory_name)
	{	open_dialog(label, callback, {}, default_directory_name);
	}

	void draw()
	{	ha_assert(running, MODULE_NAME, "file_dialog_handler::draw called while not running.");

		ImGuiFileDialog & dialog = *ImGuiFileDialog::Instance();

		std::string file_dialog_key = "FileSaveHandlerDialog";

		dialog.OpenDialog
		(	file_dialog_key,
			label,
			extension.has_value() ? extension->c_str() : nullptr,
			{	.fileName = default_name,
				.flags = ImGuiFileDialogFlags_Modal
			}
		);

		if
		(	dialog.IsOpened() &&
			dialog.Display(file_dialog_key)
		)
		{	if(dialog.IsOk())
			{	std::string path =
					extension.has_value()
						? dialog.GetFilePathName()
						: dialog.GetCurrentPath();
				callback(path);
			}
			else
				callback({});
			dialog.Close();
			running = false;
		}
	}
}