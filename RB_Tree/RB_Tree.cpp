#include <iostream>
#include <set>
#include <stdlib.h>
#include <time.h>
#include "RB_Tree.h"
using namespace ddwl;
using namespace std;
int main() {
	Myset<int> rbt;
	set<int> sb;
	set<int>::iterator it;
	Myset<int>::Iterator Myit;
	int elem1[1000001], elem2[1000001];
	srand((unsigned int)time(NULL));
	clock_t s, e;
	/************************һ�������������� | ���׼set���Ա�*******************************/
	for (int i = 0; i < 1000001; i++) elem1[i] = rand() % 10000001;
	cout << "std::set��\n";
	s = clock();
	for (int i = 0; i < 1000001; i++) sb.insert(elem1[i]);
	e = clock();
	cout << "������ʱ�� " << (double)e - s << "ms��";
	cout << "���ڹ���" << sb.size() << "��Ԫ�ء�\n";
	for (int i = 0; i < 1000001; i++) elem2[i] = (rand() % 10000001) * 3;

	s = clock();
	for (int i = 0; i < 1000001; i++) sb.erase(elem2[i]);
	e = clock();
	cout << "ɾ����ʱ�� " << (double)e - s << "ms��";
	cout << "���ڹ���" << sb.size() << "��Ԫ�ء�\n\n";

	cout << "std::Myset��\n";
	s = clock();
	for (int i = 0; i < 1000001; i++) rbt.Insert(elem1[i]);
	e = clock();
	cout <<"������ʱ�� "<<(double)e - s << "ms��";
	cout << "���ڹ���" << rbt.size() << "��Ԫ�ء�\n";
	
	s = clock();
	for (int i = 0; i < 1000001; i++) rbt.erase(elem2[i]);
	e = clock();
	cout << "ɾ����ʱ�� " << (double)e - s << "ms��";
	cout << "���ڹ���" << rbt.size() << "��Ԫ�ء�\n\n";

	for (it = sb.begin(); it != sb.end(); it++);
	for (Myit = rbt.begin(); Myit != rbt.end(); Myit++);
	system("pause");
	return 0;
}