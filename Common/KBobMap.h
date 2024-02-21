
#pragma once

/*********************************************************************/
// writed by bob													 //
// ��ͬ������ʱ����irql/����˯��������÷Ƿ�ҳ�ڴ��KSPIN_LOCK��			 //
//			    ��irql/��Ҫ˯��������÷�ҳ�ڴ��FAST_MUTEX��  		 //
/*********************************************************************/


/**************************************************************/
// �жϽڵ��Ƿ����
// ����:
//		pNodeInMap[IN]-map������ݽڵ�
//		pForeignKey[IN]-�ⲿ�����key
// ����ֵ: TRUE-��ȣ�FALSE-����ȡ�
/**************************************************************/
typedef BOOLEAN (*_MapIsEqualNode)( IN PVOID pNodeInMap, IN void* pForeignKey );

/**************************************************************/
// ��ȡ������key���ݣ���key���ж�������ʱʹ�ã�
// ����:
//		pForeignKey[IN]-�ⲿ�����key
// ����ֵ: ������key���ݡ�
/**************************************************************/
typedef void* (*_MapGetRealKey)(IN void* pForeignKey);

/**************************************************************/
// ö�ٽڵ�ص�����
// ����:
//		pNodeInMap[IN]-map������ݽڵ�
//		pForeign[IN]-�ⲿ���������
//		pParam[IN]-�ⲿ����Ĳ���
// ����ֵ: �Ƿ��˳�ö�٣�TRUE-�˳���FALSE-���˳���
/**************************************************************/
typedef BOOLEAN(*_EnumMapNode)(IN PVOID pNodeInMap, IN PVOID pForeign, IN PVOID pParam);

// map���ݡ�ע�⣺�����߲����޸�����ṹ��������г�Ա���ݡ�
typedef struct _BobMap
{
	NPAGED_LOOKASIDE_LIST	struNPLookAside;
	PAGED_LOOKASIDE_LIST	struPLookAside;
	BOOLEAN 					bIsNPage;						// �ڵ���ڴ����ԡ�  TRUE-�Ƿ�ҳ�ڴ�(�����ڴ�)  FALSE-��ҳ�ڴ�(�����ڴ�)

	ULONG					uCurNodeNum;					// map�����нڵ����

	PLIST_ENTRY				pListArray;						// ��������ָ��	
	ULONG					uListArrayCount;				// pListArray��Ԫ�ظ���

	ULONG					uTag;							// ͬExAllocatePoolWithTag��Tag
	ULONG					uDataLen;						// ��Ҫ������map�е�ÿ�����ݽڵ�Ĵ�С����λ���ֽ�

	BOOLEAN 					bIsUlongKey;					// key�Ƿ�Ϊһ��ULONG��	TRUE-keyΪULONG		FALSE-keyΪ�ַ���
	BOOLEAN 					bIsUnicode;						// ��keyΪ�ַ���ʱ���ַ�����ģʽ��	TRUE-WCHAR  FALSE-char
	
	_MapIsEqualNode			pIsEqual;						// �ص�����ָ�롣�����ж������ڵ��Ƿ���ȡ�
	_MapGetRealKey			pGetRealKey;					// ��ȡ������key���ݣ���key���ж�������ʱʹ�á�

} BobMap, *PBobMap;
 

