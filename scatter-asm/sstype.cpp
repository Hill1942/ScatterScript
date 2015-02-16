#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ssbase_type.h"


void InitLinkList(LinkList* pList)
{
	pList->pHead = NULL;
	pList->pTail = NULL;
}

void FreeLinkList(LinkList* pList)
{
	if (!pList)
		return;

	if (pList->iNodeCount) 
	{
		LinkListNode* pCurrentNode;
		LinkListNode* pNextNode;

		pCurrentNode = pList->pHead;

		while (TRUE) 
		{
			pNextNode = pCurrentNode->pNext;

			//free the data of current node
			if (pCurrentNode->pData) 
				free(pCurrentNode->pData);

			//free the current node
			if (pCurrentNode)
				free(pCurrentNode);

			//if pNextNode is NULL, then the list is at the end. If not, set it to currnet node
			if (pNextNode)
				pCurrentNode = pNextNode;
			else
				break;
		}
	}
}

int  AddNode(LinkList* pList, void* pData)
{
	LinkListNode* pNewNode = (LinkListNode*)malloc(sizeof(LinkListNode));

	pNewNode->pData = pData;
	pNewNode->pNext = NULL;

	if (!pList->iNodeCount)
	{
		pList->pHead = pNewNode;
		pList->pTail = pNewNode;
	}
	else
	{
		pList->pTail->pNext = pNewNode;
		pList->pTail        = pNewNode;
	}
	pList->iNodeCount++;

	//return NewNode's index
	return pList->iNodeCount - 1;
}

int  AddString(LinkList* pList, char* str)
{
	LinkListNode* pNode = pList->pHead;
	for (int i = 0; i < pList->iNodeCount; i++)
	{
		if (strcmp((char*)pNode->pData, str) == 0)
			return i;
		pNode = pNode->pNext;
	}
	char* newStr =(char*) malloc(strlen(str) + 1);
	strcpy(newStr, str);

	return AddNode(pList, newStr);
}



















