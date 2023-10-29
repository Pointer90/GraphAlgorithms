#include <iostream>
#include <vector>
#include <set>
#include <string>
#include <fstream>
#include <stack>

using namespace std;

/*
*+-------------------------------------------------------------- +
*|                     Ввод / Вывод данных                       |
*+-------------------------------------------------------------- +
*/

vector<vector<int>> input(const string path) {
	/* file - файл для чтения
	*  matrix - матрица смежности
	*  value - вспомогательный вектор, в который кладу значения из считаной строки
	*  line - считываемая строка
	*/

	ifstream file;
	vector<vector<int>> matrix;
	vector<int> value;
	string line;
	int capacity = 0;

	// Устанавливаю кодировку и открываю файл.
	system("chcp 1251");
	file.open(path);

	if (file.is_open()) {
		getline(file, line);
		capacity = stoi(line);

		while (getline(file, line)) {
			for (int i = 0; i < capacity; i++) {
				/* Убираю начальные пробелы
				* end = индекс, где начинается пробел
				* преобразую подстроку найденного числа в целое число и кладу в вспомогательный вектор
				* отрезаю найденный кусок от главной строки
				*/

				line = line.replace(0, line.find_first_not_of(" "), "");
				int end = line.find(" ", 0);
				value.push_back(stoi(line.substr(0, end)));
				line = line.substr(end, line.size());
			}
			//Кладу готовые значения в матрицу и отчищаю вспомогательный вектор для считывания новой строки.

			matrix.push_back(value);
			value.clear();
		}

		cout << "Чтение файла успешно завершено.\n";
	}
	else cout << "Не удалось открыть файл для чтения!\n";

	return matrix;
}


void out(const vector<int>& result, const int minWeight) {
	fstream file;

	file.open("../Output.txt");

	if (file.is_open()) {
		for (int i = 0; i < result.size() - 1; i++) file << " (" << result[i] << " — " << result[i + 1] << "), " << endl;
		printf("\n\n           Weight: %d \n\n\n", minWeight);
	}
	else {
		cout << "Файл не удалось открыть!";
	}
}
/*
* +----------------------------------------------+
* |          Алгоритм Брона — Кербоша            |
* +----------------------------------------------+
*/

void BronKerbosh(vector<vector<int>> matrix)
{
	const int capacity = matrix.size();

	set<int> S, Qp, Qm;
	vector<set<int>> result;


	for (int i = 0; i < capacity; i++) Qp.insert(i);

	int v,
		countQm = 0,
		countQp = 0,
		independentsOfGraph = 0;

	stack<int> mas1;
	stack<set<int>> mas2;
	set<int>::iterator itr;

	fstream file;

	file.open("../Output.txt");

	if (file.is_open())
	{

		while (Qp.size() != 0 || S.size() != 0)
		{
			if (Qp.size() != 0)
			{
				itr = Qp.begin();
				v = *itr;
				mas2.push(S);
				mas2.push(Qp);
				mas2.push(Qm);
				mas1.push(v);
				S.insert(v);

				for (int i = 0; i < capacity; i++)
				{
					if (matrix[v][i])
					{
						itr = Qp.find(i);
						if (itr != Qp.end()) Qp.erase(itr);

						itr = Qm.find(i);
						if (itr != Qm.end()) Qm.erase(itr);
					}
				}

				itr = Qp.find(v);

				if (itr != Qp.end()) Qp.erase(itr);
			}
			else
			{
				if (Qm.size() == 0)
				{
					if (independentsOfGraph < S.size()) independentsOfGraph = S.size();
					for (int elem : S) file << elem << " ";
					file << endl;
				}

				v = mas1.top();
				mas1.pop();

				Qm = mas2.top();
				mas2.pop();

				Qp = mas2.top();
				mas2.pop();

				S = mas2.top();
				mas2.pop();

				itr = Qp.find(v);

				if (itr != Qp.end()) Qp.erase(itr);

				Qm.insert(v);
			}
		}
	}
	else cout << "Файл не удалось открыть!";
	file.close();
	printf("\n\n              Independents of Graph: %d \n\n", independentsOfGraph);
}

int main()
{
	vector<vector<int>> matrix = input("../test2.txt");
	BronKerbosh(matrix);
}
