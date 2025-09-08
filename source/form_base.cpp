#include "form_base.hpp"
#include <imgui.h>
#include <SDL3/SDL.h>
#include <hard_assert.hpp>
#include <make_string.hpp>
#include <ImGuiFileDialog.h>
#include <ctime>
#include "file_dialog_handler.hpp"
#include "file_save_handler.hpp"
#include "forms_registry/forms_registry.hpp"

namespace dial
{	form_base::form_base()
	{	for(int i = 0; i < log_level::MAX; ++i)
			log_level_visibilities[i] = true;
		forms_registry::get().register_instance(this);
	}

	void form_base::draw()
	{	if(file_dialog_handler::is_running())
			file_dialog_handler::draw();
		draw_contents();
		spacer();
		process_log_queue();
		draw_log_action_bar();
		draw_log_content();
	}

	void form_base::log(log_message message)
	{	std::string formatted_message =
			message.message.empty()
				?	message.message
				:	make_string
					(	"[%s] [%s] %s",
						compute_time_stamp().c_str(),
						log_level::to_string(message.level).c_str(),
						message.message.c_str()
					);

		if(!log_data.empty() && log_data.back().level == message.level)
		{	if(log_data.back().message.back() != '\n')
				log_data.back().message += '\n';
			log_data.back().message += formatted_message;
		}
		else
			log_data.push_back
			(	{	.level = message.level,
					.message = formatted_message
				}
			);

		if(auto_scroll_log && log_level_visibilities[message.level])
			apply_auto_scroll_next_frame = true;
	}

	std::string form_base::compute_full_log()
	{	std::string result;
		for(log_message message : log_data)
		{	if(!result.empty() && result.back() != '\n')
			result += '\n';
			result += message.message;
		}
		return result;
	}

	void form_base::copy_log_to_clipboard()
	{	if(!SDL_SetClipboardText(compute_full_log().c_str()))
			ha_abort
			(	MODULE_NAME,
				make_string
				(	"SDL_SetClipboardText failed with error : '%s'",
					SDL_GetError()
				)
			);
	}

	std::string form_base::compute_time_stamp()
	{	time_t unix_time = time(NULL);
		ha_assert(unix_time != -1, MODULE_NAME, "libc time() failure.");

		tm * time = localtime(&unix_time);
		ha_assert(time, MODULE_NAME, "localtime failure.");

		enum { time_stamp_buffer_size = 30 };
		char time_stamp_buffer[time_stamp_buffer_size];

		ha_assert
		(	strftime(time_stamp_buffer, time_stamp_buffer_size, "%b %d %Y %I-%M-%S %p", time),
			MODULE_NAME,
			"strftime failure."
		);

		return time_stamp_buffer;
	}

	void form_base::draw_log_action_bar()
	{	ImGui::Text("Log :\n");
		ImGui::SameLine();
		draw_log_buttons();
		ImGui::SameLine();
		ImGui::Checkbox("Auto Scroll", &auto_scroll_log);
		for(int i = 0; i < log_level::MAX; ++i)
		{	ImGui::SameLine();
			ImGui::Checkbox
			(	make_string
				(	"%s Messages",
					log_level::to_string((log_level::type)i).c_str()
				),
				&log_level_visibilities[i]
			);
		}
	}

	void form_base::draw_log_buttons()
	{	struct
		{	std::string label;
			std::function<void()> callback;
		} buttons[] =
		{	{ "Clear", [this]{ log_data.clear(); } },
			{ "Copy", std::bind(&form_base::copy_log_to_clipboard, this) },
			{	"Save To File",
				[this]
				{
					file_save_handler::open_file_save_dialog
					(	file_save_handler::mode::over_write,
						"Log Save Location",
						compute_full_log(),
						[this](std::optional<std::string> path)
						{	if(path.has_value())
								log(log_level::INFO, make_string("Log written to path : '%s'", path->c_str()));
							else
								log(log_level::INFO, "Log save canceled.");
						},
						".txt",
						compute_time_stamp()
					);
				}
			}
		};

		for(int i = 0; i < std::size(buttons); ++i)
		{	if(i)
				ImGui::SameLine();
			if(ImGui::Button(buttons[i].label.c_str()))
				buttons[i].callback();
		}
	}

	void form_base::draw_log_content()
	{	ImGui::BeginChild("Log", {}, ImGuiChildFlags_Borders);
		std::map<log_level::type, ImVec4> colors =
		{	{ log_level::INFO, { 1, 1, 1, 1 } },
			{ log_level::WARNING, { 1, 1, 0, 1 } }
		};
		for(log_message message : log_data)
			if(log_level_visibilities[message.level])
				ImGui::TextColored(colors[message.level], "%s", message.message.c_str());
		if(apply_auto_scroll_next_frame)
		{	ImGui::SetScrollHereY(1);
			apply_auto_scroll_next_frame = false;
		}
		ImGui::EndChild();
	}

	void form_base::process_log_queue()
	{	log_queue_mutex.lock();
		for(log_message message : log_queue)
			log(message);
		log_queue.clear();
		log_queue_mutex.unlock();
	}

	void form_base::queue_log_message(log_message message)
	{	log_queue_mutex.lock();
		log_queue.push_back(message);
		log_queue_mutex.unlock();
	}

	void form_base::spacer()
	{	ImGui::Dummy(ImVec2(0.0f, 20.0f));
	}

	void form_base::text_input(char label[], char_buffer_t buffer)
	{	ImGui::InputText(label, buffer, char_buffer_size);
	}

	void form_base::optional_text_input(char label[], optional_char_buffer & optional_char_buffer)
	{	ImGui::Checkbox(make_string("##%s_toggle", label), &optional_char_buffer.enabled);
		ImGui::SameLine();
		if(!optional_char_buffer.enabled)
			ImGui::BeginDisabled();
		ImGui::InputText(label, optional_char_buffer.char_buffer, char_buffer_size);
		if(!optional_char_buffer.enabled)
			ImGui::EndDisabled();
	}

	void form_base::category(char label[], std::function<void()> content)
	{	ImGui::PushID(label);
		ImGui::Text(label);
		content();
		spacer();
		ImGui::PopID();
	}

	bool form_base::is_running_asynchronous_task()
	{	if(!running_asynchronous_task && asynchronous_task_thread)
		{	asynchronous_task_thread->join();
			asynchronous_task_thread.reset();
		}
		return running_asynchronous_task;
	}

	bool form_base::is_asynchronous_task_thread()
	{	return std::this_thread::get_id() == asynchronous_task_thread->get_id();
	}

	void form_base::run_asynchronous_task(std::function<void()> behavior)
	{	ha_assert(!running_asynchronous_task, MODULE_NAME, "base::run_asynchronous_task called while the asynchronous task was already running.");
		running_asynchronous_task = true;
		asynchronous_task_thread =
			std::make_unique<std::thread>
			(	[this, behavior]
				{	behavior();
					running_asynchronous_task = false;
				}
			);
	}
}
