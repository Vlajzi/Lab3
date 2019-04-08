#include "LZW.h"

LZW::LZW()
{
	this->tabela = unordered_map<string, Kod>();
	for (int i = 0; i <= 255; i++)
	{
		string ch = "";
		ch += char(i);
		tabela[ch].x = i;
	}
}



void* LZW::Compres(string file)
{
	ifstream ulaz;
	ofstream izlaz;

	

#pragma region Ulaz
	ulaz.open(file, ios::binary);

	ulaz.seekg(0,ios::end);

	int size = ulaz.tellg();

	ulaz.seekg(0, ios::beg);

	char* stream = (char*)malloc(sizeof(char) * (size+1));
	
	ulaz.read(stream, size);

	ulaz.close();

	stream[size] = '\0';
	//cout << stream;
#pragma endregion
	int pom = file.find_last_of('.');
	file.replace(pom, 4, ".LZW");



	string c = "", p = "";

	p += stream[0];
	int code = 256;
	vector<Kod> izlazniKod;


	for (int i = 0; i <size; i++) {
		if (i != size - 1)
			c += stream[i + 1];
		if (tabela.find(p + c) != tabela.end()) 
		{
			p = p + c;//ako prelazi 12 bita
		}
		else 
		{
			/*cout << p << "\t" << tabela[p].x << "\t\t"
				<< p + c << "\t" << code << endl;*/
			izlazniKod.push_back(tabela[p]);
			if (code <= 4095)
			{
				tabela[p + c].x = code;
				code++;
			}
			p = c;
		}
		c = "";
	}
	//cout << p << "\t" << tabela[p].x << endl;
	izlazniKod.push_back(tabela[p]);
	//return &izlazniKod;

#pragma region PisanjeUFajl

	izlaz.open(file,ios::binary | ios::out);




	char* outStream = (char*)malloc(sizeof(char) * (izlazniKod.size()*2 + 1));
	pom = 0;
	bitset<12> mask1("111111110000");
	bitset<12> mask11 = mask1.flip();
	bitset<12> mask2("000011111111");
	bitset<12> mask22 = mask2.flip();
	mask1.flip();
	mask2.flip();
	bool ind = true;


	//izlaz.write((char*)& izlazniKod, izlazniKod.capacity());
	int i = 0;
	for ( i = 0; i < izlazniKod.size(); i++)
	{
		if (ind)
		{
			unsigned long test = ((izlazniKod[i].x & mask2)).to_ulong();
			bitset<12> provera = (izlazniKod[i].x & mask2);
			outStream[pom++] = (char)((izlazniKod[i].x & mask2)).to_ulong();
			ind = false;
		}
		else
		{
			unsigned long test = ((izlazniKod[i - 1].x) >> 8).to_ulong() + ((izlazniKod[i].x & mask11) << 4).to_ulong();
			bitset<12> provera = ((izlazniKod[i - 1].x) >> 8).to_ulong() + ((izlazniKod[i].x & mask11) << 4).to_ulong();
			outStream[pom++] = (char)((izlazniKod[i - 1].x) >> 8).to_ulong() + ((izlazniKod[i].x & mask11) << 4).to_ulong();
			//bitset<12> uns = (unsigned char)outStream[pom - 1];
			bitset<12> provera1 = ((izlazniKod[i].x & mask1) >> 4).to_ulong();
			unsigned long test1 = provera1.to_ulong();
			outStream[pom++] = (char)((izlazniKod[i].x & mask1) >> 4).to_ulong();
			
			ind = true;
		}
		

	}
	if (!ind)
	{
		outStream[pom++] = ((izlazniKod[i - 1].x) >> 8).to_ulong();
	}
	outStream[pom] = '\0';

	izlaz.write(outStream, pom);
	izlaz.close();
#pragma endregion


	return stream;//test
}

void* LZW::Decopress(string file)
{
	ifstream ulaz;
	ofstream izlaz;

	ulaz.open(file, ios::binary);

	ulaz.seekg(0, ios::end);

	int size = ulaz.tellg();

	ulaz.seekg(0, ios::beg);

	char* stream = (char*)malloc(sizeof(char) * (size + 1));

	ulaz.read(stream, size);

	ulaz.close();

	stream[size] = '\0';
	int i = 0;

	vector<Kod> ulazniKod;

	int pom = size;
	int ind = 0;
	int poz = 0;
	Kod elm1;
	Kod elmPom;
	Kod elm2;
	bitset<12> mask1("111100000000");
	bitset<12> mask2("000000001111");


	elm1.x ^= elm1.x;
	elm2.x ^= elm2.x;

	for (i = 0; i <  pom; i++)
	{
		if (ind == 0)
		{
			elm1.x = stream[i];
			ind = 1;
		}
		else if (ind == 1)
		{
			elmPom.x &= elmPom.x;
			elmPom.x = (unsigned char)stream[i];
			elm1.x |= (elmPom.x & mask2);
			ulazniKod.push_back(elm1);
			elm1.x ^= elm1.x;
			elm2.x = (elmPom.x >> 4);
			ind = 2;
		}
		else if (ind == 2)
		{
			elmPom.x &= elmPom.x;
			elmPom.x = (unsigned char)stream[i];
			elm2.x |= (elmPom.x <<= 4);
			ulazniKod.push_back(elm2);
			elm2.x ^= elm2.x;
			ind = 0;
		}

	}


	free(stream);
	stream = (char*)malloc(sizeof(char));
	unordered_map<int, string> table;
	
	for (int i = 0; i <= 255; i++) {
		string ch = "";
		ch += char(i);
		/*Kod k;
		k.x = i;*/
		table[i] = ch;
	}
	int old = ulazniKod[0].x.to_ulong(), n;
	string s = table[old];
	string c = "";
	c += s[0];
	cout << s;
	int count = 256;
	for (int i = 0; i < ulazniKod.size() - 1; i++) {
		n = ulazniKod[i + 1].x.to_ulong();
		if (table.find(n) == table.end()) {
			s = table[old];
			s = s + c;
		}
		else {
			s = table[n];
		}
		cout << s;
		c = "";
		c += s[0];
		/*Kod k;
		k.x = count;*/
		table[count] = table[old] + c;
		count++;
		old = n;
	}
	cout << endl;
		return nullptr;
}
