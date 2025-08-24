#include <dial/form_base.hpp>

class text_reader : dial::form_base
{	private:
		std::string imported_text;
	public:
		std::string get_name() override { return "Text Reader"; }
		void draw() override;
};