/**************************************************************/
// ��ʼ��map
// ����:
//		pMap[IN]-map����
//		uListArrayCount[IN]-pListArray��Ԫ�ظ���
//		uDataLen[IN]-���ݽڵ�Ĵ�С����λ���ֽ�
//		bIsNPage[IN]-���ݽڵ���ڴ����ԡ�  TRUE-�Ƿ�ҳ�ڴ�  FALSE-��ҳ�ڴ�
//		uTag[IN]-�ڵ��ڴ��ʶ��ͬExAllocatePoolWithTag��Tag
//		bIsUlongKey[IN]-key�Ƿ�Ϊһ��ULONG��	TRUE-keyΪULONG  FALSE-keyΪ�ַ���
//		bIsUnicode[IN]-��keyΪ�ַ���ʱ���ַ����͡�	TRUE-���ַ�wchar_t  FALSE-խ�ַ�char
//		pIsEqual[IN]-�ص�����ָ�롣�����ж������ڵ��Ƿ���ȣ�������Ϊ�ա�
//		pGetRealKey[IN]-�ص�����ָ�롣���ڻ�ȡ������key���ݣ���key���ж�������ʱʹ�ã���Ϊ�ա�
// ����ֵ: TRUE-�ɹ���FALSE-ʧ�ܡ�
/**************************************************************/
BOOLEAN InitBobMap(IN PBobMap pMap, IN ULONG uListArrayCount, 
	IN ULONG uDataLen, IN BOOLEAN bIsNPage, IN ULONG uTag, IN BOOLEAN bIsUlongKey, 
	IN BOOLEAN bIsUnicode, IN _MapIsEqualNode pIsEqual, IN _MapGetRealKey pGetRealKey);

/**************************************************************/
// ����ʼ��map
// ����:
//		pMap[IN]-map����
// ����ֵ:
/**************************************************************/
VOID UnInitBobMap( IN PBobMap pMap );

/**************************************************************/
// ���map�����нڵ�
// ����:
//		pMap[IN]-map����
// ����ֵ:
/**************************************************************/
VOID ClearAllNodeOfBobMap( IN PBobMap pMap );

/**************************************************************/
// ����map�еĽڵ���
// ����:
//		pMap[IN]-map����
// ����ֵ: �ڵ�����
/**************************************************************/
ULONG GetNodeNumInBobMap(IN PBobMap pMap);

/**************************************************************/
// ���һ���ڵ�
// ����:
//		pMap[IN]-map����
//		pKey[IN]-�ؼ��ַ���
//		pSrcData[IN]-�ڵ�����
// ����ֵ: TRUE-�ɹ���FALSE-ʧ�ܡ�
/**************************************************************/
BOOLEAN AddNodeToBobMap( IN PBobMap pMap, IN void* pKey, IN PVOID pSrcData );
 
/**************************************************************/
// ɾ���ڵ㡣�ڵ��е�����Ҳ�Զ��ͷš�
// ����:
//		pMap[IN]-map����
//		pKey[IN]-�ؼ��ַ���
//		pGetData[OUT]-����ɾ���Ľڵ�����
// ����ֵ: TRUE-�ɹ���FALSE-ʧ�ܡ�
/**************************************************************/
BOOLEAN DeleteNodeFromBobMap( IN PBobMap pMap, IN void* pKey, OUT PVOID pGetData);

/**************************************************************/
// ����һ���ڵ�
// ����:
//		pMap[IN]-map����
//		pKey[IN]-�ؼ��ַ���
//		pGetData[OUT]-�����ҵ��Ľڵ�����
// ����ֵ: TRUE-�ҵ���FALSE-δ�ҵ���
/**************************************************************/
BOOLEAN FindNodeInBobMap( IN PBobMap pMap, IN void* pKey, OUT PVOID pGetData/* = NULL*/ );

/**************************************************************/
// ���Ҳ��������ݽڵ��ַ
// ����:
//		pMap[IN]-map����
//		pKey[IN]-�ؼ��ַ���
// ����ֵ: ��NULL-�ҵ���NULL-δ�ҵ���
/**************************************************************/
PVOID GetAddrOfNode( IN PBobMap pMap, IN void* pKey );

/**************************************************************/
// ö��map�����нڵ�
// ����:
//		pMap[IN]-map����
//		pEnumFun[IN]-ö�ٻص�����
//		pForeign[IN]-�ⲿ���������
//		pParam[IN]-�ⲿ����Ĳ���
// ����ֵ: 
/**************************************************************/
VOID EnumAllNodeOfBobMap(IN PBobMap pMap, IN _EnumMapNode pEnumFun, IN PVOID pForeign, IN PVOID pParam);