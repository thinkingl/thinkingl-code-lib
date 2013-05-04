
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QSlider>
#include <QtWidgets/QHBoxLayout>

int main(int argc, char *argv[])
{
	// Hello word. ֧��html-style��ǩ��label.
	QApplication a( argc, argv );
	QLabel * pLabel = new QLabel( "<h2><i>hello</i> <font color=red>qt!</font></h2>" );
	pLabel->show();

	// �򵥵�����ʾ��, �����ť�˳�����.
	QPushButton *pBtn = new QPushButton( "quit"/*, pLabel*/ );
	QObject::connect( pBtn, SIGNAL( clicked() ), &a, SLOT( quit() ) );
	pBtn->show();

	// �Ը��ӵ�����ʾ��, �����ؼ�����.
	QWidget* pWnd = new QWidget();
	pWnd->setWindowTitle( "Connect demo" );
	QSpinBox *pSpinBox = new QSpinBox;
	QSlider *pSlider = new QSlider( Qt::Horizontal );
	pSpinBox->setRange( 0, 100 );
	pSlider->setRange( 0, 100 );
	QObject::connect( pSpinBox, SIGNAL( valueChanged( int ) ), pSlider, SLOT( setValue( int ) ) );
	QObject::connect( pSlider, SIGNAL( valueChanged( int ) ), pSpinBox, SLOT( setValue( int ) ) );
	QHBoxLayout *pLayout = new QHBoxLayout;
	pLayout->addWidget( pSpinBox );
	pLayout->addWidget( pSlider );
	pWnd->setLayout( pLayout );
	pWnd->show();

	return a.exec();
}
