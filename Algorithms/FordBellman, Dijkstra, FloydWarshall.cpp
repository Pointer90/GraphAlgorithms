#include <iostream>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

vector<vector<int>> adjacencyMatrix;
int capacity, startVertex;

void inp(string path) {
	/* Инициализация файла, строки считывания и
	* Вспомогательного вектора, в который буду класть целые числа из строки.
	*/
	ifstream file;
	string str;
	vector<int> value;

	// Открываю файл по заданной директории
	file.open(path);

	// Если файл удалось открыть начинаю считывать данные, иначе информирую об ошибке.
	if (file.is_open()) {
		cout << "Файл успешно открыт!\n\n";
		// Считываю первую строку с количеством вершин и инициализирую count = количество вершин
		getline(file, str);
		capacity = stoi(str);
		// Считываю построчно все остальные данные
		while (getline(file, str)) {

			for (int i = 0; i < capacity; i++) {
				/* Убираю начальные пробелы
				* end = индекс, где начинается пробел
				* преобразую подстроку найденного числа в целое число и кладу в вспомогательный вектор
				* отрезаю найденный кусок от главной строки*/

				str = str.replace(0, str.find_first_not_of(" "), "");
				int end = str.find(" ", 0);
				value.push_back(stoi(str.substr(0, end)));
				str = str.substr(end, str.size());
			}
			adjacencyMatrix.push_back(value);
			value.clear();
		}
	}
	else {
		cout << "Не удалось открыть файл!\n\n";
	}
	//Закрываю файл
	file.close();
}

void out(int result[]) {
	fstream file;

	file.open("../Output.txt");

	if (file.is_open()) {
		file.clear();
		file << "Кратчайшие пути:\n";
		for (int i = 0; i < capacity; i++) {
			result[i] <= INT_MAX / 2 ? file << startVertex << " = > " << i << " = " << result[i] << endl : file << startVertex << " => " << i << " = " << "inf " << endl;
		}
	}
	else {
		cout << "Файл не удалось открыть!";
	}
}

void out(vector<vector<int>> result) {
	fstream file;

	file.open("../Output.txt");

	if (file.is_open()) {
		file.clear();
		for (int i = 0; i < capacity; i++) {
			for (int j = 0; j < capacity; j++) {
				result[i][j] <= INT_MAX / 4 ? file << result[i][j] << " " : file << "inf ";
			}
			file << "\n";
		}
	}
	else {
		cout << "Файл не удалось открыть!";
	}
}

struct Edge {
	int x, y, weight;
	Edge(int x, int y, int weight);
};

Edge::Edge(int newX, int newY, int newWeight) {
	x = newX;
	y = newY;
	weight = newWeight;
}

void FordBellman(int startVertex) {
	int* D = new int[capacity];
	bool isNegativeСycle = false;
	vector<Edge> arrayEdge;

	for (int i = 0; i < capacity; i++) {
		for (int j = 0; j < capacity; j++) {
			if (adjacencyMatrix[j][i] != 0) arrayEdge.push_back(Edge(i, j, adjacencyMatrix[j][i]));
		}
	}

	int countEdges = arrayEdge.size();

	for (int i = 0; i < capacity; i++) D[i] = INT_MAX / 2;
	D[startVertex] = 0;

	for (int i = 0; i < capacity - 1; i++) {
		for (int j = 0; j < countEdges; j++) {
			D[arrayEdge[j].y] = min(D[arrayEdge[j].y], D[arrayEdge[j].x] + arrayEdge[j].weight);
		}
	}

	for (int i = 0; i < capacity; i++) {
		if (i != capacity - 1) {
			for (int j = 0; j < countEdges; j++) D[arrayEdge[j].y] = min(D[arrayEdge[j].y], D[arrayEdge[j].x] + arrayEdge[j].weight);
		}

		if (i == capacity - 1) {
			for (int j = 0; j < countEdges; j++)
				if (D[arrayEdge[j].y] > D[arrayEdge[j].x] + arrayEdge[j].weight) isNegativeСycle = true;
		}
	}

	if (isNegativeСycle) {
		fstream file;
		file.open("../Output.txt");
		file.clear();
		file << "Содержит отрицательные циклы\n";
		file.close();
	} else {
		out(D);
	}
}

void Dijkstra(int startVertex) {
	/*Считываю матрицу смежности,
	* инициализирую размерность массива (беру размер первого подвектора, так как матрица все равно N x N)
	* D = массив минимальных расстояний до вершин
	* V = массив пройденных / не пройденных вершин
	* w = переменная, которая содержит в себе индекс последнего добавленного элемента в массив V
	*/
	int capacity = adjacencyMatrix[0].size();
	int* D = new int[capacity];
	bool* V = new bool[capacity];
	int w;

	// Заполняю массив D максимально возможными значениями и в V выставляю значения всех вершин как не пройденные.
	for (int i = 0; i < capacity; i++) {
		D[i] = INT_MAX;
		V[i] = false;
	}

	// Расстояние от введенной вершины до самой себя = 0.
	D[startVertex] = 0;

	for (int j = 0; j < capacity - 1; j++)
	{
		// минимальный возможный вес ребра.
		int minW = INT_MAX;

		//Нахожу минимальный вес ребра, который ещё не пройден и индекс этого ребра.
		for (int i = 0; i < capacity; i++)
			if (!V[i] && D[i] <= minW)
			{
				minW = D[i];
				w = i;
			}

		//Отмечаю индекс минимального ребра пройденной вершиной.
		V[w] = true;

		//Вычисляю кратчайший путь.
		for (int i = 0; i < capacity; i++) {
			if (!V[i] && adjacencyMatrix[w][i] && D[w] != INT_MAX) D[i] = min(D[i], D[w] + adjacencyMatrix[w][i]);
		}
	}

	out(D);
}

void FloydWarshall() {

	vector<vector<int>> resultMatrix = adjacencyMatrix;

	for (int i = 0; i < capacity; i++)
		for (int j = 0; j < capacity; j++) {
			if (resultMatrix[i][j] == 0 and i != j) resultMatrix[i][j] = INT_MAX / 2;
		}

	for (int k = 1; k < capacity; k++)
		for (int i = 0; i < capacity; i++)
			for (int j = 0; j < capacity; j++)
				if (i != j) {
					int d = min(resultMatrix[i][j], resultMatrix[i][k] + resultMatrix[k][j]);
					resultMatrix[i][j] = d;
				}
	out(resultMatrix);
}

int main()
{
	system("chcp 1251");
	cout << "Введите вершину: ";
	cin >> startVertex;

	string myPath = "../test5.txt";
	inp(myPath);

	FordBellman(startVertex);
	//Dijkstra(startVertex);
	//FloydWarshall();
}
