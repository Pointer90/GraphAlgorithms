#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stack>
#include <algorithm>

using namespace std;

/*
* +--------------------------------------------------------------+
* |                     Ввод/Вывод данных                        |
* +--------------------------------------------------------------+
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
* +--------------------------------------------------------+
* |                Проверка на связность                   |
* +--------------------------------------------------------+
*/

bool CheckGraphConnect(const vector<vector<int>>& matrix) {
    /*
    * capacity - размер матрицы смежности,
    * mas - массив маркеров, где
        1 - неизвестна связность вершины,
        2 - вершина рассмотренна,
        3 - вершина пройдена.
      end - точка останова, если не осталось значений 2 в массиве mas
    */

    int capacity = matrix.size();
    int* mas = new int[capacity];
    bool end = false;

    for (int i = 0; i < capacity; i++) {
        mas[i] = 1;
    }

    //Делаю 0-ую вершинку рассмотренной (может быть любая)
    mas[0]++;

    while (!end) {
        for (int i = 0; i < capacity; i++) {
            if (mas[i] == 2) {
                // Если вершинка уже рассмотрена, то делаю из неё пройденную.
                mas[i]++;
                // Делаю соседей i-ой вершинки рассмотренными.
                for (int j = 0; j < capacity; j++) {
                    // Если она связана и не рассмотрена, то рассматриваю.
                    if (matrix[i][j] != 0 && mas[j] == 1) mas[j]++;
                }
            }
        }

        end = true;

        // Если ещё остались рассматриваемые вершинки, то иду на следующую итерацию.
        for (int i = 0; i < capacity; i++) if (mas[i] == 2) end = false;

    }

    // Если есть хоть одна не пройденная вершинка, то граф не связный.
    for (int i = 0; i < capacity; i++) if (mas[i] != 3) return false;

    return true;
}
/*
* +-------------------------------------------------------+
* |                 Алгоритм Краскала                     |
* +-------------------------------------------------------+
*/

struct Edge {
    int a,                                          // a - начало ребра,
        b,                                          // b - конец ребра,
        w;                                          // w - вес ребра
};

bool cmp(Edge a, Edge b) {                          // Компаратор сравнения весов ребер
    return a.w < b.w;
}

void Kruskal(const vector<vector<int>>& matrix) {

    if (!CheckGraphConnect(matrix)) {                           //Проверка на связность
        cout << "Граф не связен!";
        return;
    }

    vector<Edge> e;                                             // e - массив ребер
    vector<int> result;                                         // result - массив с результатами работы алгоритма
    vector<int> markers(matrix.size());


    int minWeight = 0;                                          // minWeight - вес минимального остовного дерева

    for (int i = 0; i < matrix.size(); i++)                     // Пробегаю по матрице заполняя массив ребер
    {

        for (int j = i; j < matrix.size(); j++)
        {
            if (matrix[i][j] != 0) e.push_back(Edge{ i, j, matrix[i][j] });
        }
    }

    for (int i = 0; i < matrix.size(); i++) markers[i] = i;     // Массив маркеров вершин, где указывается к какому множеству они на данный момент принадлежат
   
    sort(e.begin(), e.end(), cmp);                              // Сортирую массив ребер по весу
    
    
    for (int i = 0; i < e.size(); i++)                          // Пробегаю по массиву ребер
    {
        if (markers[e[i].a] != markers[e[i].b])                 // Если вершинки ребра принадлежат разным множествам, то добавляю вес этого ребра к общему
        {                                                       // заношу сохраняю вершинку в результат.
            minWeight += e[i].w;
            result.push_back(e[i].a);
            result.push_back(e[i].b);


            int oldM = markers[e[i].b],                         // oldM - старый маркер множества, к которому принадлежит ребро
                newM = markers[e[i].a];                         // newM - новый маркер множества, к которому принадлежит ребро

            for (int j = 0; j < matrix.size(); j++) if (markers[j] == oldM) markers[j] = newM;  // Пробегаюсь по массиву маркеров изменяя все старые маркеры на новые, т.е сливаю множества в одн 
        }
    }

    out(result, minWeight);                                     // Вывод ответа
}

/*
* +-------------------------------------------------------+
* |                    Алгоритм Прима                     |
* +-------------------------------------------------------+
*/

void Prim(const vector<vector<int>>& matrix) {

    if (!CheckGraphConnect(matrix)) {               //Проверка на связность
        cout << "Граф не связен!";
        return;
    }

    int capacity = matrix.size(),                   // capacity - размерность входящей матрицы смежности
        countEdges = 0,                             // countEdges - количество просмотренных ребер
        minWeight = 0;                              // minWeight - минимальный вес остовного дерева

    vector<bool> MST(matrix.size(), false);         // MST - массив, в котором содержится информация о том пройдена ли вершинка
    vector<int> result;                             // result - массив с результатами работы алгоритма
    MST[0] = true;

    while (countEdges < capacity - 1) {

        int min = INT_MAX,                          // min - минимальный возможный вес не пройденного ребра
            v1 = -1,                                // v1, v2 - индексы начала и конца найденного минимального ребра
            v2 = -1;

        for (int i = 0; i < capacity; i++) {        // Пробегаю по всей матрице

            if (MST[i]) {                           // Если начальная вершина ребра уже пройдена
                
                for (int j = 0; j < capacity; j++) {

                    if (!MST[j] && matrix[i][j]) {  // Если конечная вершина ребра ещё не пройдена и не имеет 0-вой вес

                        if (matrix[i][j] < min) {   // Запоминаю самый минимальный вес ребра и его индексы начала и конца

                            min = matrix[i][j];
                            v1 = i;
                            v2 = j;
                        }
                    }
                }
            }
        }

        if (v1 != -1 && v2 != -1) {                 // Если индексы изменились увеличиваю количество просомтренных ребер
            result.push_back(v1);                   // добавляю найденный минимальный вес к общему весу дерева и отмечаю,
            result.push_back(v2);                   // что вершинки пройдены, т.е содержатся в мин. остовном дереве.
            countEdges++;                           // Добавляю индексы пройденного ребра в историю добавления.
            minWeight += min;
            MST[v1] = true;
            MST[v2] = true;
        }
    }

    out(result, minWeight);                         // Печать ответа.
}

int main()
{
    vector<vector<int>> matrix = input("../test1.txt");
    //Kruskal(matrix);
    Prim(matrix);
}
