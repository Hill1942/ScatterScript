#ifndef SSFUNCS_H
#define SSFUNCS_H


namespace _vm
{
#define BUILT_IN_FUNC_DEFINE(ss_func_name) \
	void ss_func_name(Value* params, int paramCount)

	namespace io
	{
		BUILT_IN_FUNC_DEFINE(ssprint);
		BUILT_IN_FUNC_DEFINE(ssfopen);
		BUILT_IN_FUNC_DEFINE(ssfclose);
		BUILT_IN_FUNC_DEFINE(ssfgetc);
		BUILT_IN_FUNC_DEFINE(ssfgets);
		BUILT_IN_FUNC_DEFINE(ssfwrite);

	}

	


}


#endif