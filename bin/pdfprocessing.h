#pragma once
#include <Windows.h>

extern "C"
{
	__declspec(dllexport) void PDFAddPage(char *strsourcefile,int iInsertAtPageNo, char* strappendfile, char* stroutputfile);
	__declspec(dllexport) void PDFDeletePage(char *strsourcefile, int iPageNo);
	__declspec(dllexport) void PDFMerge(char *strsourcefile1, char *strsourcefile2, char *strmergedfile);
	__declspec(dllexport) void PDFSplit(char *strsourcefile1, int iStartFromPageNo, char * strOutputFile1,char * strOutputFile2);
	__declspec(dllexport) void PDFCrop(char *strsourcefile1, int iLeft, int iTop, int iRight, int iBottom, char* strOuputFile);
	__declspec(dllexport) void PDFGetFileInfo(char *strsourcefile, int *iWidth, int *iHeight);
	__declspec(dllexport) void PDFSetProperty(char *strPropName, char *strPropValue);
	
	

	__declspec(dllexport) void PDFPreload(char *strsourcefile);
	__declspec(dllexport) char* PDFGetKeyword();
	__declspec(dllexport) char* PDFGetAuthor();
	__declspec(dllexport) char* PDFGetTitle();
	__declspec(dllexport) char* PDFGetSubject();
	__declspec(dllexport) char* PDFGetProducer();
	__declspec(dllexport) char* PDFGetCreationDate();
	__declspec(dllexport) char* PDFGetModifyDate();
	__declspec(dllexport) bool PDFIsEncrypted();
	__declspec(dllexport) double PDFGetVersionNo();
}