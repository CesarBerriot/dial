#pragma once

#include <functional>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <mutex>
#include <memory>
#include <thread>
#include "drawable.hpp"

namespace dial
{	class form_base : drawable
	{	protected:
			struct log_level
			{	enum type
				{	INFO,
					WARNING,
					MAX
				};

				static std::string to_string(type value)
				{	return
						std::map<type, std::string>
						{	{ INFO, "Info" },
							{ WARNING, "Warning" }
						}[value];
				}
			};
		private:
			struct log_message
			{	log_level::type level;
				std::string message;
			};
		protected:
			enum { char_buffer_size = 150 };
			typedef char char_buffer_t[char_buffer_size];
			typedef char * char_buffer_pointer_t;
			struct optional_char_buffer
			{	bool enabled;
				char_buffer_t char_buffer;
			};
		private:
			std::vector<log_message> log_data;
			bool
				auto_scroll_log = true,
				apply_auto_scroll_next_frame,
				running_asynchronous_task = false;
				std::list<log_message> log_queue;
				std::mutex log_queue_mutex;
				std::unique_ptr<std::thread> asynchronous_task_thread;
				bool log_level_visibilities[log_level::MAX];
		private:
			void draw() final;
			void log(log_message);
			std::string compute_full_log();
			void copy_log_to_clipboard();
			std::string compute_time_stamp();
			void draw_log_action_bar();
			void draw_log_buttons();
			void draw_log_content();
			void process_log_queue();
			void queue_log_message(log_message);
		protected:
			virtual void draw_contents() = 0;
			virtual bool should_draw_log() { return true; }
			void log(log_level::type level, std::string message) { log({ level, message }); }
			void spacer();
			void text_input(char label[], char_buffer_t);
			void optional_text_input(char label[], optional_char_buffer&);
			void category(char label[], std::function<void()> content);
			bool is_running_asynchronous_task();
			bool is_asynchronous_task_thread();
			void run_asynchronous_task(std::function<void()> behavior);
			void queue_log_message(log_level::type level, std::string message) { queue_log_message({ level, message }); }
		public:
			form_base();
			~form_base() override {}
			virtual std::string get_name() = 0;
	};
}
