#include <ntddk.h> 

#include "KBobMap.h"

//#include "KBobDebug.h"

// 数据存储格式：sizeof(BobMapNode) + 数据（长度为uDataLen）
typedef struct _BobMapNode
{
	LIST_ENTRY				next;

} BobMapNode, *PBobMapNode;

BOOLEAN IsValidMap(IN PBobMap pMap)
{
	if (NULL == pMap ||
		0 >= pMap->uDataLen ||
		NULL == pMap->pListArray ||
		0 >= pMap->uListArrayCount ||
		NULL == pMap->pIsEqual)
	{
		return FALSE;
	}		

	return TRUE;
}

unsigned long GetHashValue(IN PBobMap pMap, IN void* pKey)
{
	unsigned long v, ret = 0;

	if (NULL == pKey)
	{
		return ret;
	}

	if (pMap->bIsUlongKey)  // key为unsigned long
	{
		v = *(unsigned long*)pKey;
	}
	else // key为字符串
	{
		int r;
		long n = 0x100;

		if (pMap->bIsUnicode) // wchar_t
		{
			wchar_t* c = (wchar_t*)pKey;
			wchar_t cTemp = *c;
			while (cTemp)
			{
				if (L'A' <= cTemp && cTemp <= L'Z') // 转小写字符
				{
					cTemp += 32;
				}

				v = n | cTemp;
				n += 0x100;
				r = (int)((v >> 2) ^ v) & 0x0f;
				ret = (ret << r) | (ret >> (32 - r));
				ret &= 0xFFFFFFFFL;
				ret ^= v * v;
				c++;
				cTemp = *c;
			}
		}
		else // char
		{
			char* c = (char*)pKey;
			char cTemp = *c;
			while (cTemp)
			{
				if ('A' <= cTemp && cTemp <= 'Z')  // 转小写字符
				{
					cTemp += 32;
				}

				v = n | cTemp;
				n += 0x100;
				r = (int)((v >> 2) ^ v) & 0x0f;
				ret = (ret << r) | (ret >> (32 - r));
				ret &= 0xFFFFFFFFL;
				ret ^= v * v;
				c++;
				cTemp = *c;
			}
		}

		v = (ret >> 16) ^ ret;
	}

	return (v % pMap->uListArrayCount);
}

void* GetRealKey(IN PBobMap pMap, IN void* pKey)
{
	if (NULL != pMap->pGetRealKey)
	{
		return pMap->pGetRealKey(pKey);
	}

	return pKey;
}

PVOID GetMapRealDataAddr(IN PBobMapNode pNode)
{
	return (PVOID)((unsigned char*)pNode + sizeof(BobMapNode));
}

PBobMapNode AllocateBufForBobMap(IN PBobMap pMap)
{
	PBobMapNode pNode = NULL;

	if (pMap->bIsNPage)
		pNode = ExAllocateFromNPagedLookasideList(&pMap->struNPLookAside);
	else
		pNode = ExAllocateFromPagedLookasideList(&pMap->struPLookAside);

	if (NULL != pNode)
		RtlZeroMemory(pNode, sizeof(BobMapNode) + pMap->uDataLen);

	return pNode;
}

VOID FreeBobMapNode(IN PBobMap pMap, IN PBobMapNode pNode)
{
	if (pMap->bIsNPage)
		ExFreeToNPagedLookasideList(&pMap->struNPLookAside, pNode);
	else
		ExFreeToPagedLookasideList(&pMap->struPLookAside, pNode);
}

PLIST_ENTRY FindNodeInBobMapEx(IN PBobMap pMap, IN ULONG uHashValue, IN void* pKey)
{
	BOOLEAN bIsFind = FALSE;
	PLIST_ENTRY pEntry = NULL, pHead = &pMap->pListArray[uHashValue];

	if (!IsListEmpty(pHead))
	{
		PBobMapNode pNode = NULL;
		for (pEntry = pHead->Flink; pEntry != pHead; pEntry = pEntry->Flink)	// 遍历整链表来查找
		{
			pNode = CONTAINING_RECORD(pEntry, BobMapNode, next);
			if (NULL != pNode && pMap->pIsEqual(GetMapRealDataAddr(pNode), pKey))
			{
				bIsFind = TRUE;
				break;
			}
		}
	}

	if (!bIsFind)  // 未找到
	{
		pEntry = NULL;
	}

	return pEntry;
}

