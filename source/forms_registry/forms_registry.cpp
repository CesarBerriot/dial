#include "forms_registry.hpp"
#include <hard_assert.hpp>
#include <make_string.hpp>

namespace dial
{	void forms_registry::perform_duplicate_checks()
	{	decltype(instances)::iterator found_duplicate =
			std::find_if
			(	instances.begin(),
				instances.end(),
				[this](form_base * first)
				{	return
						std::find_if
						(	instances.begin(),
							instances.end(),
							[this, first](form_base * second)
							{	return
									first != second &&
									first->get_name() == second->get_name();
							}
						) != instances.end();
				}
			);

		bool result = found_duplicate == instances.end();

		ha_assert
		(	result,
			MODULE_NAME,
			result ?
				"" :
				make_string
				(	"Multiple form instances registered using the name '%s'.",
					(*found_duplicate)->get_name().c_str()
				)
		);
	}
}
