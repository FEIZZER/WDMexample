#include <iostream>
#include <sstream>





int main(int args, char* argv[])
{
	std::stringbuf sb;
	std::istream is(&sb);
	std::ostream os(&sb);
	std::iostream ios(&sb);

	char msg[] = { 'f' , 'g' , '0x0' , 'u' ,'0x0' };
	char out[10] = { 0 };

	ios.write(msg, 5);
	std::cout << ios.gcount();
	std::cout << "eof:" << ios.read(out, 10).eof() << "readed:" << ios.gcount();

	return 0;
}