VOID UnInitBobMap( IN PBobMap pMap )
{
	if (!IsValidMap(pMap))
		return;

	ClearAllNodeOfBobMap(pMap);

	if (NULL != pMap->pListArray)
	{
		ExFreePool(pMap->pListArray);
		pMap->pListArray = NULL;
	}

	if (pMap->bIsNPage)
	{
		ExDeleteNPagedLookasideList(&pMap->struNPLookAside);
	}
	else
	{
		ExDeletePagedLookasideList(&pMap->struPLookAside);
	}
 
	pMap->uCurNodeNum = 0;
	pMap->uListArrayCount = 0;
	pMap->uDataLen = 0;
	pMap->bIsUlongKey = FALSE;
	pMap->bIsUnicode = FALSE;
	pMap->pIsEqual = NULL;	
}

BOOLEAN InitBobMap(IN PBobMap pMap, IN ULONG uListArrayCount,
		IN ULONG uDataLen, IN BOOLEAN bIsNPage, IN ULONG uTag, IN BOOLEAN bIsUlongKey,
		IN BOOLEAN bIsUnicode, IN _MapIsEqualNode pIsEqual, IN _MapGetRealKey pGetRealKey)
{
	BOOLEAN bRet = FALSE;
 
	do 
	{
		if( NULL == pMap )
			break;

		if( 0 >= uListArrayCount || 0 >= uDataLen || NULL == pIsEqual )
			break;

		pMap->pListArray = ExAllocatePool(NonPagedPool, uListArrayCount * sizeof( LIST_ENTRY ));
		if( NULL == pMap->pListArray )
			break;
 
		for( pMap->uListArrayCount = 0; pMap->uListArrayCount < uListArrayCount; pMap->uListArrayCount++ )
			InitializeListHead( &pMap->pListArray[ pMap->uListArrayCount ] );
		
		pMap->uCurNodeNum = 0;		
		pMap->uListArrayCount = uListArrayCount;
		pMap->uDataLen = uDataLen;
		pMap->bIsNPage = bIsNPage;
		pMap->uTag = uTag;
		pMap->bIsUlongKey = bIsUlongKey;
		pMap->bIsUnicode = bIsUnicode;
		pMap->pIsEqual = pIsEqual;
		pMap->pGetRealKey = pGetRealKey;

		if (pMap->bIsNPage)
		{
			ExInitializeNPagedLookasideList(&pMap->struNPLookAside, NULL, NULL,
				0, pMap->uDataLen + sizeof(BobMapNode), pMap->uTag, 0);
		}
		else
		{
			ExInitializePagedLookasideList(&pMap->struPLookAside, NULL, NULL,
				0, pMap->uDataLen + sizeof(BobMapNode), pMap->uTag, 0);
		}

		bRet = TRUE;

	} while ( 0 );

	return bRet;
}
 
VOID ClearAllNodeOfBobMap( IN PBobMap pMap )
{
	if (!IsValidMap(pMap))
		return;

	if( NULL != pMap->pListArray && pMap->uListArrayCount > 0 )
	{
		ULONG i;
		PLIST_ENTRY pHead = NULL;
		for( i = 0; i < pMap->uListArrayCount; i++ )
		{
			pHead = &pMap->pListArray[i];
			while (!IsListEmpty(pHead))
			{
				FreeBobMapNode(pMap, CONTAINING_RECORD(RemoveHeadList(pHead), BobMapNode, next));
			}
		}		
	}

	pMap->uCurNodeNum = 0;
}

ULONG GetNodeNumInBobMap(IN PBobMap pMap)
{
	if (!IsValidMap(pMap))
		return 0;

	return pMap->uCurNodeNum;
}

BOOLEAN FindNodeInBobMap(IN PBobMap pMap, IN void* pKey, OUT PVOID pGetData/* = NULL*/)
{
	BOOLEAN bRet = FALSE;
	void* pRealKey = NULL;
	PLIST_ENTRY pEntry = NULL;

	do
	{
		if (!IsValidMap(pMap))
			break;

		if (NULL == pKey)
			break;

		pRealKey = GetRealKey(pMap, pKey);
		if (NULL == pRealKey)
		{
			break;
		}

		pEntry = FindNodeInBobMapEx(pMap, GetHashValue(pMap, pRealKey), pKey);
		bRet = (NULL != pEntry);

		if (bRet && NULL != pGetData)
		{
			RtlCopyMemory(pGetData, GetMapRealDataAddr(CONTAINING_RECORD(pEntry, BobMapNode, next)), pMap->uDataLen);
		}

	} while (0);

	return bRet;
}

