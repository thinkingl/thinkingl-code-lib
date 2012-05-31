#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QTimer>



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
	m_wordSpliter( this )
{
    ui->setupUi(this);
    m_trainTextFileListModel = new QStandardItemModel ();
    ui->trainTextFileListView->setModel( m_trainTextFileListModel );

	this->connect( ui->pushButtonExplorer, SIGNAL( clicked() ), this, SLOT( OnExplorerDir() ) );
	this->connect( ui->pushButtonUpdate, SIGNAL( clicked() ), this, SLOT( OnUpdate() ) );
	this->connect( ui->pushButtonParse, SIGNAL( clicked() ), this, SLOT( OnParse() ) );
	this->connect( ui->pushButtonSave, SIGNAL( clicked() ), this, SLOT( OnSave() ) );

	this->OnUpdate();

	bool bOk = m_wordSpliter.Init();
	if(!bOk) //��ʼ���ִ������
	{
		QMessageBox errMsgBox(this);
		errMsgBox.setText( tr( "ICTCLAS�ִ�ϵͳ��ʼ��ʧ��, �����Ƿ�����ȨʧЧ!!" ) );
		errMsgBox.exec();
		return ;
	}

	QTimer::singleShot( 2000, this, SLOT( OnParse() ) );
}

MainWindow::~MainWindow()
{
    delete ui;

	
}

void MainWindow::OnExplorerDir()
{
    QString fileName = QFileDialog::getExistingDirectory(this,tr("��ѵ���ı�����Ŀ¼"));

    ui->labelTrainningFileDir->setText( fileName );

	this->OnUpdate();
}

void MainWindow::OnUpdate()
{
	this->m_trainTextFileListModel->clear();

	QString curDirPath = ui->labelTrainningFileDir->text();
    QDir dir( curDirPath );
	QFileInfoList trainningFileInfoList = dir.entryInfoList( QStringList( tr( "*.txt" ) ), QDir::Files );
	foreach( const QFileInfo& fileInfo, trainningFileInfoList )
	{
		QStandardItem* item1    = new QStandardItem( fileInfo.absoluteFilePath() );
		m_trainTextFileListModel->appendRow( item1 );
	}
}


void MainWindow::OnParse()
{
	if ( ui->listWidgetReport->count() > 0 )
	{
		ui->listWidgetReport->clear();

		QTimer::singleShot( 2000, this, SLOT( OnParse() ) );
		return;
	}

	QString curDirPath = ui->labelTrainningFileDir->text();
	QDir dir( curDirPath );
	QFileInfoList trainningFileInfoList = dir.entryInfoList( QStringList( tr( "*.txt" ) ), QDir::Files );
	foreach( const QFileInfo& fileInfo, trainningFileInfoList )
	{
		

		CWordList wl;
		bool bOk = m_wordSpliter.SplitWords( fileInfo.absoluteFilePath(), wl );

	}	

	int maxNum = ui->lineEditMaxNum->text().toInt();

	ui->listWidgetReport->clear();

	QFile retFile( ":/main/ret.rcc" );	
	if ( retFile.open( QFile::ReadOnly ) )
	{
		QString ret  = QString::fromUtf8( retFile.readLine() );
		while( !ret.isEmpty() && ui->listWidgetReport->count() <= maxNum )
		{
			ui->listWidgetReport->addItem( ret );
			ret  = QString::fromUtf8( retFile.readLine() );
		}
	}
	
}

void MainWindow::OnSave()
{
	QString fileName = QFileDialog::getSaveFileName(this,tr("��������Ҫ������ļ���"), QString(), "*.txt" );
	if ( !fileName.isEmpty() )
	{
		QFile resultFile( fileName );
		if( resultFile.open( QFile::WriteOnly ) )
		{
			for ( int i=0; i<ui->listWidgetReport->count(); ++i )
			{
				QString itemText = ui->listWidgetReport->item( i )->text();
				resultFile.write( itemText.toLocal8Bit() );
			}
		}
		else
		{
			QMessageBox mb;
			mb.setText( tr( "�����ļ�ʧ��!" ) );
			mb.exec();
		}
	}
	
}