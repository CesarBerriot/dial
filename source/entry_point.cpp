#include "entry_point.hpp"
#include <string>
#include <hard_assert.hpp>
#include <make_string.hpp>

typedef bool(*point_procedure_t)();

point_procedure_t
	dial_internal_entry_point = nullptr,
	dial_internal_exit_point = nullptr;

static void set(bool & called, std::string macro_name, point_procedure_t & point_pointer, point_procedure_t value)
{	ha_assert(!called, MODULE_NAME, make_string("%s used twice.", macro_name.c_str()));
	point_pointer = value;
	called = true;
}

void dial_internal_set_entry_point(point_procedure_t entry_point)
{	static bool called = false;
	set(called, "DIAL_ENTRY_POINT", dial_internal_entry_point, entry_point);
}

void dial_internal_set_exit_point(point_procedure_t exit_point)
{	static bool called = false;
	set(called, "DIAL_EXIT_POINT", dial_internal_exit_point, exit_point);
}