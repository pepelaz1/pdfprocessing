// sample.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../pdfprocessing/pdfprocessing.h"
#include <string>
using namespace std;

void print_usage()
{
	printf("usage:\n");
	printf("add page: sample.exe -a <source file> <page number> <append file> <destination file>\n");
	printf("delete page: sample.exe -d <source file> <page number>\n");
	printf("merge: sample.exe -m <source file 1> <source file 2> <destination file>\n");
	printf("split: sample.exe -s <source file> <start page number> <destination file 1> <destination file 2>\n");
	printf("crop: sample.exe -c <source file> <left> <top> <right> <bottom> <destination file>\n");
	printf("info: sample.exe -i <source file>\n");
}

int _tmain(int argc, _TCHAR* argv[])
{
	if ( argc > 2 )
	{
		if (!lstrcmp(argv[1],"-a"))
		{
			if (argc < 6)
				print_usage();
			else
			{
				PDFSetProperty("producer", "Jimmy");
				PDFAddPage(argv[2], atoi(argv[3]), argv[4], argv[5]);
			}
		}
		else if (!lstrcmp(argv[1],"-d"))
		{
			if (argc < 4)
				print_usage();
			else
				PDFDeletePage(argv[2], atoi(argv[3]));
		}
		else if (!lstrcmp(argv[1],"-m"))
		{
			if (argc < 5)
				print_usage();
			else
				PDFMerge(argv[2], argv[3], argv[4]);
		}
		else if (!lstrcmp(argv[1],"-s"))
		{
			if (argc < 6)
				print_usage();
			else
				PDFSplit(argv[2], atoi(argv[3]), argv[4], argv[5]);
		}
		else if (!lstrcmp(argv[1],"-c"))
		{
			if (argc < 8)
				print_usage();
			else
				PDFCrop(argv[2], atoi(argv[3]), atoi(argv[4]), atoi(argv[5]), atoi(argv[6]), argv[7]);
		}
		else if (!lstrcmp(argv[1],"-i"))
		{
			if (argc < 3)
				print_usage();
			else
			{
				int width, height;
				PDFGetFileInfo(argv[2], &width, &height);
				printf("Width = %d\nHeight = %d\n", width, height);
				PDFPreload(argv[2]);
				printf("Author = %s\nKeyword = %s\nTitle = %s\nSubject = %s\nProducer = %s\nCreation date = %s\nModify date = %s\nIs encrypted = %d\nVersion number = %.1f\n "
					, PDFGetAuthor(), PDFGetKeyword(), PDFGetTitle(), PDFGetSubject(), PDFGetProducer(), PDFGetCreationDate(), PDFGetModifyDate(), PDFIsEncrypted(), PDFGetVersionNo());
			}
		}
		else 
			print_usage();
	}
	else
		print_usage();
	return 0;
}

