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

	// 销毁,释放资源.
	ICTCLAS_Exit();
}

void MainWindow::OnExplorerDir()
{
    QString fileName = QFileDialog::getExistingDirectory(this,tr("打开训练文本所在目录"));

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
	if(!ICTCLAS_Init()) //初始化分词组件。
	{
		QMessageBox errMsgBox(this);
		errMsgBox.setText( tr( "ICTCLAS分词系统初始化失败, 请检查是否是授权失效!!" ) );
		errMsgBox.exec();
		return ;
	}
	else
	{
		qDebug("Init ok\n");
	}

	//设置词性标注集(0 计算所二级标注集，1 计算所一级标注集，2 北大二级标注集，3 北大一级标注集)
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
