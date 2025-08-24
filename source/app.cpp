#include "app.hpp"
#include "imgui.h"
#include "forms_registry/forms_registry.hpp"

static void draw_form(dial::form_base * form)
{	form->draw();
	form->dial::form_base::draw();
}

namespace dial
{	void app::draw()
	{	forms_registry::instances_t forms = forms_registry::get().get_instances();
		if(forms.size() > 1)
		{	if(ImGui::BeginTabBar("Forms"))
			{	for(auto form : forms)
					if(ImGui::BeginTabItem(form->get_name().c_str()))
					{	draw_form(form);
						ImGui::EndTabItem();
					}
				ImGui::EndTabBar();
			}
		}
		else
			if(forms.empty())
				ImGui::Text("Welcome to DIAL! Create a static form instance to get started.");
			else
				draw_form(forms[0]);
	}
}
