#pragma once
#include <vector>
#include <string>
#include <hash_map>
using namespace std;
#include "logger.h"
#include "podofo.h"
using namespace PoDoFo;

class CProcessor
{
private:
	CLogger m_logger;
	hash_map<string,string> _props;
	void ApplyProperties(PdfMemDocument &doc);
	void Compress(PdfMemDocument &doc);

	PdfMemDocument _doc;
	PdfString _author;
	PdfString _keywords;
	PdfString _title;
	PdfString _subject;
	PdfString _producer;
	PdfString _creation_date;
	PdfString _modify_date;
	bool _encrypted;
	double _version;

	string FmtDate(PdfString dt);
public:
	CProcessor();
	~CProcessor();
	bool AddPage(char *src, int pageno, char *append, char *out);
	void DeletePage(char *src, int pageno);
	void Merge(char *src1, char *src2, char *dst);
	void Split(char *src, int start_pageno, char *dst1, char *dst2);
	void Crop(char *src, int left, int top, int right, int bottom, char *dst);
	void GetFileInfo(char *src, int *width, int *height);
	void SetProperty(char *propname, char *propvalue);
	void Preload(char *sourcefile);
	char* GetKeyword();
	char* GetAuthor();
	char* GetTitle();
	char* GetSubject();
	char* GetProducer();
	char* GetCreationDate();
	char* GetModifyDate();
	bool IsEncrypted();
	double GetVersionNo();
	
};

