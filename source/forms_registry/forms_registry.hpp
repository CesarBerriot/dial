#pragma once

#include <vector>
#include "form_base.hpp"
#include "singleton.hpp"

namespace dial
{	class forms_registry final : public singleton<forms_registry>
	{	public:
			typedef std::vector<form_base*> instances_t;
		private:
			instances_t instances;
		public:
			void register_instance(form_base * instance) { instances.push_back(instance); }
			void perform_duplicate_checks();
			instances_t const & get_instances() { return instances; }
	};
}
