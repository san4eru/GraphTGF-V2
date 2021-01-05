#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cstdlib>
#include <string>   
#include <vector>
#include <algorithm>
#include <fstream>
#include <functional>

using namespace std;

class Adder;
class Edge;
class Vertex;
class Graph;
class Files;
class ShowData;
class MainMenu;

class DATA
{
protected:
	vector<Edge*> edges;
	vector<Vertex*> vertexs;
	vector<vector<int>> listWays;
public:
	void ClearWays() 
	{
		listWays.clear();
	}
	virtual ~DATA()
	{
		vector<Edge*>().swap(edges);
		edges.shrink_to_fit();
		vector<Vertex*>().swap(vertexs);
		vertexs.shrink_to_fit();
		vector<vector<int>>().swap(listWays);
		listWays.shrink_to_fit();
	}
};

class Adder
{
private:
	int a, b, w, index;
	string label;
public:
	void ADEdge(vector<Vertex*>&, vector<Edge*>&, int);

	void ADVertex(vector<Vertex*>&, vector<Edge*>&, int);

	void ChangeEdge(vector<Edge*>&, int);

	void ChangeEdge(vector<Edge*>&);

	void ChangeVertex(vector<Vertex*>&);

	void ChangeVertex(vector<Vertex*>&, vector<Edge*>&);
};

class Edge
{
private:
	int u, v, weight;
	friend void Adder::ADEdge(vector<Vertex*>&, vector<Edge*>&, int);
	friend void Adder::ADVertex(vector<Vertex*>&, vector<Edge*>&, int);
	friend void Adder::ChangeEdge(vector<Edge*>&, int);
	friend void Adder::ChangeEdge(vector<Edge*>&);
	friend void Adder::ChangeVertex(vector<Vertex*>&, vector<Edge*>&);
	friend class Functions;
public:
	Edge(int u, int v, int weight)
	{
		this->u = u;
		this->v = v;
		this->weight = weight;
	}

	int U() {
		return u;
	}
	int V() {
		return v;
	}
	int WEIGHT() {
		return weight;
	}

	bool operator == (Edge &other) {
		return this->u == other.u && this->v == other.v;
	}
};

class Vertex
{
private:
	int index;
	string label;
	friend void Adder::ADVertex(vector<Vertex*>&, vector<Edge*>&, int);
	friend void Adder::ChangeVertex(vector<Vertex*>&);
	friend void Adder::ChangeVertex(vector<Vertex*>&, vector<Edge*>&);
	friend class Functions;
public:
	Vertex(int index, string label)
	{
		this->index = index;
		this->label = label;
	}

	int INDEX() {
		return index;
	}
	string LABEL() {
		return label;
	}

	bool operator == (const Vertex &other) {
		return this->index == other.index || this->label == other.label;
	}
};

class Graph 
{
private:
	bool Find(vector<int> &list, int x)
	{
		if (list.empty())
			return false;
		else
		{
			for (int i = 0; i < list.size(); i++)
			{
				if (list[i] == x)
					return true;
			}
		}
		return false;
	}

	bool Compare(vector<int> &L1, vector<int> &L2)
	{
		if (L1.size() == L2.size())
		{
			for (int i = 0; i < L1.size(); i++)
			{
				if (L1[i] != L2[i])
					return false;
			}
		}
		else
			return false;
		return true;
	}

	void AddWay(vector<int> &list, vector<vector<int>> &listWays)
	{
		int i = 0;
		bool temp = true;
		while (i < listWays.size() && temp)
		{
			if (Compare(list, listWays[i]))
				temp = false;
			else
				i++;
		}
		if (temp || i == listWays.size())
		{
			listWays.push_back(vector<int>());
			for (int j = 0; j < list.size(); j++)
			{
				listWays[listWays.size() - 1].push_back(list[j]);
			}
		}
	}

public:
	void Ways(vector<Edge*> &edges, int numberEdge, vector<int> &list, vector<vector<int>> &listWays, int b)
	{
		for (int i = 0; i < edges.size(); i++)
		{
			Edge* edge = edges[i];
			if (!Find(list, numberEdge))
			{
				if (edge->U() == numberEdge)
				{
					list.push_back(numberEdge);

					if (!Find(list, edge->V()))
						Ways(edges, edge->V(), list, listWays, b);
					else
					{
						if (edge->U() == b)
							AddWay(list, listWays);
					}
					list.erase(remove(list.begin(), list.end(), numberEdge), list.end());
				}
				else if (edge->V() == numberEdge)
				{
					list.push_back(numberEdge);

					if (!Find(list, edge->U()))
						Ways(edges, edge->U(), list, listWays, b);
					else
					{
						if (edge->V() == b)
							AddWay(list, listWays);
					}
					list.erase(remove(list.begin(), list.end(), numberEdge), list.end());
				}
			}
		}
	}
};

