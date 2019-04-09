#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <bitset>
#include <algorithm>

using namespace std;

struct nod
{
	double count;
	char symbol;
	vector<bool> kod;
};

typedef struct nod Nod;

class ShannonFano
{
public:
	ShannonFano();
	~ShannonFano() = default;

	void Copress(string file);
	static inline bool sort(Nod a, Nod  b) {
		return (a.count > b.count);
	}
	void shannon(int l, int h,double total, vector<Nod> &p, bool poz);
private:
	
	vector<nod> sansa;
};
