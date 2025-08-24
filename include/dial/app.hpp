#pragma once

#include "drawable.hpp"
#include "singleton.hpp"

namespace dial
{	class app final : drawable, public singleton<app>
	{	public:
			void draw() override;
	};
}