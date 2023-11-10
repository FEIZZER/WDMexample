
#ifndef PCH_H
#define PCH_H

// ����Ҫ�ڴ˴�Ԥ����ı�ͷ
#include "stdbool.h"

#define PNTHEADER(base)  ((PIMAGE_NT_HEADERS)(((PIMAGE_DOS_HEADER)base)->e_lfanew + (ULONG_PTR)base))

char* oleaut32_ordinal2funcname[444] = {
        "","","SysAllocString","SysReAllocString",
        "SysAllocStringLen","SysReAllocStringLen","SysFreeString","SysStringLen",
        "VariantInit","VariantClear","VariantCopy","VariantCopyInd",
        "VariantChangeType","VariantTimeToDosDateTime","DosDateTimeToVariantTime","SafeArrayCreate",
        "SafeArrayDestroy","SafeArrayGetDim","SafeArrayGetElemsize","SafeArrayGetUBound",
        "SafeArrayGetLBound","SafeArrayLock","SafeArrayUnlock","SafeArrayAccessData",
        "SafeArrayUnaccessData","SafeArrayGetElement","SafeArrayPutElement","SafeArrayCopy",
        "DispGetParam","DispGetIDsOfNames","DispInvoke","CreateDispTypeInfo",
        "CreateStdDispatch","RegisterActiveObject","RevokeActiveObject","GetActiveObject",
        "SafeArrayAllocDescriptor","SafeArrayAllocData","SafeArrayDestroyDescriptor","SafeArrayDestroyData",
        "SafeArrayRedim","SafeArrayAllocDescriptorEx","SafeArrayCreateEx","SafeArrayCreateVectorEx",
        "SafeArraySetRecordInfo","SafeArrayGetRecordInfo","VarParseNumFromStr","VarNumFromParseNum",
        "VarI2FromUI1","VarI2FromI4","VarI2FromR4","VarI2FromR8",
        "VarI2FromCy","VarI2FromDate","VarI2FromStr","VarI2FromDisp",
        "VarI2FromBool","SafeArraySetIID","VarI4FromUI1","VarI4FromI2",
        "VarI4FromR4","VarI4FromR8","VarI4FromCy","VarI4FromDate",
        "VarI4FromStr","VarI4FromDisp","VarI4FromBool","SafeArrayGetIID",
        "VarR4FromUI1","VarR4FromI2","VarR4FromI4","VarR4FromR8",
        "VarR4FromCy","VarR4FromDate","VarR4FromStr","VarR4FromDisp",
        "VarR4FromBool","SafeArrayGetVartype","VarR8FromUI1","VarR8FromI2",
        "VarR8FromI4","VarR8FromR4","VarR8FromCy","VarR8FromDate",
        "VarR8FromStr","VarR8FromDisp","VarR8FromBool","VarFormat",
        "VarDateFromUI1","VarDateFromI2","VarDateFromI4","VarDateFromR4",
        "VarDateFromR8","VarDateFromCy","VarDateFromStr","VarDateFromDisp",
        "VarDateFromBool","VarFormatDateTime","VarCyFromUI1","VarCyFromI2",
        "VarCyFromI4","VarCyFromR4","VarCyFromR8","VarCyFromDate",
        "VarCyFromStr","VarCyFromDisp","VarCyFromBool","VarFormatNumber",
        "VarBstrFromUI1","VarBstrFromI2","VarBstrFromI4","VarBstrFromR4",
        "VarBstrFromR8","VarBstrFromCy","VarBstrFromDate","VarBstrFromDisp",
        "VarBstrFromBool","VarFormatPercent","VarBoolFromUI1","VarBoolFromI2",
        "VarBoolFromI4","VarBoolFromR4","VarBoolFromR8","VarBoolFromDate",
        "VarBoolFromCy","VarBoolFromStr","VarBoolFromDisp","VarFormatCurrency",
        "VarWeekdayName","VarMonthName","VarUI1FromI2","VarUI1FromI4",
        "VarUI1FromR4","VarUI1FromR8","VarUI1FromCy","VarUI1FromDate",
        "VarUI1FromStr","VarUI1FromDisp","VarUI1FromBool","VarFormatFromTokens",
        "VarTokenizeFormatString","VarAdd","VarAnd","VarDiv",
        "DllCanUnloadNow","DllGetClassObject","DispCallFunc","VariantChangeTypeEx",
        "SafeArrayPtrOfIndex","SysStringByteLen","SysAllocStringByteLen","DllRegisterServer",
        "VarEqv","VarIdiv","VarImp","VarMod",
        "VarMul","VarOr","VarPow","VarSub",
        "CreateTypeLib","LoadTypeLib","LoadRegTypeLib","RegisterTypeLib",
        "QueryPathOfRegTypeLib","LHashValOfNameSys","LHashValOfNameSysA","VarXor",
        "VarAbs","VarFix","OaBuildVersion","ClearCustData",
        "VarInt","VarNeg","VarNot","VarRound",
        "VarCmp","VarDecAdd","VarDecDiv","VarDecMul",
        "CreateTypeLib2","VarDecSub","VarDecAbs","LoadTypeLibEx",
        "SystemTimeToVariantTime","VariantTimeToSystemTime","UnRegisterTypeLib","VarDecFix",
        "VarDecInt","VarDecNeg","VarDecFromUI1","VarDecFromI2",
        "VarDecFromI4","VarDecFromR4","VarDecFromR8","VarDecFromDate",
        "VarDecFromCy","VarDecFromStr","VarDecFromDisp","VarDecFromBool",
        "GetErrorInfo","SetErrorInfo","CreateErrorInfo","VarDecRound",
        "VarDecCmp","VarI2FromI1","VarI2FromUI2","VarI2FromUI4",
        "VarI2FromDec","VarI4FromI1","VarI4FromUI2","VarI4FromUI4",
        "VarI4FromDec","VarR4FromI1","VarR4FromUI2","VarR4FromUI4",
        "VarR4FromDec","VarR8FromI1","VarR8FromUI2","VarR8FromUI4",
        "VarR8FromDec","VarDateFromI1","VarDateFromUI2","VarDateFromUI4",
        "VarDateFromDec","VarCyFromI1","VarCyFromUI2","VarCyFromUI4",
        "VarCyFromDec","VarBstrFromI1","VarBstrFromUI2","VarBstrFromUI4",
        "VarBstrFromDec","VarBoolFromI1","VarBoolFromUI2","VarBoolFromUI4",
        "VarBoolFromDec","VarUI1FromI1","VarUI1FromUI2","VarUI1FromUI4",
        "VarUI1FromDec","VarDecFromI1","VarDecFromUI2","VarDecFromUI4",
        "VarI1FromUI1","VarI1FromI2","VarI1FromI4","VarI1FromR4",
        "VarI1FromR8","VarI1FromDate","VarI1FromCy","VarI1FromStr",
        "VarI1FromDisp","VarI1FromBool","VarI1FromUI2","VarI1FromUI4",
        "VarI1FromDec","VarUI2FromUI1","VarUI2FromI2","VarUI2FromI4",
        "VarUI2FromR4","VarUI2FromR8","VarUI2FromDate","VarUI2FromCy",
        "VarUI2FromStr","VarUI2FromDisp","VarUI2FromBool","VarUI2FromI1",
        "VarUI2FromUI4","VarUI2FromDec","VarUI4FromUI1","VarUI4FromI2",
        "VarUI4FromI4","VarUI4FromR4","VarUI4FromR8","VarUI4FromDate",
        "VarUI4FromCy","VarUI4FromStr","VarUI4FromDisp","VarUI4FromBool",
        "VarUI4FromI1","VarUI4FromUI2","VarUI4FromDec","BSTR_UserSize",
        "BSTR_UserMarshal","BSTR_UserUnmarshal","BSTR_UserFree","VARIANT_UserSize",
        "VARIANT_UserMarshal","VARIANT_UserUnmarshal","VARIANT_UserFree","LPSAFEARRAY_UserSize",
        "LPSAFEARRAY_UserMarshal","LPSAFEARRAY_UserUnmarshal","LPSAFEARRAY_UserFree","LPSAFEARRAY_Size",
        "LPSAFEARRAY_Marshal","LPSAFEARRAY_Unmarshal","VarDecCmpR8","VarCyAdd",
        "DllUnregisterServer","OACreateTypeLib2","","VarCyMul",
        "VarCyMulI4","VarCySub","VarCyAbs","VarCyFix",
        "VarCyInt","VarCyNeg","VarCyRound","VarCyCmp",
        "VarCyCmpR8","VarBstrCat","VarBstrCmp","VarR8Pow",
        "VarR4CmpR8","VarR8Round","VarCat","VarDateFromUdateEx",
        "","","GetRecordInfoFromGuids","GetRecordInfoFromTypeInfo",
        "","SetVarConversionLocaleSetting","GetVarConversionLocaleSetting","SetOaNoCache",
        "","VarCyMulI8","VarDateFromUdate","VarUdateFromDate",
        "GetAltMonthNames","VarI8FromUI1","VarI8FromI2","VarI8FromR4",
        "VarI8FromR8","VarI8FromCy","VarI8FromDate","VarI8FromStr",
        "VarI8FromDisp","VarI8FromBool","VarI8FromI1","VarI8FromUI2",
        "VarI8FromUI4","VarI8FromDec","VarI2FromI8","VarI2FromUI8",
        "VarI4FromI8","VarI4FromUI8","","",
        "","","","",
        "","","","",
        "VarR4FromI8","VarR4FromUI8","VarR8FromI8","VarR8FromUI8",
        "VarDateFromI8","VarDateFromUI8","VarCyFromI8","VarCyFromUI8",
        "VarBstrFromI8","VarBstrFromUI8","VarBoolFromI8","VarBoolFromUI8",
        "VarUI1FromI8","VarUI1FromUI8","VarDecFromI8","VarDecFromUI8",
        "VarI1FromI8","VarI1FromUI8","VarUI2FromI8","VarUI2FromUI8",
        "","","","",
        "","","","",
        "","","","",
        "","","","",
        "","","","",
        "","OleLoadPictureEx","OleLoadPictureFileEx","",
        "","","","",
        "","","","SafeArrayCreateVector",
        "SafeArrayCopyData","VectorFromBstr","BstrFromVector","OleIconToCursor",
        "OleCreatePropertyFrameIndirect","OleCreatePropertyFrame","OleLoadPicture","OleCreatePictureIndirect",
        "OleCreateFontIndirect","OleTranslateColor","OleLoadPictureFile","OleSavePictureFile",
        "OleLoadPicturePath","VarUI4FromI8","VarUI4FromUI8","VarI8FromUI8",
        "VarUI8FromI8","VarUI8FromUI1","VarUI8FromI2","VarUI8FromR4",
        "VarUI8FromR8","VarUI8FromCy","VarUI8FromDate","VarUI8FromStr",
        "VarUI8FromDisp","VarUI8FromBool","VarUI8FromI1","VarUI8FromUI2",
        "VarUI8FromUI4","VarUI8FromDec","RegisterTypeLibForUser","UnRegisterTypeLibForUser"
};
char* ws2_32_ordinal2funcname[501] = {
        "","accept","bind","closesocket",
        "connect","getpeername","getsockname","getsockopt",
        "htonl","htons","ioctlsocket","inet_addr",
        "inet_ntoa","listen","ntohl","ntohs",
        "recv","recvfrom","select","send",
        "sendto","setsockopt","shutdown","socket",
        "GetAddrInfoW","GetNameInfoW","WSApSetPostRoutine","FreeAddrInfoW",
        "WPUCompleteOverlappedRequest","WSAAccept","WSAAddressToStringA","WSAAddressToStringW",
        "WSACloseEvent","WSAConnect","WSACreateEvent","WSADuplicateSocketA",
        "WSADuplicateSocketW","WSAEnumNameSpaceProvidersA","WSAEnumNameSpaceProvidersW","WSAEnumNetworkEvents",
        "WSAEnumProtocolsA","WSAEnumProtocolsW","WSAEventSelect","WSAGetOverlappedResult",
        "WSAGetQOSByName","WSAGetServiceClassInfoA","WSAGetServiceClassInfoW","WSAGetServiceClassNameByClassIdA",
        "WSAGetServiceClassNameByClassIdW","WSAHtonl","WSAHtons","gethostbyaddr",
        "gethostbyname","getprotobyname","getprotobynumber","getservbyname",
        "getservbyport","gethostname","WSAInstallServiceClassA","WSAInstallServiceClassW",
        "WSAIoctl","WSAJoinLeaf","WSALookupServiceBeginA","WSALookupServiceBeginW",
        "WSALookupServiceEnd","WSALookupServiceNextA","WSALookupServiceNextW","WSANSPIoctl",
        "WSANtohl","WSANtohs","WSAProviderConfigChange","WSARecv",
        "WSARecvDisconnect","WSARecvFrom","WSARemoveServiceClass","WSAResetEvent",
        "WSASend","WSASendDisconnect","WSASendTo","WSASetEvent",
        "WSASetServiceA","WSASetServiceW","WSASocketA","WSASocketW",
        "WSAStringToAddressA","WSAStringToAddressW","WSAWaitForMultipleEvents","WSCDeinstallProvider",
        "WSCEnableNSProvider","WSCEnumProtocols","WSCGetProviderPath","WSCInstallNameSpace",
        "WSCInstallProvider","WSCUnInstallNameSpace","WSCUpdateProvider","WSCWriteNameSpaceOrder",
        "WSCWriteProviderOrder","freeaddrinfo","getaddrinfo","getnameinfo",
        "","WSAAsyncSelect","WSAAsyncGetHostByAddr","WSAAsyncGetHostByName",
        "WSAAsyncGetProtoByNumber","WSAAsyncGetProtoByName","WSAAsyncGetServByPort","WSAAsyncGetServByName",
        "WSACancelAsyncRequest","WSASetBlockingHook","WSAUnhookBlockingHook","WSAGetLastError",
        "WSASetLastError","WSACancelBlockingCall","WSAIsBlocking","WSAStartup",
        "WSACleanup","","","",
        "","","","",
        "","","","",
        "","","","",
        "","","","",
        "","","","",
        "","","","",
        "","","","",
        "","","","__WSAFDIsSet",
        "","","","",
        "","","","",
        "","","","",
        "","","","",
        "","","","",
        "","","","",
        "","","","",
        "","","","",
        "","","","",
        "","","","",
        "","","","",
        "","","","",
        "","","","",
        "","","","",
        "","","","",
        "","","","",
        "","","","",
        "","","","",
        "","","","",
        "","","","",
        "","","","",
        "","","","",
        "","","","",
        "","","","",
        "","","","",
        "","","","",
        "","","","",
        "","","","",
        "","","","",
        "","","","",
        "","","","",
        "","","","",
        "","","","",
        "","","","",
        "","","","",
        "","","","",
        "","","","",
        "","","","",
        "","","","",
        "","","","",
        "","","","",
        "","","","",
        "","","","",
        "","","","",
        "","","","",
        "","","","",
        "","","","",
        "","","","",
        "","","","",
        "","","","",
        "","","","",
        "","","","",
        "","","","",
        "","","","",
        "","","","",
        "","","","",
        "","","","",
        "","","","",
        "","","","",
        "","","","",
        "","","","",
        "","","","",
        "","","","",
        "","","","",
        "","","","",
        "","","","",
        "","","","",
        "","","","",
        "","","","",
        "","","","",
        "","","","",
        "","","","",
        "","","","",
        "","","","",
        "","","","",
        "","","","",
        "","","","",
        "","","","",
        "","","","",
        "","","","",
        "","","","",
        "","","","",
        "","","","",
        "","","","",
        "","","","",
        "","","","",
        "","","","",
        "WEP"
};

#endif //PCH_H