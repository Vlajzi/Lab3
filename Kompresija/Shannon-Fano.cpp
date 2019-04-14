#include "Shannon-Fano.h"

ShannonFano::ShannonFano()
{
	ResetSansa();
}



void ShannonFano::Copress(string file)
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
	int i = 0;
	stream[size] = '\0';

	for ( i = 0; i < size; i++)
	{
		sansa[(unsigned char)stream[i]].count++;
		//cout << i << endl;
	}
	


	for (int i = 0; i < sansa.size(); i++)
	{
		sansa[i].count /= size;
	}


	double* tabela = (double*)malloc(sizeof(double) * 256);

	for (int i = 0; i <256;i++ )
	{
		tabela[i] = sansa[i].count;
	}


	std::sort(sansa.begin(), sansa.end(), sort);

	//int i = 0;
	for (i = 0; i < sansa.size(); i++)
	{
		if (sansa[i].count == 0)
		{
			break;
		}
	}



	

	shannon(0, i,1,sansa,false);//prover false true?
	//shannonv2(0, i, sansa);


	int ind = 0;
	int j = 0;
	int pozition = 0;

	unsigned char c = '\0';

	for (int sp = 0; sp < size; sp++)
	{
		for (int i = 0; i < sansa.size(); i++)
		{
			if (sansa[i].count == 0)
			{
				break;
			}
			if (stream[sp] == sansa[i].symbol)
			{

				for (j = 0; j < sansa[i].kod.size(); j++)
				{
					unsigned char test = (((unsigned char)sansa[i].kod[j]) << ind);
					c += (((unsigned char)sansa[i].kod[j]) << ind);
					ind++;
					if (ind == 8)
					{
						stream[pozition++] = c;
						ind = 0;
						c = '\0';
					}
				}
			}
		}
	}
	if (ind != 0)
		stream[pozition++] = c;

	stream[pozition] = '\0';

	int pom = file.find_last_of('.');
	file.replace(pom, 4, ".SF");

	izlaz.open(file, ios::binary | ios::out);
	izlaz.write((char*)& size, sizeof(size));
	izlaz.write((char*)tabela, sizeof(double) * 256);
	izlaz.write(stream, (pozition));
	izlaz.close();
	WriteTable();
	//cout << stream;

}


void ShannonFano::shannon(int dole, int gore,double total, vector<Nod> &p,bool poz)
{
	double f1 = 0, mid, f2 = 0;
	mid = total / 2;
	int mpoint  = 0;

	if (gore <= dole)
	{
		return;
	}
	if (gore - dole <= 1)
	{
		return;
	}
	if (gore - dole == 2)
	{
		p[dole].kod.push_back(false);
		p[gore - 1].kod.push_back(true);
		return;
	}
	for (int i = dole; i < gore; i++)
	{
		f1 += p[i].count;
		p[i].kod.push_back(false);
		if (f1 >= mid)
		{
			mpoint = i+1;
			break;
		}
	}
	for (int i = mpoint; i < gore; i++)
	{
		f2 += p[i].count;
		p[i].kod.push_back(true);
	}



	shannon(dole, mpoint, f1, p, false);
	shannon(mpoint, gore, f2, p, true);


}

