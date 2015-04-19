// pdfprocessing.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "processor.h"

CProcessor g_processor;

extern "C" 
{
	__declspec(dllexport) bool PDFAddPage(char *strsourcefile,int iInsertAtPageNo, char* strappendfile, char *strOutputFile)
	{
		return g_processor.AddPage(strsourcefile, iInsertAtPageNo, strappendfile, strOutputFile);
	}

	__declspec(dllexport) void PDFDeletePage(char *strsourcefile, int iPageNo)
	{
		g_processor.DeletePage(strsourcefile, iPageNo);
	}

	__declspec(dllexport) void PDFMerge(char *strsourcefile1, char *strsourcefile2, char *strmergedfile)
	{
		g_processor.Merge(strsourcefile1, strsourcefile2, strmergedfile);
	}

	__declspec(dllexport) void PDFSplit(char *strsourcefile1, int iStartFromPageNo, char * strOutputFile1,char * strOutputFile2)
	{
		g_processor.Split(strsourcefile1, iStartFromPageNo, strOutputFile1, strOutputFile2);
	}

	__declspec(dllexport) void PDFCrop(char *strsourcefile1, int iLeft, int iTop, int iRight, int iBottom, char* strOuputFile)
	{
		g_processor.Crop(strsourcefile1, iLeft, iTop, iRight,iBottom, strOuputFile );
	}

	__declspec(dllexport) void PDFGetFileInfo(char *strsourcefile1, int *iWidth, int *iHeight)
	{
		g_processor.GetFileInfo(strsourcefile1, iWidth, iHeight );
	}

	__declspec(dllexport) void PDFSetProperty(char *strPropName, char *strPropValue)
	{
		g_processor.SetProperty(strPropName, strPropValue );
	}

	__declspec(dllexport) void PDFPreload(char *strsourcefile)
	{
		g_processor.Preload(strsourcefile);
	}

	__declspec(dllexport) char* PDFGetKeyword()
	{
		return g_processor.GetKeyword();
	}

	__declspec(dllexport) char* PDFGetAuthor()
	{
		return g_processor.GetAuthor();
	}

	__declspec(dllexport) char* PDFGetTitle()
	{
		return g_processor.GetTitle();
	}

	__declspec(dllexport) char* PDFGetSubject()
	{
		return g_processor.GetSubject();
	}

	__declspec(dllexport) char* PDFGetProducer()
	{
		return g_processor.GetProducer();
	}

	__declspec(dllexport) char* PDFGetCreationDate()
	{
		return g_processor.GetCreationDate();
	}

	__declspec(dllexport) char* PDFGetModifyDate()
	{
		return g_processor.GetModifyDate();
	}

	__declspec(dllexport) bool PDFIsEncrypted()
	{
		return g_processor.IsEncrypted();
	}

	__declspec(dllexport)double PDFGetVersionNo()
	{
		return g_processor.GetVersionNo();
	}
}