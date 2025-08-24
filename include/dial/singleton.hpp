#pragma once

namespace dial
{	template<class derived_class> class singleton
	{	public:
			static derived_class & get()
			{	static derived_class instance;
				return instance;
			}
			virtual ~singleton() {}
	};
}
