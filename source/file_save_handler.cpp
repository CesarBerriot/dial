#include "file_save_handler.hpp"
#include <filesystem>
#include <map>
#include <hard_assert.hpp>
#include <make_string.hpp>
#include <SDL3/SDL.h>

namespace dial::file_save_handler
{	void open_file_save_dialog
	(	mode mode,
		std::string label,
		std::string data,
		file_dialog_handler::callback_t callback,
		std::string extension,
		std::string default_file_name
	)
	{	file_dialog_handler::open_file_dialog
		(	label,
			[mode, data, callback](std::optional<std::string> path)
			{	if(path.has_value())
					save_to_file(mode, *path, data);
				callback(path);
			},
			extension,
			default_file_name
		);

	}

	void save_to_file(mode mode, std::string path, std::string data)
	{	std::map<enum mode, std::string> modes =
			{	{ mode::over_write, "w" },
				{ mode::append, "a" },
			};

		SDL_IOStream * stream = SDL_IOFromFile(path.c_str(), modes[mode].c_str());
		ha_assert(stream, MODULE_NAME, "SDL_IOFromFile failure.");

		if(SDL_WriteIO(stream, data.c_str(), data.size()) < data.size())
			ha_abort(MODULE_NAME, make_string("SDL_WriteIO failure with error : '%s'.", SDL_GetError()));

		ha_assert(SDL_CloseIO(stream), MODULE_NAME, "SDL_CloseIO failure.");
	}
}
