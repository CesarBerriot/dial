#pragma once

#include <pre_main_runner.hpp>

/**
 * Usage :
 * DIAL_ENTRY_POINT
 * (	FUNCTION_POINTER_HERE
 * );
 *
 * DIAL_ENTRY_POINT
 * (	[]{ return true; }
 * );
 */

#define DIAL_ENTRY_POINT(...)							\
	RUN_PRE_MAIN										\
	(	void dial_internal_set_entry_point(bool(*)());	\
		dial_internal_set_entry_point(__VA_ARGS__);		\
	)

#define DIAL_EXIT_POINT(...)							\
	RUN_PRE_MAIN										\
	(	void dial_internal_set_exit_point(bool(*)());	\
		dial_internal_set_exit_point(__VA_ARGS__);		\
	)
