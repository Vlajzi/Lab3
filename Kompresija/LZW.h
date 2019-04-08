#pragma once


#include <string>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <bitset>

using namespace std;

struct kod
{

	bitset<12> x;

};

typedef struct kod Kod;

class LZW
{
public:
	LZW();
	~LZW() = default;
	void* Compres(string file);
	void* Decopress(string file);

private:
	unordered_map<string,Kod> tabela;
	
};
