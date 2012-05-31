#ifndef CWORDSPLITER_H
#define CWORDSPLITER_H

#include <QObject>
#include <vector>

class CWord
{
public:
	QString m_word;				// 词.
	QString m_docFileName;	// 所在文件.
	int m_pos;							// 位置.
	QString m_wordType;		// 词性.
	QString m_content;			// 上下文.
};
typedef std::vector< CWord > CWordList;

class CWordSpliter : public QObject
{
	Q_OBJECT

public:
	CWordSpliter(QObject *parent);
	~CWordSpliter();

	bool Init();

	bool SplitWords( QString filePath, CWordList& wordList );

private:
	
};

#endif // CWORDSPLITER_H
