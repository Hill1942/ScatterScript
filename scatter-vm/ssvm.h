#define GET_POS_STACK_INDEX(index) \
	(index < 0 ? index += g_Script.stack.iFrameIndex : index)


void ResetScript();

int ValueToInt(Value value);
float ValueToFloat(Value value);
char* ValueToString(Value value);

int GetOpType(int opIndex);
Value GetOpValue(int opIndex);
Value* GetOpValuePointer(int opIndex);
int GetOpValueAsInt(int opIndex);
float GetOpValueAsFloat(int opIndex);
char* GetOpValueAsString(int opIndex);
int GetOpValueAsStackIndex(int opIndex);
int GetOpValueAsInstrIndex(int opIndex);
int GetOpValueAsFuncIndex(int opIndex);
char* GetOpValueASHostAPI(int opIndex);



