#ifndef SSBASE_TYPE_H
#define SSBASE_TYPE_H

#define TRUE  1
#define FALSE 0

struct LinkListNode
{
    void*          pData;
    LinkListNode* pNext;
};
struct LinkList
{
    LinkListNode* pHead;
    LinkListNode* pTail;
    int           iNodeCount;
};
void InitLinkList(LinkList* pList);
void FreeLinkList(LinkList* pList);
int  AddNode(LinkList* pList, void *pData);
void DeleteNode(LinkList* pList, LinkListNode* pNode);


struct Stack
{
	LinkList elementList;
};
void  InitStack(Stack* pStack);
void  FreeStack(Stack* pStack);
int   IsStackEmpty(Stack* pStack);
void  Push(Stack* pStack, void* pData);
void  Pop(Stack* pStack);
void* Peek(Stack* pStack);



#endif