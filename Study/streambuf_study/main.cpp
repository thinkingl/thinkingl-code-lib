#include <iostream>

using namespace std;

class MyOStream : public ostream
{
public:
	MyOStream();

private:
	class Buffer : public streambuf {
	public:
		Buffer();
		virtual int overflow(int = EOF);
		virtual int underflow();
		virtual int sync();
		char buffer[250];
	} buffer;
};

MyOStream::MyOStream()
	:ostream(&buffer)
{

}

MyOStream::Buffer::Buffer()
{
	setg(buffer, buffer, &buffer[sizeof(buffer) - 2]);
	setp(buffer, &buffer[sizeof(buffer) - 2]);
}

int MyOStream::Buffer::overflow(int c)
{
	int bufSize = pptr() - pbase();

	if (c != EOF) {
		*pptr() = (char)c;
		bufSize++;
	}

	if (bufSize != 0) {
		char * p = pbase();
		setp(p, epptr());
		p[bufSize] = '\0';

#ifdef UNICODE
		// Note we do NOT use PWideString here as it could cause infinitely
		// recursive calls if there is an error!
		PINDEX length = strlen(p);
		wchar_t * unicode = new wchar_t[length + 1];
		unicode[length] = 0;
		MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, p, length, unicode, length + 1);
		OutputDebugString(unicode);
		delete[] unicode;
#else
		//OutputDebugString(p);
		cout << p;
#endif
	}

	return 0;
}


int MyOStream::Buffer::underflow()
{
	return EOF;
}


int MyOStream::Buffer::sync()
{
	return overflow(EOF);
}

int main()
{
	cout << "streambuf study run!" << endl;

	MyOStream testOS;
	testOS << "test log ... num [" << 123 << "]" << endl;
	testOS << "line 2: sfsdfasf";
	testOS << "\n";
	testOS.
	testOS << endl;
	char Big[255] = { 0 };
	for (int i = 0; i < sizeof(Big)-1; ++i)
	{
		Big[i] = '0';
	}
	for (int i = 0; i < 10; ++i)
	{
		char c = '1' + i;
		Big[244 + i] = c;
	}
	Big[254] = 0;
	testOS << Big;
	testOS << endl;
	testOS.flush();
	return 0;
}