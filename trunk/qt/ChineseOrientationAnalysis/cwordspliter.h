#ifndef CWORDSPLITER_H
#define CWORDSPLITER_H

#include <QObject>
#include <vector>

class CWord
{
public:
	QString m_word;				// ��.
	QString m_docFileName;	// �����ļ�.
	int m_pos;							// λ��.
	QString m_wordType;		// ����.
	QString m_content;			// ������.
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
