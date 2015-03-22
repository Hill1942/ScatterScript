#include <stdlib.h>

#include "ssbase_type.h"

void InitLinkList(LinkList* pList)
{
    pList->pHead = NULL;
    pList->pTail = NULL;
	pList->iNodeCount = 0;
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

        while (1)
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
int AddNode(LinkList* pList, void *pData)
{
	LinkListNode* pNewNode = (LinkListNode*) malloc(sizeof(LinkListNode));
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
		pList->pTail = pNewNode;
	}
	pList->iNodeCount++;

	return pList->iNodeCount - 1;
}
void DeleteNode(LinkList* pList, LinkListNode* pNode)
{
	if (pList->iNodeCount == 0)
		return;

	if (pNode == pList->pHead)
	{
		pList->pHead = pNode->pNext;
	}
	else
	{
		LinkListNode* pThroughNode = pList->pHead;
		for (int i = 0; i < pList->iNodeCount; i++)
		{
			if (pThroughNode->pNext == pNode)
			{
				if (pList->pTail == pNode)
				{
					pThroughNode->pNext = NULL;
					pList->pTail = pThroughNode;
				}
				else
				{
					pThroughNode->pNext = pNode->pNext;
				}
				break;
			}
			pThroughNode = pThroughNode->pNext;
		}
	}

	pList->iNodeCount--;
	if (pNode->pData)
		free(pNode->pData);
	free(pNode);
}

void InitStack(Stack* pStack)
{
	InitLinkList(&pStack->elementList);
}
void FreeStack(Stack* pStack)
{
	FreeLinkList(&pStack->elementList);
}
int IsStackEmpty(Stack* pStack)
{
	if (pStack->elementList.iNodeCount > 0)
		return FALSE;
	else
		return TRUE;
}
void Push(Stack* pStack, void* pData)
{
	AddNode(&pStack->elementList, pData);
}
void Pop(Stack* pStack)
{
	DeleteNode(&pStack->elementList, pStack->elementList.pTail);
}
void* Peek(Stack* pStack)
{
	return pStack->elementList.pTail->pData;
}