class Files
{
private:
	vector<string> split(const string& str, const char * delim)
	{
		vector<string> dest;
		char* pTempStr = _strdup(str.c_str());
		char* pWord = strtok(pTempStr, delim);
		while (pWord != NULL) {
			dest.push_back(pWord);
			pWord = strtok(NULL, delim);
		}

		free(pTempStr);

		return dest;
	}

public:
	int ReadFromFile(string FILENAME, vector<Vertex*>& vertexs, vector<Edge*>& edges) {
		char buff[255];
		vector<string> buffer;
		bool flag = 0;
		fstream fin;
		fin.open(FILENAME, ios::in);
		if (!fin.good())
		{
			return -1;
		}
		while (!fin.eof()) {
			fin.getline(buff, 20);
			if (buff[0] == '#') {
				flag = 1;
				continue;
			}
			if (flag == 0) {
				buffer = split(buff, " ");
				Vertex* tempVar = new Vertex(atoi(buffer[0].c_str()), buffer[1].c_str());
				vertexs.push_back(tempVar);
			}
			if (flag == 1) {
				buffer = split(buff, " ");
				Edge* tempVar1 = new Edge(atoi(buffer[0].c_str()), atoi(buffer[1].c_str()), atoi(buffer[2].c_str()));
				edges.push_back(tempVar1);
			}
		}

		fin.close();
		return 0;
	}

	void WriteFromFile(string FILENAME, vector<Vertex*> &vertexs, vector<Edge*> edges) {
		ofstream out(FILENAME);

		for (vector<Vertex*>::iterator ITER = vertexs.begin(); ITER != vertexs.end(); ITER++) {
			out << (*ITER)->INDEX() << " " << (*ITER)->LABEL() << endl;
		}
		out << "#" << endl;
		for (vector<Edge*>::iterator ITER = edges.begin(); ITER != edges.end(); ITER++)
			out << (*ITER)->U() << " " << (*ITER)->V() << " " << (*ITER)->WEIGHT() << endl;
		out.close();
	}
};

class ShowData
{
public:
	void Print(vector<vector<int>> list, vector<Vertex*> vertexs)
	{
		int j = 1;
		for (vector<vector<int>>::iterator ITER = list.begin(); ITER != list.end(); ITER++) {
			cout << "[" << j << "] ";
			for (const auto &i : *ITER) {
				for (vector<Vertex*>::iterator k = vertexs.begin(); k != vertexs.end(); k++) {
					if (i == (*k)->INDEX()) {
						cout << (*k)->LABEL() << " -> ";
						break;
					}
				}
			}
			cout << endl;
			j++;
		}
	}
	void Print(vector<Edge*> edges, vector<Vertex*> vertexs, int flag)
	{
		for (vector<Edge*>::iterator ITER = edges.begin(); ITER != edges.end(); ITER++) {
			for (vector<Vertex*>::iterator i = vertexs.begin(); i != vertexs.end(); i++) {
				if ((*ITER)->U() == (*i)->INDEX()) {
					cout << (*i)->LABEL() << " -> ";
					break;
				}
			}
			for (vector<Vertex*>::iterator i = vertexs.begin(); i != vertexs.end(); i++) {
				if ((*ITER)->V() == (*i)->INDEX()) {
					if (flag == 1)
						cout << (*i)->LABEL() << endl;
					else
						cout << (*i)->LABEL() << "   ���: " << (*ITER)->WEIGHT() << endl;
					break;
				}
			}
		}
	}
	void Print(vector<Vertex*> vertexs)
	{
		for (vector<Vertex*>::iterator i = vertexs.begin(); i != vertexs.end(); i++)
				cout << (*i)->LABEL() << " [" << (*i)->INDEX() << "]" << endl;
	}
};

