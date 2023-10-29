#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stack>

using namespace std;

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

void out(stack<int> result) {
    fstream file;

    file.open("../Output.txt");

    if (file.is_open()) {
        file.clear();
        while (!result.empty()) {
            file << result.top() << " -> ";
            result.pop();
        }
    }
    else {
        cout << "Файл не удалось открыть!";
    }
}

/*
+--------------------------------------------+
|   Задание №1 Проверка графа на связность   |
+--------------------------------------------+
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
    for (int i = 0; i < capacity; i++) {
        if (mas[i] != 3) {
            cout << "\n The graph is not connected \n";
            return false;
        } 
    }

    cout << "\n The connected Graph \n";
    return true;
}

/*
+-----------------------------------------------+
|   Задание №2 Проверка графа на Эйлеров цикл   |
+-----------------------------------------------+
*/

bool CheckGraphElier(const vector<vector<int>> matrix){
    /*  capacity - размер входного графа
    *   copyMatrix - копия входного графа, чтобы не портить оригинал
    *   visited - стек посещенных вершин
    *   result - стек вершин эйлерова цикла
    *   degreeVertex - степень вершины
    */

    int capacity = matrix.size();
    vector<vector<int>> copyMatrix = matrix;
    stack<int> result;
    stack<int> visited;
    int DegreeVertex;

    //Проверка на четность вершин

    for (int i = 0; i < capacity; i++) {
        DegreeVertex = 0;
        for (int j = 0; j < capacity; j++) {
            if (matrix[i][j] != 0) DegreeVertex++;
        }
        if (DegreeVertex % 2 == 1) {
            cout << "Граф не Эйлеров";
            return false;
        }
    }

    //Выбираю любую вершину в качестве начальной, в частности 0
    visited.push(0);

    //Пока посещенные вершинки не опустеют
    while (!visited.empty()) {
        /*
        *   is_empty - флаг, который проверяет пуста ли строка / столбец в матрице смежности
        *   i - индекс крайней вершинки, с которой нужно рассматривать следующую вершинку
        */

        bool is_empty = true;
        int i = visited.top();

        // Ищу не нулевой элемент в строки i с индексом. Если нахожу, то drop из цикла и все заново, иначе строка пуста и перехожу к if 
        for (int j = 0; j < capacity; j++) {
            if (copyMatrix[i][j] != 0) {
                // Если не пустой добавляю в стек visited и удаляю элемент из матрицы — drop из цикла.
                is_empty = !is_empty;
                visited.push(j);
                copyMatrix[i][j] = 0;
                copyMatrix[j][i] = 0;
                break;
            }
        }

        // Если у вершинки не оказалось ребер, то переливаю её из visited в result
        if (is_empty) {
            result.push(visited.top());
            visited.pop();
        }
    }

    // Печать эйлерова цикла
    out(result);

    return true;
}

/*
+--------------------------------------------------------------------------------+
|   Задание №2 Проверка графа на Эйлеров цикл через "Алгоритм на основе циклов"  |
+--------------------------------------------------------------------------------+
*/

stack<int> result;

//Рекурсия, которая ищет индекс
void findIndex(int index, vector<vector<int>>& copyMatrix) {
    /*
    *   is_empty - флаг, который проверяет пуста ли строка / столбец в матрице смежности
    *   index - индекс крайней вершинки, с которой нужно рассматривать следующую вершинку
    */
    bool isEmpty = false;
    for (int j = 0; j < copyMatrix.size(); j++) {
        if (copyMatrix[index][j] != 0) {
            isEmpty = !isEmpty;
            copyMatrix[index][j] = 0;
            copyMatrix[j][index] = 0;
            findIndex(j, copyMatrix);
        }
    }

    result.push(index);
}

//Точка входа в рекурсию, в которой еще проверяется на Эйлеровость
bool CycleBasedAlgorithm(const vector<vector<int>> matrix) {
    /*  capacity - размер входного графа
    *   copyMatrix - копия входного графа, чтобы не портить оригинал
    *   degreeVertex - степень вершины
    */
    int capacity = matrix.size();
    vector<vector<int>> copyMatrix = matrix;
    int DegreeVertex;

    //Проверка на четность вершин

    for (int i = 0; i < capacity; i++) {
        DegreeVertex = 0;
        for (int j = 0; j < capacity; j++) {
            if (matrix[i][j] != 0) DegreeVertex++;
        }
        if (DegreeVertex % 2 == 1) {
            cout << "Граф не Эйлеров";
            return false;
        }
    }

    //Запускаю рекурсию
    findIndex(0, copyMatrix);

    //Печать результата
    out(result);
}

int main()
{
    vector<vector<int>> matrix = input("../test1.txt");
    //CheckGraphConnect(matrix);
    //CheckGraphElier(matrix);
    CycleBasedAlgorithm(matrix);
}
