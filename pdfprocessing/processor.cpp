#include "StdAfx.h"
#include "processor.h"
#include "ImageExtractor.h"


CProcessor::CProcessor()
{
}


CProcessor::~CProcessor()
{
}

bool CProcessor::AddPage(char *src, int pageno, char *append, char *out)
{
	try {

		PdfMemDocument *doc1 = new PdfMemDocument(src);	
		PdfMemDocument *doc2 = new PdfMemDocument(append);	

		PdfMemDocument dst;
		dst.InsertPages(*doc1,0,pageno);
		dst.InsertPages(*doc2,0,doc2->GetPageCount());
		dst.InsertPages(src,pageno,doc1->GetPageCount() - pageno);
	
		Compress(dst);
		ApplyProperties(dst);
		dst.Write(out); 
	

		delete doc1;
		delete doc2;

		printf("Ok");
		return true;
	}
	catch( PdfError &e)
	{
		e.PrintErrorMsg();
		return false;
	}
}	

void CProcessor::DeletePage(char *src, int pageno)
{
	try {
		string tmp;
		string err, msg;

		/*char s[MAX_PATH];
		DWORD r1 = GetTempPath(MAX_PATH,s);

		if (!r1)
		m_logger.Put("Failed to get temporary path");

		string path = s;
		path = path + "pdfprocessing";
		msg = "Temporary path " + path;
		m_logger.Put(msg);

		if (!CreateDirectory(path.c_str(), NULL))
		{
			if (GetLastError() != ERROR_ALREADY_EXISTS)
			{
				err = "Failed to create temp directory " + path;
				m_logger.Put(err);
			}
		}
		else
		{
			msg = "Created temporary directory " + path;
			m_logger.Put(msg);
		}		
		
		GetTempFileName(path.c_str(),"",0,tmp);
		
		msg = "Temporary file name ";
		msg += tmp;
		m_logger.Put(msg);*/

		msg = "Input file - ";
		msg += src;
		m_logger.Put(msg);

		tmp = src;
		tmp += "___";
		
		msg = "Tmp file - ";
		msg += tmp;
		m_logger.Put(msg);



		if (!CopyFile(src,tmp.c_str(),false))
		{
			DWORD ierr = GetLastError();
			char serr[MAX_PATH];
			sprintf(serr,"%d",ierr);
			err = "Failed to copy source file to temporary file. Error ";
			err += serr;
			m_logger.Put(err);
		}
		else
		{  
			m_logger.Put("Source file copied to temporary file");
		}
		PdfMemDocument *doc = new PdfMemDocument(tmp.c_str());
		m_logger.Put("Document loaded from temporary file");
		doc->DeletePages(pageno,1);
		m_logger.Put("Pages deleted");
		Compress(*doc);
		m_logger.Put("Compressed");
		ApplyProperties(*doc);
		m_logger.Put("Properties applied");
		doc->Write(src);
		m_logger.Put("Output file is written onto source file");
		delete doc;
		m_logger.Put("Document in memory deleted");
		DeleteFile(tmp.c_str());
		m_logger.Put("Temporary file deleted");
		printf("Ok");
	}
	catch( PdfError &e)
	{
		e.PrintErrorMsg();
	}
}

void CProcessor::Merge(char *src1, char *src2, char *dst)
{
	try {

	   PdfMemDocument *input1 = new PdfMemDocument(src1);
	   PdfMemDocument *input2 = new PdfMemDocument(src2);

	   PdfMemDocument out;
	   out.EmbedSubsetFonts();
	   out.InsertPages(*input1,0,input1->GetPageCount());
	   out.InsertPages(*input2,0,input2->GetPageCount());
	   out.EmbedSubsetFonts();
	   Compress(out);
	   ApplyProperties(out);
	   out.Write(dst);

	   delete input1;
	   delete input2;

	   printf("Ok");
	}
	catch( PdfError &e)
	{
		e.PrintErrorMsg();
	} 
}

void CProcessor::Split(char *src, int start_pageno, char *dst1, char *dst2)
{
	try {
	   PdfMemDocument input1(src);
	   PdfMemDocument input2(src);

	   input1.DeletePages(start_pageno, input1.GetPageCount() - start_pageno);
	   input2.DeletePages(0, start_pageno);
	   
	   ApplyProperties(input1);
	   ApplyProperties(input2);

	   Compress(input1);
	   Compress(input2);

	   input1.Write(dst1);
	   input2.Write(dst2);
	}
	catch( PdfError &e)
	{
		e.PrintErrorMsg();
	}
}