class Functions : private DATA
{
private:

public:
	Functions (vector<Edge*> edges1, vector<Vertex*> vertexs1, vector<vector<int>> listWays1)
	{
		edges = edges1;
		vertexs = vertexs1;
		listWays = listWays1;
	}
	int FIRST(string label)
	{
		int index = -2;
		Vertex* tempVar = new Vertex(-1, label);

		for (int i = 0; i < vertexs.size(); i++)
		{
			if (vertexs[i]->label == tempVar->label)
				tempVar->index = vertexs[i]->index;
		}
		if (tempVar->index == -1)
			return -1;

		for (int j = 0; j < edges.size(); j++)
		{
			if (edges[j]->u == tempVar->index)
			{
				index = edges[j]->v;
				break;
			}

			if (edges[j]->v == tempVar->index)
			{
				index = edges[j]->u;
				break;
			}
		}

		return index;
	}
	int NEXT(string label, int index)
	{
		Vertex* tempVar = new Vertex(1, label);

		for (int i = 0; i < vertexs.size(); i++)
		{
			if (vertexs[i]->label == tempVar->label)
				tempVar->index = vertexs[i]->index;
		}
		int min = INT_MAX;
		for (int j = 0; j < edges.size(); j++)
		{
			if (edges[j]->u == tempVar->index)
			{
				if ((edges[j]->v > index) && (edges[j]->v < min))
					min = edges[j]->v;
			}

			if (edges[j]->v == tempVar->index)
			{
				if ((edges[j]->u > index) && (edges[j]->u < min))
					min = edges[j]->u;
			}
		}

		return min;
	}
	string VERTEX(string label, int index)
	{
		int sindex;
		string slabel;

		Vertex* tempVar = new Vertex(1, label);

		for (int i = 0; i < vertexs.size(); i++)
		{
			if (vertexs[i]->label == tempVar->label)
				tempVar->index = vertexs[i]->index;
		}

		for (int j = 0; j < edges.size(); j++)
		{
			if (edges[j]->u == tempVar->index)
			{
				if (edges[j]->v == index)
					sindex = edges[j]->v;
			}

			if (edges[j]->v == tempVar->index)
			{
				if (edges[j]->u == index)
					sindex = edges[j]->u;
			}
		}

		Vertex* tempVar1 = new Vertex(sindex, "1");
		for (int i = 0; i < vertexs.size(); i++)
		{
			if (vertexs[i]->index == tempVar1->index)
				slabel = vertexs[i]->label;
		}

		return slabel;
	}
};

