#define GET_STACK_INDEX(index) \
	(index < 0 ? index += g_Script.stack.iFrameIndex : index)

void Init();
void ShutDown();

int LoadScript(char* filename);
void ResetScript();
void RunScript();

void CopyValue(Value* dest, Value source);

int ValueToInt(Value value);
float ValueToFloat(Value value);
char* ValueToString(Value value);

Value GetStackValue(int index);
void SetStackValue(int index, Value value);

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

void Push(Value value);
Value Pop();
void PushFrame(int size);
void PopFrame(int size);

int GetTime();


