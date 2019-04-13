#include "Shannon-Fano.h"

ShannonFano::ShannonFano()
{
	for (int i = 0; i < 256; i++)
	{
		Nod tmp;
		tmp.count = 0;
		tmp.symbol = char(i);

		sansa.push_back(tmp);
	}
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


	cout << "end";

}


void ShannonFano::shannon(int dole, int gore,double total, vector<Nod> &p,bool poz)
{
	double f1 = 0, mid, f2 = 0;
	mid = total / 2;
	int mpoint;

	if (gore <= dole)
	{
		return;
	}

	if (gore - dole == 2)
	{
		p[dole].kod.push_back(false);
		p[gore - 1].kod.push_back(true);
		return;
	}
	if(gore - dole == 1)
	for (int i = dole; i < gore; i++)
	{
		f1 += p[i].count;
		p[i].kod.push_back(false);
		if (f1 >= mid)
		{
			mpoint = ++i;
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