void ResetScript();

int ValueToInt(Value value);
float ValueToFloat(Value value);
char* ValueToString(Value value);

int GetOpType(int opIndex);
Value GetOpValue(int opIndex);
int GetOpValueAsInt(int opIndex);
float GetOpValueAsFloat(int opIndex);
char* GetOpValueAsString(int opIndex);
int GetOpValueAsStackIndex(int opIndex);
int GetOpValueAsInstrIndex(int opIndex);
int GetOpValueAsFuncIndex(int opIndex);
char* GetOpValueASHostAPI(int opIndex);

