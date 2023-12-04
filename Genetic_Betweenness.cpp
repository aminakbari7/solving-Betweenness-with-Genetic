#include <iostream>
#include<fstream>
#include<ctime>
using namespace std;
int state[946][3];//(x,y,z)   
int tedad = 0;
//---------------------------------------------add file to array 
void add_file_to_array()
{
	fstream f("test1.txt");//orginal from site 0
	if (!f) {
		cout << "Error opening file" << endl;
		return;
	}
	int x = 0, y = 0, z = 0;
	int i = 0;
	while (f >> x >> y >> z)
	{
		state[i][0] = x;
		state[i][1] = y;
		state[i][2] = z;
		i++;
	}
	tedad = i + 1;
	f.close();
}
//-----------------------------------------------------
struct Chromosome
{
	int A[100];
	int betweeness;
	Chromosome* next;

};
Chromosome* best = 0;
//------------------------------------------------------------
class population
{
public:
	Chromosome* people;
	int capacity;
	int members;
	int summembersbtwns;
	population(int n)
	{
		capacity = n;
		members = 0;
		summembersbtwns = 0;
		people = new Chromosome[n];
	}
	int randynum()
	{
		return rand() % (100);
	}
	void randomly_make_initial_population()
	{
		int s = 0;
		while (s < capacity)
		{
			for (int i = 0; i < 100;)
			{
				int q = randynum();
				if (arraycheck(people[s].A, q))
				{
					people[s].A[i] = q;
					i++;
				}
			}
			people[s].betweeness = countbetweenness(people[s].A);
			members++;
			summembersbtwns += people[s].betweeness;
			s++;
		}
		//////////////////////////////
		best = &people[0];
		for (int i = 1; i < members; i++)
		{
			if (best->betweeness > people[i].betweeness)
				best = &people[i];
		}
	}

	int countbetweenness(int* a)
	{
		int sum = 0;
		for (int i = 0; i < tedad - 1; i++)
		{
			int x = find(state[i][0], a);
			int y = find(state[i][1], a);
			int z = find(state[i][2], a);
			if (x < y && y < z) sum++;
			if (z < y && y < x) sum++;//1
		}
		return sum;
	}
	int find(int key, int* a)
	{
		for (int i = 0; i < 100; i++)
			if (a[i] == key)
				return i;
		return -1;
	}
	bool arraycheck(int* a, int x)
	{
		for (int j = 0; j < 100; j++)
			if (a[j] == x) return false;
		return true;

	}
	void print()
	{
		cout << "--------------------------------------------" << endl;
		for (int i = 0; i < members; i++)
			cout << people[i].betweeness << "          " << i << endl;
	}
	int whoismyparent()
	{
		double r = ((double)rand() / RAND_MAX) * summembersbtwns;
		int s = 0;
		for (int i = 0; i < members; i++)
		{
			s += people[i].betweeness;
			if (r <= s) return i;
		}
		return members-1;
	}
	Chromosome* hellochild(Chromosome* parent1, Chromosome* parent2, int crross)
	{
		int temp[100];
		for (int i = 0; i < 100; i++)temp[i] = i;
		Chromosome* child = new Chromosome;
		for (int i = 0; i < crross; i++)
		{
			child->A[i] = parent1->A[i];
			crash(temp, parent1->A[i]);
		}

		for (int i = crross; i < 100; i++)
		{
			if (check_other_parent(child->A, parent2->A[i], crross))
			{
				child->A[i] = parent2->A[i];
				crash(temp, parent2->A[i]);
			}
			else
				child->A[i] = -1;
		}

		for (int i = crross; i < 100; i++)
		{
			if (child->A[i] == -1)
				child->A[i] = give_value(temp);
		}
		child->betweeness = countbetweenness(child->A);
		//-------------------jahesh
		if (rand() % 20 == 0) {
			int oldb = child->betweeness;
			int s1 = randynum();
			int s2 = randynum();
			int t1 = child->A[s1];
			int t2 = child->A[s2];
			child->A[s1] = t2;
			child->A[s2] = t1;
		}
		//if (countbetweenness(child->A) < oldb)
		/*{
			child->A[s1] = t1;
			child->A[s2] = t2;
			child->betweeness = countbetweenness(child->A);

		}*/
		//--------
		return child;
	}
	int give_value(int* a)
	{
		for (int i = 0; i < 100; i++)
		{
			if (a[i] != -1)
			{
				int t = a[i];
				a[i] = -1;
				return t;
			}
		}
	}
	bool check_other_parent(int* a, int key, int crross)
	{
		for (int i = 0; i < crross; i++)
		{
			if (a[i] == key)return false;
		}
		return true;
	}
	void crash(int* a, int key)
	{
		for (int i = 0; i < 100; i++)
			if (a[i] == key)
				a[i] = -1;
	}
};
//---------------------------------------------------------------------
int main()
{
	int start_time = time(0);
	srand(time(0));
	add_file_to_array();
	population p(100);
	p.randomly_make_initial_population();
	int i = 0;
	do {
		population new_p(100);
		for (int i = 0; i <p.capacity; i++)
		{
			//------choose two parent
			Chromosome* parent1 = &p.people[p.whoismyparent()];
			Chromosome* parent2 = &p.people[p.whoismyparent()];
			//-------------crros place
			int crros = p.randynum();

			//--------time to build the child
			Chromosome* child = p.hellochild(parent1, parent2, crros);
			if (child->betweeness > best->betweeness)
			{
				best = child;
				cout << best->betweeness << endl;
			}
			//add
				new_p.people[new_p.members] = *child;
				new_p.members++;
				new_p.summembersbtwns += child->betweeness;
		}

		p = new_p;
		i++;

	} while (i < 100000);
	cout << "time : " << time(0) - start_time << endl;
	system("pause");
	return 0;
}