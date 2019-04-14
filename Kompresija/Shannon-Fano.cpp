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

	stream[size] = '\0';

	for (int i = 0; i < size; i++)
	{
		sansa[stream[i]].count++;
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

	int i = 0;
	for (i = 0; i < sansa.size(); i++)
	{
		if (sansa[i].count == 0)
		{
			break;
		}
	}



	

	shannon(0, i,1,sansa,false);//prover false true?


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


	izlaz.open("Test.SF", ios::binary | ios::out);
	izlaz.write((char*)& pozition, sizeof(pozition));
	izlaz.write((char*)tabela, sizeof(double) * 256);
	izlaz.write(stream, (pozition));
	izlaz.close();
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

void ShannonFano::Decomress()
{
	ifstream ulaz;
	ofstream izlaz;

	ulaz.open("Test.SF", ios::binary);

	ulaz.seekg(0, ios::end);

	double* tabela = (double*)malloc(sizeof(double)*256);

	int size = ulaz.tellg();
	size -= sizeof(double) * 256 - sizeof(int);
	int numbOfC;
	ulaz.seekg(0, ios::beg);

	char* stream = (char*)malloc(sizeof(char) * (size + 1));
	ulaz.read((char*)& numbOfC, sizeof(int));
	ulaz.read((char*)tabela,sizeof(double)*256);
	ulaz.read(stream, size);

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
		if (sansa[1].count == 0)
			break;
	}
	stream[size] = '\0';

	shannon(0, i, 1, sansa, false);

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