void ShannonFano::Decomress(string file)
{
	ifstream ulaz;
	ofstream izlaz;

	ulaz.open(file, ios::binary);

	ulaz.seekg(0, ios::end);

	double* tabela = (double*)malloc(sizeof(double)*256);

	int size = ulaz.tellg();
	if (size < 0)
		return;
	size -= sizeof(double) * 256 - sizeof(int);
	int numbOfC;
	ulaz.seekg(0, ios::beg);

	char* stream = (char*)malloc(sizeof(char) * (size + 1));

	ulaz.read((char*)& numbOfC, sizeof(int));
	ulaz.read((char*)tabela,sizeof(double)*256);
	ulaz.read(stream, size);

	char* rez = (char*)malloc(sizeof(char) * (numbOfC+1));

	ulaz.close();

	int i;

	ResetSansa();

	for (i = 0; i < 256; i++)
	{
		sansa[i].count = tabela[i];
		
	}

	std::sort(sansa.begin(), sansa.end(), sort);

	for (i = 0; i < 256; i++)
	{
		if (sansa[i].count == 0)
			break;
	}
	stream[size] = '\0';

	shannon(0, i, 1, sansa, false);
	//shannonv2(0, i, sansa);
	int pom = i;
	i = 0;
	int j = 0;
	int poz = 0;
	int k = 0;
	Nod comp;
	unsigned char c;
	while(k < numbOfC)
	{
		if (poz == 0)
		{
			 c = (unsigned char)stream[j];
		}
		if (k == 4)
		{
			k = k; //break
		}
		unsigned int test = c & 1;
		comp.kod.push_back((c & 1));
		c >>= 1;
		poz++;

		//while(comp.kod.size() == sansa[i].kod.size())
		while(true)
		{
			if (comp.kod == sansa[i].kod)
			{
				rez[k] = sansa[i].symbol;
				i = 0;
				comp.kod.clear();
				k++;
				break;
			}
			i++;
			if (i == pom)
			{
				i = 0;
				break;
			}
		}
		i = 0;
		if (poz == 8)
		{
			poz = 0;
			j++;
		}
	}
	rez[numbOfC] = '\0';

	pom = file.find_last_of('.');
	file.replace(pom, 4, ".txt");
	string iz_file = "";
	iz_file += "De-" + file;

	//cout << "Dekodirano: " << endl << rez;
	izlaz.open(iz_file, ios::binary);
	izlaz.write(rez, numbOfC + 1);
	izlaz.close();
}

void ShannonFano::WriteTable()
{
	cout << endl;
	int i = 0;
	vector<bool>::iterator p;

	cout << "Simbol\t" << "Procenat\t\t" << "Kod" << endl;
	while (sansa[i].count != 0 && i != 256)
	{
		cout << sansa[i].symbol << "\t" << sansa[i].count << "\t";
		for (p = sansa[i].kod.begin(); p != sansa[i].kod.end(); p++)
		{
			cout << *p;
		}
		i++;
		cout << endl;
	}

}

void ShannonFano::ResetSansa()
{
	sansa.clear();
	for (int i = 0; i < 256; i++)
	{
		Nod tmp;
		tmp.count = 0;
		tmp.symbol = char(i);

		sansa.push_back(tmp);
	}
}
void ShannonFano::shannonv2(int l, int h, vector<Nod>& p)
{
	double pack1 = 0, pack2 = 0, diff1 = 0, diff2 = 0;
	int i, d, k, j;
	if ((l + 1) == h || l == h || l > h) {
		if (l == h || l > h)
			return;
		p[h].kod.push_back(false);
		p[l].kod.push_back(true);
		return;
	}
	else {
		for (i = l; i <= h - 1; i++)
			pack1 = pack1 + p[i].count;
		pack2 = pack2 + p[h].count;
		diff1 = pack1 - pack2;
		if (diff1 < 0)
			diff1 = diff1 * -1;
		j = 2;
		while (j != h - l + 1) {
			k = h - j;
			pack1 = pack2 = 0;
			for (i = l; i <= k; i++)
				pack1 = pack1 + p[i].count;
			for (i = h; i > k; i--)
				pack2 = pack2 + p[i].count;
			diff2 = pack1 - pack2;
			if (diff2 < 0)
				diff2 = diff2 * -1;
			if (diff2 >= diff1)
				break;
			diff1 = diff2;
			j++;
		}
		k++;
		for (i = l; i <= k; i++)
			p[l].kod.push_back(true);
		for (i = k + 1; i <= h; i++)
			p[h].kod.push_back(false);

		// Invoke shannon function 
		shannonv2(l, k, p);
		shannonv2(k + 1, h, p);
	}
}