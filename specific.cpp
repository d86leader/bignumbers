#include <iostream>
#include "bignumbers.h"

int main(int argc, char** argv)
{
	bignumber a;
	bignumber b;
	a.restore("92 db 2e 21 02 d6 dd 54 ec 33 69 db bb f6 31 4d e2 a7 06 a9 c8 35 92 21 c5 76 9e ed 1e 53 ca 0c dc a7 6b 7a 4d a2 63 62 af e8 08 a8 90 af 9a fa 9d 87 a2 ae d0 3e c6 9f 23 0c 72 62 f8 25 b4 86 42 ed 98 fa 22 5e bb e6 87 33 b5 4d 69 17 50 86 40 32 2a 48 74 ce fb 69 c1 45 b3 d8 7d 6a 79 45 bc 91 45 fa d3 b2 15 ee 6a 20 ee 00 c9 bb d7 e7 7f a2 5e dc ce 79 6e 85 2f 75 60 2b 92 28 87 06 11 c7 6c 10 52 cf 37 ad 75 1b 3f cf dc b7 45 c8 68 02 c0 f5 7e ff 48 00 f7 46 37 70 a5 92 a7 a3 db d9 e5 b1 4b 32 93 38 6f 5e 6b df 09 6e 10 1d 2f 67 6e 4e dc 5c a8 6e 4e 64 26 18 35 37 da 19 f7 f4 13 00 5b f5 2f 53 d6 cc 1b c2 c7 c8 20 02 c9 ae 46 8f a3 6d 50 a3 5c 96 c0 e0 5b 71 47 a1 76 53 e0 52 8a ca 5e 41 3c 78 9b 1f 34 c0 87 f8 1f ea b6 44 72 37 74 4a 2e 23 22 37 7f 24 c8 a1 3c 33 3f 33 68 8a b6 ca 52 13 81 39 7d 94 78 0b ea f8 e3 8c f5 31 15 f5 49 8d 7b 1e bd d6 18 12 a4 da ef aa 2a 81 b5 7f 7b 35 7e 7b 14 c1 ab 9f 87 21 50 00 f4 25 66 81 e6 03 40 f1 60 17 86 68 ed 1f e7 50 39 36 da 07 97 40 9a 07 83 d9 d5 03 99 ff 7a 13 58 1e 31 7b d6 43 cd 2b ee 90 ef 5a 74 75 ab 05 a0 be d0 95 f7 59 c4 c0 b7 79 21 a0 6a fc b4 34 06 6a 60 9b 53 d1 79 c6 4c 32 13 18 50 10 9f 2e 56 2e c2 3c 00 08 09 c6 7d e5 68 84 a3 cf 8a f2 43 ea e7 e8 59 b6 42 8b 4f 9c a4 df b4 ab dc fd 19 1c d6 b9 5a c4 02 45 1a 36 0a 61 61 63 8d a3 13 69 40 b9 29 b6 ad 03 bc e9 8e d3 9f 26 4d 68 45 57 98 83 66 f6 f2 f2 24 9a bc e6 31 3c 01 5f 98 4c b5 d6 8e df 73 9e 8b c3 de f1 81 39 16 d7 bb 83 50 63 a1 e1 8d 92 f6 f7 21 aa 64 ed 66 65 83 cf 9b e2 0e dd 6f 81 49 7c 06 a3 e7 71 52 64 74 e9 a9 0f c1 b6 23 3b 24 9a 7b 65 73 7f 25 ae 2a 13 af ed f1 64 74 ec 23 6e 31 d6 75 f5 2e f1 5a 6b 7f 37 28 a5 90 92 9e da c9 0a 58 88 14 4e 52 66 07 3a 88 0f f6 17 92 b9 3d 62 91 ad 50 ae 85 75 46 51 b0 bb ff eb 51 52 c3 91 33 49 b6 26 8c 7f c0 8e ff f6 f4 7c d7 6a f6 bd 06 4c ca d8 e1 5c 5f 9a 37 0a 8a 8c 77 a4 51 4e 8b 3e 5b 4e 75 80 4b ad 65 6a 42 0b b8 ec 4b 5c a0 f2 85 89 f5 fe ed 29 cc a2 9b 2b 37 12 73 ba b7 2f 98 02 e9 23 09 39 ee 30 0d 92 14 61 a7 b1 92 ce 57 d0 e7 f5 e6 bb fe 4d 1c 56 dc cd 40 91 f6 e3 33 75 92 c0 1a 0e a2 18 33 63 56 5d b3 c8 b9 e6 5a d3 b6 17 59 e8 e4 fb 9f 0f 89 96 ae f6 a9 d1 dc 91 f4 d7 47 7f 7a c3 bd 90 bf bb ac 12 59 82 cb 9f 0a ee be bc 26 72 a9 58 ae d0 db d5 91 5c 5f 5d c4 b0 ef 4f 79 7c fc 89 fd b0 60 fe 76 d7 17 08 cf 51 36 05 f5 3d ad 04 b9 1e 62 52 62 ce 37 47 88 c3 ce b0 64 0e f4 1b a1 24 9b c5 a5 2d 61 95 10 ec 4a 9c 18 fc 4e 73 17 25 a2 0c e6 e0 c8 26 f2 9e 19 7e 86 b2 e6 a8 91 ac 40 d2");
	b = 0xd240;

	auto t = a.quot_rem(b);
	auto quot = t.first;
	auto rem = t.second;

	if( quot*b + rem != a )
	{
		std::cout << "wrong division result!\n";
	}

	std::cout << '\t' << "a:"            << (a           ).dump() << std::endl;
	std::cout << '\t' << "b:"            << (b           ).dump() << std::endl;
	std::cout << '\t' << "quot:"         << (quot        ).dump() << std::endl;
	std::cout << '\t' << "rem:"          << (rem         ).dump() << std::endl;
	std::cout << '\t' << "quot*b:"       << (quot*b      ).dump() << std::endl;
	std::cout << '\t' << "quot*b + rem:" << (quot*b + rem).dump() << std::endl;
	std::cout << '\t' << "a - rem:"      << (a - rem     ).dump() << std::endl;

	if( quot*b + rem != a )
	{
		std::cout << "wrong division result!\n";
	}

	return 0;
}
