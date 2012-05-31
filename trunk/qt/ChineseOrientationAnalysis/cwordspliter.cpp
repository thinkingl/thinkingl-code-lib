#include "cwordspliter.h"
#include "libs_win32/ICTCLAS50.h"
#include <QFile>

CWordSpliter::CWordSpliter(QObject *parent)
	: QObject(parent)
{

}

CWordSpliter::~CWordSpliter()
{
	// 销毁,释放资源.
	ICTCLAS_Exit();
}

bool CWordSpliter::Init()
{
	bool bOk = ICTCLAS_Init(); //初始化分词组件。
	
	//设置词性标注集(0 计算所二级标注集，1 计算所一级标注集，2 北大二级标注集，3 北大一级标注集)
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