BOOLEAN AddNodeToBobMap(IN PBobMap pMap, IN void* pKey, IN PVOID pSrcData)
{
	BOOLEAN bRet = FALSE;
	ULONG uHashValue = 0;
	void* pRealKey = NULL;

	do
	{
		if (!IsValidMap(pMap))
		{
			//BobOutputError("AddNodeToBobMap(), IsValidMap() failed!");
			break;
		}

		if (NULL == pKey || NULL == pSrcData)
		{
			//BobOutputError("AddNodeToBobMap(), NULL == pKey || NULL == pSrcData");
			break;
		}

		pRealKey = GetRealKey(pMap, pKey);
		if (NULL == pRealKey)
		{
			break;
		}
		
		uHashValue = GetHashValue(pMap, pRealKey);
		if (NULL == FindNodeInBobMapEx(pMap, uHashValue, pKey))
		{
			PBobMapNode pNode = AllocateBufForBobMap(pMap);
			if (NULL != pNode)
			{
				RtlCopyMemory(GetMapRealDataAddr(pNode), pSrcData, pMap->uDataLen);
				InsertTailList(&pMap->pListArray[uHashValue], &pNode->next);
				pMap->uCurNodeNum++;

				bRet = TRUE;
			}
			else
			{
				//BobOutputError("AddNodeToBobMap(), AllocateBufForBobMap() failed! uHashValue:%d", uHashValue);
			}
		}
		else
		{
			//BobOutputError("AddNodeToBobMap(), node exist! uHashValue:%d", uHashValue);
		}

	} while (0);

	return bRet;
}

BOOLEAN DeleteNodeFromBobMap(IN PBobMap pMap, IN void* pKey, OUT PVOID pGetData)
{
	BOOLEAN bRet = FALSE;
	void* pRealKey = NULL;
	PLIST_ENTRY pEntry = NULL;

	do
	{
		if (!IsValidMap(pMap))
		{
			//BobOutputError("DeleteNodeFromBobMap(), IsValidMap() failed!");
			break;
		}

		if (NULL == pKey)
		{
			//BobOutputError("DeleteNodeFromBobMap(), NULL == pKey");
			break;
		}

		pRealKey = GetRealKey(pMap, pKey);
		if (NULL == pRealKey)
		{
			break;
		}

		pEntry = FindNodeInBobMapEx(pMap, GetHashValue(pMap, pRealKey), pKey);
		if (NULL != pEntry)
		{
			PBobMapNode pNode = CONTAINING_RECORD(pEntry, BobMapNode, next);		
			if (NULL != pGetData)
			{
				RtlCopyMemory(pGetData, GetMapRealDataAddr(pNode), pMap->uDataLen);
			}

			RemoveEntryList(pEntry); // 擦除节点
			FreeBobMapNode(pMap, pNode); // 释放节点数据
			pMap->uCurNodeNum--;
		}
		else
		{
			//BobOutputError("DeleteNodeFromBobMap(), can't find key!");
		}

		bRet = TRUE;

	} while (0);

	return bRet;
}

PVOID GetAddrOfNode(IN PBobMap pMap, IN void* pKey)
{
	PVOID pRet = NULL;
	void* pRealKey = NULL;
	PLIST_ENTRY pEntry = NULL;

	do
	{
		if (!IsValidMap(pMap))
			break;

		if (NULL == pKey)
			break;

		pRealKey = GetRealKey(pMap, pKey);
		if (NULL == pRealKey)
		{
			break;
		}

		pEntry = FindNodeInBobMapEx(pMap, GetHashValue(pMap, pRealKey), pKey);
		if (NULL != pEntry)
		{
			pRet = GetMapRealDataAddr(CONTAINING_RECORD(pEntry, BobMapNode, next));
		}

	} while (0);

	return pRet;
}

VOID EnumAllNodeOfBobMap(IN PBobMap pMap, IN _EnumMapNode pEnumFun, IN PVOID pForeign, IN PVOID pParam)
{
	if (!IsValidMap(pMap))
		return;

	if (NULL == pEnumFun)
		return;

	if (NULL != pMap->pListArray && pMap->uListArrayCount > 0)
	{
		ULONG i;
		for (i = 0; i < pMap->uListArrayCount; i++)
		{
			PLIST_ENTRY pEntry = NULL;
			for (pEntry = pMap->pListArray[i].Flink; pEntry != (&pMap->pListArray[i]); pEntry = pEntry->Flink)	// 遍历整个链表来查找
			{
				if (pEnumFun(GetMapRealDataAddr(CONTAINING_RECORD(pEntry, BobMapNode, next)), pForeign, pParam))
				{
					break; // 结束枚举
				}
			}
		}
	}
}