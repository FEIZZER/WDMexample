
#pragma once

/*********************************************************************/
// writed by bob													 //
// 在同步数据时，高irql/不能睡眠情况下用非分页内存和KSPIN_LOCK，			 //
//			    低irql/需要睡眠情况下用分页内存和FAST_MUTEX。  		 //
/*********************************************************************/


/**************************************************************/
// 判断节点是否相等
// 参数:
//		pNodeInMap[IN]-map里的数据节点
//		pForeignKey[IN]-外部传入的key
// 返回值: TRUE-相等，FALSE-不相等。
/**************************************************************/
typedef BOOLEAN (*_MapIsEqualNode)( IN PVOID pNodeInMap, IN void* pForeignKey );

/**************************************************************/
// 获取真正的key数据（在key中有额外数据时使用）
// 参数:
//		pForeignKey[IN]-外部传入的key
// 返回值: 真正的key数据。
/**************************************************************/
typedef void* (*_MapGetRealKey)(IN void* pForeignKey);

/**************************************************************/
// 枚举节点回调函数
// 参数:
//		pNodeInMap[IN]-map里的数据节点
//		pForeign[IN]-外部传入的数据
//		pParam[IN]-外部传入的参数
// 返回值: 是否退出枚举：TRUE-退出，FALSE-不退出。
/**************************************************************/
typedef BOOLEAN(*_EnumMapNode)(IN PVOID pNodeInMap, IN PVOID pForeign, IN PVOID pParam);

// map数据。注意：调用者不能修改这个结构体里的所有成员数据。
typedef struct _BobMap
{
	NPAGED_LOOKASIDE_LIST	struNPLookAside;
	PAGED_LOOKASIDE_LIST	struPLookAside;
	BOOLEAN 					bIsNPage;						// 节点的内存属性。  TRUE-非分页内存(物理内存)  FALSE-分页内存(虚拟内存)

	ULONG					uCurNodeNum;					// map中已有节点个数

	PLIST_ENTRY				pListArray;						// 链表数组指针	
	ULONG					uListArrayCount;				// pListArray中元素个数

	ULONG					uTag;							// 同ExAllocatePoolWithTag的Tag
	ULONG					uDataLen;						// 需要保存在map中的每个数据节点的大小。单位：字节

	BOOLEAN 					bIsUlongKey;					// key是否为一个ULONG。	TRUE-key为ULONG		FALSE-key为字符串
	BOOLEAN 					bIsUnicode;						// 当key为字符串时，字符编码模式。	TRUE-WCHAR  FALSE-char
	
	_MapIsEqualNode			pIsEqual;						// 回调函数指针。用于判断两个节点是否相等。
	_MapGetRealKey			pGetRealKey;					// 获取真正的key数据，在key中有额外数据时使用。

} BobMap, *PBobMap;
 

/**************************************************************/
// 初始化map
// 参数:
//		pMap[IN]-map数据
//		uListArrayCount[IN]-pListArray中元素个数
//		uDataLen[IN]-数据节点的大小。单位：字节
//		bIsNPage[IN]-数据节点的内存属性。  TRUE-非分页内存  FALSE-分页内存
//		uTag[IN]-节点内存标识，同ExAllocatePoolWithTag的Tag
//		bIsUlongKey[IN]-key是否为一个ULONG。	TRUE-key为ULONG  FALSE-key为字符串
//		bIsUnicode[IN]-当key为字符串时，字符类型。	TRUE-宽字符wchar_t  FALSE-窄字符char
//		pIsEqual[IN]-回调函数指针。用于判断两个节点是否相等，不允许为空。
//		pGetRealKey[IN]-回调函数指针。用于获取真正的key数据，在key中有额外数据时使用，可为空。
// 返回值: TRUE-成功，FALSE-失败。
/**************************************************************/
BOOLEAN InitBobMap(IN PBobMap pMap, IN ULONG uListArrayCount, 
	IN ULONG uDataLen, IN BOOLEAN bIsNPage, IN ULONG uTag, IN BOOLEAN bIsUlongKey, 
	IN BOOLEAN bIsUnicode, IN _MapIsEqualNode pIsEqual, IN _MapGetRealKey pGetRealKey);

/**************************************************************/
// 反初始化map
// 参数:
//		pMap[IN]-map数据
// 返回值:
/**************************************************************/
VOID UnInitBobMap( IN PBobMap pMap );

/**************************************************************/
// 清除map里所有节点
// 参数:
//		pMap[IN]-map数据
// 返回值:
/**************************************************************/
VOID ClearAllNodeOfBobMap( IN PBobMap pMap );

/**************************************************************/
// 返回map中的节点数
// 参数:
//		pMap[IN]-map数据
// 返回值: 节点数。
/**************************************************************/
ULONG GetNodeNumInBobMap(IN PBobMap pMap);

/**************************************************************/
// 添加一个节点
// 参数:
//		pMap[IN]-map数据
//		pKey[IN]-关键字符串
//		pSrcData[IN]-节点数据
// 返回值: TRUE-成功，FALSE-失败。
/**************************************************************/
BOOLEAN AddNodeToBobMap( IN PBobMap pMap, IN void* pKey, IN PVOID pSrcData );
 
/**************************************************************/
// 删除节点。节点中的数据也自动释放。
// 参数:
//		pMap[IN]-map数据
//		pKey[IN]-关键字符串
//		pGetData[OUT]-返回删除的节点数据
// 返回值: TRUE-成功，FALSE-失败。
/**************************************************************/
BOOLEAN DeleteNodeFromBobMap( IN PBobMap pMap, IN void* pKey, OUT PVOID pGetData);

/**************************************************************/
// 查找一个节点
// 参数:
//		pMap[IN]-map数据
//		pKey[IN]-关键字符串
//		pGetData[OUT]-返回找到的节点数据
// 返回值: TRUE-找到，FALSE-未找到。
/**************************************************************/
BOOLEAN FindNodeInBobMap( IN PBobMap pMap, IN void* pKey, OUT PVOID pGetData/* = NULL*/ );

/**************************************************************/
// 查找并返回数据节点地址
// 参数:
//		pMap[IN]-map数据
//		pKey[IN]-关键字符串
// 返回值: 非NULL-找到，NULL-未找到。
/**************************************************************/
PVOID GetAddrOfNode( IN PBobMap pMap, IN void* pKey );

/**************************************************************/
// 枚举map中所有节点
// 参数:
//		pMap[IN]-map数据
//		pEnumFun[IN]-枚举回调函数
//		pForeign[IN]-外部传入的数据
//		pParam[IN]-外部传入的参数
// 返回值: 
/**************************************************************/
VOID EnumAllNodeOfBobMap(IN PBobMap pMap, IN _EnumMapNode pEnumFun, IN PVOID pForeign, IN PVOID pParam);