void Adder::ADEdge(vector<Vertex*>& vertexs, vector<Edge*>& edges, int flag) 
{
	if (flag == 4)
	{
		cout << "������� ���� - ������ ������ � ��� (1 2 24): ";
		cin >> a >> b >> w;

		Edge* tempVar = new Edge(a, b, w);
		edges.push_back(tempVar);
	}
	else
	{
		cout << "������� ���� - ������ ������ (1 2): ";
		cin >> a >> b;
		w = 0;
		Edge* tempVar = new Edge(a, b, w);
		for (int i = 0; i < edges.size(); i++)
		{
			if (edges[i]->u == tempVar->u && edges[i]->v == tempVar->v)
			{
				w = edges[i]->weight;
				edges.erase(edges.begin() + i);
			}
		}
		cout << "���� " << a << " -> " << b << " ����� " << w << " - �������!"<< endl;
	}

}
void Adder::ADVertex(vector<Vertex*>& vertexs, vector<Edge*>& edges, int flag) 
{
	if (flag == 3) 
	{
		cout << "������� ����� ������� (������): ";
		cin >> index;
		cout << endl;
		cout << "������� �������� �������: ";
		cin >> label;

		Vertex* tempVar = new Vertex(index, label);
		vertexs.push_back(tempVar);
	}
	else 
	{
		vector<Edge> DELedges;
		Edge* stemp = new Edge(0, 0, 0);
		int Tflag = 0;
		cout << "������� �� 1 - ����� (������), 2 - ��������: " ;
		cin >> Tflag;
		cout << endl;
		if (Tflag == 1)
		{
			cout << "������� ����� (������) �������: ";
			cin >> index;
			label = "";
			w = 0;
			b = 0;
			Vertex* tempVar = new Vertex(index, label);
			for (int i = 0; i < vertexs.size(); i++)
			{
				if (vertexs[i]->index == tempVar->index)
				{
					Edge* tempVar1 = new Edge(index, b, w);
					for (int j = 0; j < edges.size(); j++)
					{
						if (edges[j]->u == tempVar1->u)
						{
							stemp = edges[j];
							DELedges.push_back(*stemp);
							edges.erase(edges.begin() + j);
						}
						if (edges[j]->v == tempVar1->u)
						{
							stemp = edges[j];
							DELedges.push_back(*stemp);
							edges.erase(edges.begin() + j);
						}
					}

					label = vertexs[i]->label;
					vertexs.erase(vertexs.begin() + i);
				}
			}
		}
		else 
		{
			cout << "������� �������� �������: ";
			cin >> label;
			index = 0;
			Vertex* tempVar = new Vertex(index, label);
			for (int i = 0; i < vertexs.size(); i++)
			{
				if (vertexs[i]->label == tempVar->label)
				{
					Edge* tempVar1 = new Edge(vertexs[i]->index, b, w);
					for (int j = 0; j < edges.size(); j++)
					{
						if (edges[j]->u == tempVar1->u)
						{
							stemp = edges[j];
							DELedges.push_back(*stemp);
							edges.erase(edges.begin() + j);
						}
						if (edges[j]->v == tempVar1->u)
						{
							stemp = edges[j];
							DELedges.push_back(*stemp);
							edges.erase(edges.begin() + j);
						}
					}
					index = vertexs[i]->index;
					vertexs.erase(vertexs.begin() + i);
				}
			}
		}
		cout << "������� " << label << " ��� �������� " << index << " - ������A!" << endl;

		if (DELedges.size() != 0) {
			cout << "����� ������� " << DELedges.size() << " ���: " << endl;
			for (vector<Edge>::iterator ITER = DELedges.begin(); ITER != DELedges.end(); ITER++)
				cout << "                     " << ITER->U() << " - " << ITER->V() << endl;
		}
	}
}
void Adder::ChangeEdge(vector<Edge*>& edges, int flag) 
{
	cout << "������� ����, � ������� ������ �������� ��� (1 2): ";
	cin >> a >> b;
	cout << endl;
	cout << "������� ���: ";
	cin >> w;
	int tempW;
	Edge* tempVar = new Edge(a, b, w);
	for (int i = 0; i < edges.size(); i++)
	{
		if (edges[i]->u == tempVar->u && edges[i]->v == tempVar->v)
		{
			tempW = edges[i]->weight;
			edges[i]->weight = w;
		}
	}
	cout << endl << "���� " << a << " -> " << b << " ���� � ����� " << tempW << " ����� � ����� " << w << endl;
}
void Adder::ChangeEdge(vector<Edge*>& edges)
{
	cout << "������� ����, � ������� ������ �������� ����������� (1 2): ";
	cin >> a >> b;
	w = 0;
	int temp;
	Edge* tempVar = new Edge(a, b, w);
	for (int i = 0; i < edges.size(); i++)
	{
		if (edges[i]->u == tempVar->u && edges[i]->v == tempVar->v)
		{
			w = i;
			temp = edges[i]->u;
			edges[i]->u = edges[i]->v;
			edges[i]->v = temp;
		}
	}
	cout << endl << "���� " << a << " -> " << b << " ���� " << edges[w]->u << " -> " << edges[w]->v << endl;
}
void Adder::ChangeVertex(vector<Vertex*>& vertexs)
{
	cout << "�������  �������� �������, ������� ������ �������������: ";
	cin >> label;
	cout << endl;
	cout << "������� ����� ��������: ";
	string newlabel;
	cin >> newlabel;

	Vertex* tempVar = new Vertex(1, label);
	for (int i = 0; i < vertexs.size(); i++)
	{
		if (vertexs[i]->label == tempVar->label)
		{
			w = i;
			vertexs[i]->label = newlabel;
		}
	}
	cout << "������� � ��������� " << label << " � �������� " << vertexs[w]->index << " ���� ������������� � " << vertexs[w]->label << endl;
}
void Adder::ChangeVertex(vector<Vertex*>& vertexs, vector<Edge*>& edges)
{
	cout << "������� �������� �������, � ������� ������ �������� ������: ";
	cin >> label;
	cout << endl;
	cout << "������� ����� ������: ";
	int newindex;
	cin >> newindex;
	cout << endl;

	Vertex* tempVar = new Vertex(1, label);
	Vertex* tempVar1 = new Vertex(newindex, "1");

	for (int i = 0; i < vertexs.size(); i++)
	{
		if (vertexs[i]->label == tempVar->label)
			tempVar->index = vertexs[i]->index;
	}

	bool flags = 0;

	for (int i = 0; i < vertexs.size(); i++)
	{
		if (vertexs[i]->index == tempVar1->index)
		{
			flags = 1;
			tempVar1->label = vertexs[i]->label;
			break;
		}
	}
	if (flags)
	{
		bool q;
		cout << "������� ������� � ����� " << tempVar1->label << " � ����� �� ��������!" << endl;
		cout << "������� ����� ���������! (��� ���� ����� ���������� � " << label << " � ����� �������� " << newindex << " ), � " << tempVar1->label  << " ����� �������!" << endl;
		cout << "����������? 1 - ��, 0 - ���: ";
		cin >> q;
		cout << endl;
		if (q)
		{
			vector<Edge> BReplaceedges;
			vector<Edge*> AReplaceedges;
			Edge* stemp = new Edge(0, 0, 0);
			for (int j = 0; j < edges.size(); j++)
			{
				if (edges[j]->u == tempVar->index) 
				{
					stemp = edges[j];
					BReplaceedges.push_back(*stemp);
					edges[j]->u = tempVar1->index;
					AReplaceedges.push_back(edges[j]);
				}
				if (edges[j]->v == tempVar->index) 
				{
					stemp = edges[j];
					BReplaceedges.push_back(*stemp);
					edges[j]->v = tempVar1->index;
					AReplaceedges.push_back(edges[j]);
				}
			}
			for (int i = 0; i < vertexs.size(); i++)
			{
				if (vertexs[i]->index == newindex)
					vertexs[i]->label = label;
			}
			for (int i = 0; i < vertexs.size(); i++)
			{
				if (vertexs[i]->label == tempVar1->label)
					vertexs.erase(vertexs.begin() + i);
			}
			cout << "�������" << label << " ���� � �������� " << tempVar->index << " ����� � " << tempVar1->index << endl;
			cout << "�������" << tempVar1->label << " � �������� " << tempVar1->index << " �������! " << endl;

			if (BReplaceedges.size() != 0) 
			{
				cout << "���� �������� " << BReplaceedges.size() << " ���" << endl;
				vector<Edge>::iterator ITER2 = BReplaceedges.begin();
				for (vector<Edge*>::iterator ITER = AReplaceedges.begin(); ITER != AReplaceedges.end(); ITER++, ITER2++)
					cout << ITER2->U() << " - " << ITER2->V() << "==>" << (*ITER)->U() << " - " << (*ITER)->V() << endl;
			}
		}
	}
	else
	{
		for (int j = 0; j < vertexs.size(); j++)
		{
			if (vertexs[j]->index == tempVar->index)
			{
				w = j;
				vertexs[j]->index = newindex;
			}
		}
		cout << "� ������� � ��������� " << tempVar->label << " � �������� " << tempVar->index << " ��� ������� ������ �� " << vertexs[w]->index << endl;
	}
}

