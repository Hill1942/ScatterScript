#ifndef SSVM_H
#define SSVM_H

namespace _vm
{
	#define GET_STACK_INDEX(index) \
	(index < 0 ? index += vm_script.stack.iFrameIndex : index)


	int LoadScript(char* filename);
    void ResetScript();
    void RunScript();
    
    void CopyValue(Value* dest, Value source);
    
    int ValueToInt(Value value);
    float ValueToFloat(Value value);
    char* ValueToString(Value value);
    
    Value GetStackValue(int index);
    void SetStackValue(int index, Value value);
    
    int GetOpTypeFromStack(int opIndex);
    
    int GetOpTypeFromInstr(int opIndex);
    
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
    
}

#endif





