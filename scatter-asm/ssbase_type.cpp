#include <stdlib.h>

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





















