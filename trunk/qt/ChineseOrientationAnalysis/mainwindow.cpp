#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include "libs_win32/ICTCLAS50.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_trainTextFileListModel = new QStandardItemModel ();
    ui->trainTextFileListView->setModel( m_trainTextFileListModel );

	this->connect( ui->pushButtonExplorer, SIGNAL( clicked() ), this, SLOT( OnExplorerDir() ) );
	this->connect( ui->pushButtonUpdate, SIGNAL( clicked() ), this, SLOT( OnUpdate() ) );
	this->connect( ui->pushButtonParse, SIGNAL( clicked() ), this, SLOT( OnParse() ) );

	this->OnUpdate();

	this->ICTCLASInit();
}

MainWindow::~MainWindow()
{
    delete ui;

	// ����,�ͷ���Դ.
	ICTCLAS_Exit();
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

void MainWindow::ICTCLASInit()
{
	if(!ICTCLAS_Init()) //��ʼ���ִ������
	{
		QMessageBox errMsgBox(this);
		errMsgBox.setText( tr( "ICTCLAS�ִ�ϵͳ��ʼ��ʧ��, �����Ƿ�����ȨʧЧ!!" ) );
		errMsgBox.exec();
		return ;
	}
	else
	{
		qDebug("Init ok\n");
	}

	//���ô��Ա�ע��(0 ������������ע����1 ������һ����ע����2 ���������ע����3 ����һ����ע��)
	ICTCLAS_SetPOSmap(2);
}

void MainWindow::OnParse()
{
	ICTCLAS_FileProcess("Test.txt", "Test_result.txt",CODE_TYPE_GB,1);

	QString curDirPath = ui->labelTrainningFileDir->text();
	QDir dir( curDirPath );
	QFileInfoList trainningFileInfoList = dir.entryInfoList( QStringList( tr( "*.txt" ) ), QDir::Files );
	foreach( const QFileInfo& fileInfo, trainningFileInfoList )
	{
		QString resultPath = fileInfo.absoluteFilePath() +"_ICTCLAS.tmp";

		bool bOk = ICTCLAS_FileProcess( fileInfo.absoluteFilePath().toLocal8Bit(), resultPath.toLocal8Bit(),CODE_TYPE_GB,1);
		bool kkk = bOk;
		kkk;
	}	
}