void CProcessor::Crop(char *src, int left, int top, int right, int bottom, char *dst)
{
	try {

		PdfMemDocument doc;
        doc.Load(src);
		
		for( int i = 0 ; i < doc.GetPageCount(); i++ ) 
        {
            PdfPage* page = doc.GetPage( i ); 
			PdfRect cbox = page->GetMediaBox();
			PdfRect r(left,cbox.GetHeight() - bottom,right-left,bottom-top);
			PdfVariant var;  
			r.ToVariant( var );
	
			page->GetObject()->GetDictionary().AddKey( PdfName("MediaBox"), var );
        }
		ApplyProperties(doc);
		doc.Write(dst);
	}
	catch( PdfError &e)
	{
		e.PrintErrorMsg();
	}
}

void CProcessor::GetFileInfo(char *src, int *width, int *height)
{
	try {
		PdfMemDocument doc (src);
		PdfPage* page = doc.GetPage( 0 ); 
		PdfRect cbox = page->GetMediaBox();
		*width= (int)cbox.GetWidth();
		*height = (int)cbox.GetHeight();
	}
	catch( PdfError &e)
	{
		e.PrintErrorMsg();
	}
}

void CProcessor::ApplyProperties(PdfMemDocument &doc)
{
	try {
		hash_map<string,string>::iterator it = _props.begin();
		for(;it != _props.end(); ++it)
		{
			if (it->first == "producer")
				doc.GetInfo()->SetProducer(it->second);
		}
	}
	catch( PdfError &e)
	{
		e.PrintErrorMsg();
	}
}

void CProcessor::SetProperty(char *propname, char *propvalue)
{
	_props[propname] = propvalue;
}

void CProcessor::Compress(PdfMemDocument &doc)
{
	TIVecObjects it   = doc.GetObjects().begin();
	while( it != doc.GetObjects().end() )
	{
		bool dlt = false;
		if( (*it)->HasStream())
		{
			PdfMemStream *s = dynamic_cast<PdfMemStream*>((*it)->GetStream());
	//		s->Compress();
		}			
		++it;
	}
}

string CProcessor::FmtDate(PdfString dt)
{
	string src = dt.GetString();
	string res = src.substr(8,2) + "." + src.substr(6,2) + "." + src.substr(2,4) + " " +  src.substr(10,2) + ":" +  src.substr(12,2) + ":" +  src.substr(14,2) ;
	return res;
}

void CProcessor::Preload(char *strsourcefile)
{
	_doc.Load(strsourcefile);
	_author = _doc.GetInfo()->GetAuthor().IsValid() ? _doc.GetInfo()->GetAuthor() : "";
	_keywords = _doc.GetInfo()->GetKeywords().IsValid() ? _doc.GetInfo()->GetKeywords() : "";
	_title = _doc.GetInfo()->GetTitle().IsValid() ? _doc.GetInfo()->GetTitle() : "";
	_subject = _doc.GetInfo()->GetSubject().IsValid() ? _doc.GetInfo()->GetSubject() : "";
	_producer = _doc.GetInfo()->GetProducer().IsValid() ? _doc.GetInfo()->GetProducer() : "";
	_creation_date = _doc.GetInfo()->GetCreationDate().IsValid() ? FmtDate(_doc.GetInfo()->GetCreationDate()).c_str() : "";
	_modify_date = _doc.GetInfo()->GetModifyDate().IsValid() ? FmtDate(_doc.GetInfo()->GetModifyDate()).c_str() : "";
	_encrypted = _doc.GetEncrypted();
	_version = 1 + (double)_doc.GetPdfVersion()/10.0;
}

char*  CProcessor::GetKeyword()
{
	return (char *)_keywords.GetString( );
}

char*  CProcessor::GetAuthor()
{
	return (char *)_author.GetString();
}

char*  CProcessor::GetTitle()
{
	return (char *)_title.GetString();
}

char*  CProcessor::GetSubject()
{
	return (char *)_subject.GetString();
}

char*  CProcessor::GetProducer()
{
	return (char *)_producer.GetString();
}

char*  CProcessor::GetCreationDate()
{
	return (char *)_creation_date.GetString();
}

char*  CProcessor::GetModifyDate()
{
	return (char *)_modify_date.GetString();
}

bool  CProcessor::IsEncrypted()
{
	return _encrypted;
}

double  CProcessor::GetVersionNo()
{
	return _version;
}