class MainMenu : public DATA
{
private:
	int flag, check;
	vector<int> Paph;
public:
	Graph graph;
	ShowData show;
	Adder add;
	void menu()
	{
		cout << "      ________________________________________________________________________________________________________" << endl;
		cout << "     |                                                                                                        |" << endl;
		cout << "     |                                                  ����                                                  |" << endl;
		cout << "     |________________________________________________________________________________________________________|" << endl;
		cout << "     |                                                                                                        |" << endl;
		cout << "     |       (1) ��������� ���� �� �����              (7) ������� ����                                        |" << endl;
		cout << "     |       (2) ������� ����                         (8) �������� ����                                       |" << endl;
		cout << "     |       (3) �������� �������                     (9) ����� ��� ������� ���� �� A �� B                    |" << endl;
		cout << "     |       (4) ������� �������                      (10) ������� FIRST(v), NEXT(v, i), VERTEX(v, i)         |" << endl;
		cout << "     |       (5) �������� �������                     (11) ��������� ������� ����                             |" << endl;
		cout << "     |       (6) �������� ����                        (12) ������� ������� ����                               |" << endl;
		cout << "     |                                                                                                        |" << endl;
		cout << "     |                                        (13) ����� �� ���������                                         |" << endl;
		cout << "     |________________________________________________________________________________________________________|" << endl;
	}
	void input_item()
	{
		menu();
		cout << endl;
		cout << "�������� ��������: ";
		cin >> flag;

		if (flag <= 0 or flag > 13) {
			cout << "������� �������� ��������. ��������� ����!" << endl;
			system("pause"); system("cls");
			input_item();
		}
		if (flag == 1)
		{
			Files file;
			string FILENAME;
			cout << "������� ��� �����: ";
			cin >> FILENAME;
			check = file.ReadFromFile(FILENAME, vertexs, edges);
			if (check == -1)
				cout << "������ ������ �����!" << endl;
			system("pause"); system("cls");
			input_item();
		}
		if (flag == 2)
		{
			if (!vertexs.empty())
			{
				cout << "������� 1 - ������ ���" << endl;
				cout << "        2 - ������ ����� ���" << endl;
				cout << "        3 - ������ ������ � ��������: ";
				cin >> flag;
				cout << endl;
				if (flag == 1 or flag == 2)
				{
					if (!edges.empty())
						show.Print(edges, vertexs, flag);
					else
						cout << "� ����� ��� ���!" << endl;
				}
				else if (flag == 3)
				{
					show.Print(vertexs);
				}
				else
				{
					cout << "������� �������� ��������. ��������� ����!" << endl;
					system("pause"); system("cls");
					input_item();
				}
			}
			else cout << "���� ����! (��� �� ����� �������)" << endl; system("pause"); system("cls");
			input_item();
		}
		if (flag == 3)
		{
			add.ADVertex(vertexs, edges, flag);
			system("pause"); system("cls");
			input_item();
		}
		if (flag == 4)
		{
			if (!vertexs.empty()) {
				add.ADVertex(vertexs, edges, flag);
			}
			else cout << "� ����� ��� ������!" << endl; system("pause"); system("cls");
			input_item();
		}
		if (flag == 5)
		{
			if (!vertexs.empty()) {
				cout << "�������� 1 - ��������" << endl;
				cout << "         2 - ������: ";
				cin >> flag;
				if (flag == 1)
					add.ChangeVertex(vertexs);
				else if (flag == 2)
					add.ChangeVertex(vertexs, edges);
				else
				{
					cout << "������� �������� ��������. ��������� ����!" << endl;
					system("pause"); system("cls");
					input_item();
				}
			}
			else cout << "���� ������! � ����� ��� ������." << endl; system("pause"); system("cls");
			input_item();
		}
		if (flag == 6)
		{
			add.ADEdge(vertexs, edges, flag);
			system("pause"); system("cls");
			input_item();
		}
		if (flag == 7)
		{
			if (!edges.empty()) {
				add.ADEdge(vertexs, edges, flag);
			}
			else cout << "� ����� ��� ���!" << endl; system("pause"); system("cls");
			input_item();
		}
		if (flag == 8)
		{
			if (!edges.empty()) {
				cout << "�������� 1 - ����������� ����" << endl;
				cout << "         2 - ��� ����: ";
				cin >> flag;
				if (flag == 1)
					add.ChangeEdge(edges);
				else if (flag == 2)
					add.ChangeEdge(edges, 1);
				else
				{
					cout << "������� �������� ��������. ��������� ����!" << endl;
					system("pause"); system("cls");
					input_item();
				}
			}
			else cout << "� ����� ��� ���!" << endl; system("pause"); system("cls");
			input_item();
		}
		if (flag == 9)
		{
			if (!edges.empty()) {
				cout << "������� ���� �� A �� B (����� ������): ";
				int a, b;
				cin >> a >> b;
				graph.Ways(edges, a, Paph, listWays, b);
				if (!listWays.empty()) {
					show.Print(listWays, vertexs);
					ClearWays();
				}
				else cout << "����� �� " << a << " � " << b << " �� ����������!" << endl;
			}
			else cout << "� ����� ��� ���!" << endl; system("pause"); system("cls");
			input_item();
		}
		if (flag == 10)
		{
			Functions fun(edges, vertexs, listWays);
			if (!vertexs.empty()) {
				cout << "                  1 - FIRST(v), ������� ���������� ������ ������ ������� ������� � �����" << endl;
				cout << "                  2 - NEXT(v, i), ������� ���������� ������ �������, ������� � �������� v, ��������� �� �������� i" << endl;
				cout << "                  3 - VERTEX(v, i), ������� ���������� ������� � �������� i �� ��������� ������, ������� � v" << endl;
				cout << "�������� �������: "; cin >> flag;
				cout << endl;
				if (flag == 1) 
				{
					string label;
					cout << "������� �������� �������: "; cin >> label;
					if (fun.FIRST(label) == -1)
						cout << "����� ������� ��� � �����!" << endl;
					else if (fun.FIRST(label) == -2)
						cout << "� ������� ��� ������� ������!" << endl;
					else
						cout << "�������, ������� � " << label << " ����� ������ " << fun.FIRST(label) << endl;
				}
				else if (flag == 2) 
				{
					string label;
					cout << "������� �������� �������: "; cin >> label;
					cout << endl;
					int index;
					cout << "������� ������: "; cin >> index;
					cout << "������ �������, ������� � �������� " << label << ", ��������� �� �������� " << index << " ����� " << fun.NEXT(label, index) << endl;
				}
				else if (flag == 3) 
				{
					string label;
					cout << "������� �������� �������: "; cin >> label;
					cout << endl;
					int index;
					cout << "������� ������: "; cin >> index;
					cout << "������� " << fun.VERTEX(label, index) << " � �������� " << index << " �� ��������� ������, ������� � " << label << endl;
				}
				else
				{
					cout << "������� �������� ��������. ��������� ����!" << endl;
					system("pause"); system("cls");
					input_item();
				}
			}
			else cout << "� ����� ��� ������!" << endl; system("pause"); system("cls");
			input_item();
		}
		if (flag == 11)
		{
			if (!vertexs.empty()) {
				Files file;
				string FILENAME;
				cout << "������� ��� ������ �����: ";
				cin >> FILENAME;
				file.WriteFromFile(FILENAME, vertexs, edges);
			}
			else cout << "� ����� ��� ������!" << endl; system("pause"); system("cls");
			input_item();
		}
		if (flag == 12)
		{
			edges.clear();
			vertexs.clear();
			if (vertexs.empty() && edges.empty())
				cout << "������� ���� ������!" << endl;
			else
				cout << "ERROR: ��������� ������ ��������!" << endl;

			system("pause"); system("cls");
			input_item();
		}
		if (flag == 13)
			exit(0);
	}
	~MainMenu() override
	{
		vector<int>().swap(Paph);
		Paph.shrink_to_fit();
		graph.~Graph();
		show.~ShowData();
		add.~Adder();
	}
};

void main()
{
	setlocale(LC_ALL, "RUS");
	MainMenu start;
	start.input_item();
	system("pause");
}