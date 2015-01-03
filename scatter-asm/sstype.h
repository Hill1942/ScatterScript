#ifndef SSPRE_H
#define SSPRE_H

#define TRUE  1
#define FALSE 0

typedef struct _LinkListNode
{
	void*          pData;
	_LinkListNode* pNext;

}LinkListNode;

typedef struct _LinkList
{
	LinkListNode* pHead;
	LinkListNode* pTail;
	int           iNodeCount;

}LinkList;

void InitLinkList(LinkList* pList);
void FreeLinkList(LinkList* pList);
int  AddNode(LinkList* pList, void *pData);
int  AddString(LinkList* pList, char* str);

#endif