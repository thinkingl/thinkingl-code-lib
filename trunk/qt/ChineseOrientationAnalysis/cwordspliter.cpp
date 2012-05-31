#include "cwordspliter.h"
#include "libs_win32/ICTCLAS50.h"
#include <QFile>

CWordSpliter::CWordSpliter(QObject *parent)
	: QObject(parent)
{

}

CWordSpliter::~CWordSpliter()
{
	// ����,�ͷ���Դ.
	ICTCLAS_Exit();
}

bool CWordSpliter::Init()
{
	bool bOk = ICTCLAS_Init(); //��ʼ���ִ������
	
	//���ô��Ա�ע��(0 ������������ע����1 ������һ����ע����2 ���������ע����3 ����һ����ע��)
	ICTCLAS_SetPOSmap(2);

	return bOk;
}

bool CWordSpliter::SplitWords( QString filePath, CWordList& wordList )
{
	QString resultPath = filePath +"_ICTCLAS.tmp";

	bool bOk = ICTCLAS_FileProcess( filePath.toLocal8Bit(), resultPath.toLocal8Bit(),CODE_TYPE_GB,1);

	QFile qf( resultPath );


	return bOk;
}