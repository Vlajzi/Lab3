#include <strstream>
#include <iostream>
#include <fstream>
#include <string>
#include "LZW.h"
#include "Shannon-Fano.h"
using namespace std;

static const char alphanumb[] = "0123456789 qwertyuiopasdfghjklzxcvbnm QWERTYUIOPASDFGHJKLZXCVBNM .,. \n";

void GenFile(int count);
void Probe();

int main(int argc, char* argv[])
{

	Probe();
	

	/*int help = 100;
	for (int i = 0; i < 5;i++)
	{
		GenFile(help);
		help *= 10;
	}*/

	/*bitset<12> mask1("111111110000");
	bitset<12> mask11 = mask1.flip();
	bitset<12> mask2("000011111111");
	bitset<12> mask22 = mask2.flip();
	mask1.flip();
	mask2.flip();

	bitset<12> proba("110110010100");
	bitset<12> proba1("111000011101");
	unsigned long t1 = (proba & mask2).to_ulong();
	bitset<12> provera = (proba & mask2);
	unsigned long t2 = ((proba) >> 8).to_ulong() + ((proba1 & mask11) << 4).to_ulong();
	bitset<12> provera1 = ((proba) >> 8).to_ulong() + ((proba1 & mask11) << 4).to_ulong();
	
	bitset<12> provera2 = ((proba1 & mask1) >> 4).to_ulong();
	unsigned long t3 = provera1.to_ulong();*/
	/*LZW test;
	char* s = (char*)test.Compres("Test.txt");
	test.Decopress("Test.LZW");*/
	ShannonFano sf;
	sf.Copress("File-1000.txt");
	sf.Decomress();
	cout << "end";
}

void GenFile(int count)
{
	srand(13);
	int size = strlen(alphanumb) - 1;

	char* doc = (char*)malloc(sizeof(char) * (count+1));

	for (int i = 0; i < count; i++)
	{
		doc[i] = alphanumb[rand() % size];
	}
	doc[count] = '\0';
	string st = "File-";
	st += to_string(count);
	st += ".txt";
	ofstream izlaz;

	try
	{
		izlaz.open(st);

		izlaz << doc;
	}
	catch (exception ex)
	{
		cout << ex.what();
	}
	izlaz.close();
}

void Probe()
{
	bitset<12> a("110010010010");

	char c = a.to_ulong();

	bitset<12> b = c;
	bitset<12> e = (unsigned char)c;
}