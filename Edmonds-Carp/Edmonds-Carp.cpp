#include <iostream>
#include <vector>
#include <fstream>
#include <string>

const int inf = INT_MAX / 4;
#define ctrl(x) (x > 0 ? x : inf)

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

void out(const vector<vector<int>>& result) 
{
    ofstream file;

    file.open("../Output.txt");

    if (file.is_open()) 
    {
        for (int i = 0; i < result.size(); i++)
        {
            for (int j = 0; j < result.size(); j++) file << result[i][j] << " ";
            file << endl;
        }
    }
    else cout << "Файл не удалось открыть!";
}

// минимальный путь от стока к истоку - алгоритм Дейкстры
bool min_way(vector < vector < int > >& D, vector < int >& w, vector < int >& p)
{
    int i, j;
    int n = D.size();
    vector < int > a(n, 0);
    p.assign(n, 0);
    w.assign(n, inf);
    for (i = 0; i < n; ++i)
        w[i] = ctrl(D[0][i]);
    a[0] = 1;
    w[0] = 0;
    p[0] = -1;

    for (i = 0; i < n - 2; ++i)
    {
        int Min = 2 * inf, iMin = -1;
        for (j = 0; j < n; ++j)
            if (!a[j] && w[j] < Min)
            {
                Min = w[j];
                iMin = j;
            }

        if (iMin < 0) return  false;
        a[iMin] = 1;

        for (j = 0; j < n; ++j)
            if (w[j] > w[iMin] + ctrl(D[iMin][j]))
            {
                w[j] = w[iMin] + D[iMin][j];
                p[j] = iMin;
            }
    }

    return w[n - 1] < inf / 10;
}
// максимальный поток
void max_flow(vector < vector < int > > C, vector < vector < int > >& F)
{
    int i, j;
    int n = C.size();
    F.assign(n, vector < int >(n, 0));
    vector < vector < int > > Cf = C;
    vector < int > w, p;

    while (min_way(Cf, w, p))
    {
        int s = inf;
        for (i = n - 1; i != 0; i = p[i])
            s = min(s, Cf[p[i]][i]);

        for (i = n - 1; i != 0; i = p[i])
        {
            int u = p[i];
            int v = i;
            F[u][v] += s;
            F[v][u] = -F[u][v];
            Cf[u][v] = C[u][v] - F[u][v];
            Cf[v][u] = C[v][u] - F[v][u];
        }
    }

    out(F);

    int max = 0;

    for (int i = 0; i < F.size(); i++) max += F[0][i];
    cout << "\n Величина максимального потока: " << max << endl;
}

int main()
{
    vector<vector<int>> graph = input("../6.txt");
    vector<vector<int>> graphNull(graph.size(), vector<int> (graph.size()));
    max_flow(graph, graphNull);
}