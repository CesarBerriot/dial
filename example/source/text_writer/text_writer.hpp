#include <dial/form_base.hpp>

class text_writer : dial::form_base
{	private:
		enum
		{	exported_text_max_characters = 500,
			exported_text_buffer_size = exported_text_max_characters + 1
		};
		char exported_text[exported_text_buffer_size];
	public:
		std::string get_name() override { return "Text Writer"; }
		void draw() override;
};
