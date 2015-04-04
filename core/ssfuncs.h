#ifndef SSFUNCS_H
#define SSFUNCS_H


namespace _vm
{
#define DEFINE_BUILT_IN_FUNC(ss_func_name) \
	void ss_func_name(Value* params, int paramCount)

	DEFINE_BUILT_IN_FUNC(ssprint);
	DEFINE_BUILT_IN_FUNC(ssfopen);
	DEFINE_BUILT_IN_FUNC(ssgetc);


}


#endif