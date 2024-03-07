#include <iostream>
#include <vector>
#include <ctime>
#include <string>

#include <fstream>

#include "Windows.h"

using namespace std;

const int INF = INT_MAX;

// Функция для вывода кратчайшего пути
void printPath(int start, int end, vector<vector<int>>& next, string& result_toFile)
{
	if (next[start][end] == -1)
	{
		cout << "Нет пути из " << start + 1 << " в " << end + 1 << endl;
		result_toFile += "Нет пути из " + to_string(start + 1);
		result_toFile += " в " + to_string(end + 1) + '\n';
		return;
	}
	cout << "Путь: " << start + 1 << " в " << end + 1 << ": ";
	result_toFile += "Путь: " + to_string(start + 1);
	result_toFile += " в " + to_string(end + 1);
	cout << "(" << start + 1;
	result_toFile += ": (" + to_string(start + 1);
	while (start != end)
	{
		start = next[start][end];
		cout << " --> " << start + 1;
		result_toFile += " ---> " + to_string(start + 1);
	}
	//cout << endl;
}

// Функция для нахождения кратчайших путей между всеми парами вершин
void floydWarshall(vector<vector<int>>& graph, int V, string& result_toFile)
{
	vector<vector<int>> dist(V, vector<int>(V));
	vector<vector<int>> next(V, vector<int>(V, -1));

	result_toFile = "";

	// Инициализация расстояний и предшественников
	for (int i = 0; i < V; i++)
	{
		for (int j = 0; j < V; j++)
		{
			dist[i][j] = graph[i][j];
			if (graph[i][j] != INF && i != j)
			{
				next[i][j] = j;
			}
		}
	}

	// Обновление кратчайших расстояний и предшественников
	for (int k = 0; k < V; k++)
	{
		for (int i = 0; i < V; i++)
		{
			for (int j = 0; j < V; j++)
			{
				if (dist[i][k] != INF && dist[k][j] != INF && dist[i][k] + dist[k][j] < dist[i][j])
				{
					dist[i][j] = dist[i][k] + dist[k][j];
					next[i][j] = next[i][k];
				}
			}
		}
	}

	// Вывод кратчайших путей
	for (int i = 0; i < V; i++)
	{
		for (int j = 0; j < V; j++)
		{
			if (i != j)
			{
				printPath(i, j, next, result_toFile);
				cout << ") = " << dist[i][j] << endl;
				result_toFile += ") = " + to_string(dist[i][j]) + '\n';
			}
		}
	}
}

void matrixGenerator()
{
	int v = 0;
	do
	{
		cout << "Введите количевство вершин(Укажите 2 или больше): "; cin >> v;
	} while (v < 2);
	srand(time(0));
	cout << endl << "Сгенерированная матрица:" << endl << endl;
	ofstream to_file("matrix.txt");
	to_file << v << endl;
	for (int i = 0; i < v; i++)
	{
		for (int j = 0; j < v; j++)
		{
			if (i == j) { to_file << 0; cout << 0; }
			else if (rand() % 2 == 0) { to_file << -1; cout << -1; }
			else
			{
				int justNumber = 0;
				do
				{
					justNumber = rand() % 25;
				} while (justNumber == 0);
				to_file << justNumber;
				cout << justNumber;
			}

			to_file << ' ';
			cout << ' ';
		}

		to_file << endl;
		cout << endl;
	}
	to_file.close();
}

void seeMatrix()
{
	ifstream from_file("matrix.txt");
	if (!from_file.is_open())
	{
		cerr << "Не удалось открыть файл.";
		system("pause>nul");
	}
	else
	{
		int v = 0, justNumber = 0;
		from_file >> v;
		cout << "Матрица в файле \"matrix.txt\"" << endl << endl << endl;
		cout << "Размерность матрицы: " << v << endl << endl;

		for (int i = 0; i < v; i++)
		{
			for (int j = 0; j < v; j++)
			{
				from_file >> justNumber;
				cout << justNumber << ' ';
			}
			cout << endl;
		}


	}
	from_file.close();
}

int menu()
{
	system("cls");
	int choice;
	cout << "[1] >> Выполнить задание." << endl << endl;
	cout << "[2] >> Создать матрицу." << endl;
	cout << "[3] >> Посмотреть текущую матрицу в файле matrix.txt." << endl << endl << endl;
	cout << "[0] >> Выйти." << endl << endl;
	cout << "Ваш выбор: "; cin >> choice;

	return choice;
}

int main()
{

	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	setlocale(LC_ALL, "Ukr");

	int choice = 0;
	string result_toFile;

	do
	{
		switch (choice = menu())
		{
		case 1:
		{
			system("cls");
			ifstream from_file("matrix.txt");
			if (!from_file.is_open())
			{
				cerr << "Не удалось открыть файл.";
				system("pause>nul");
			}
			else
			{
				// Чтение матрицы смежности из файла
				int V = 0;
				from_file >> V;
				vector<vector<int>> graph(V, vector<int>(V));

				for (int i = 0; i < V; i++)
				{
					for (int j = 0; j < V; j++)
					{
						from_file >> graph[i][j];
						if (graph[i][j] == -1)
						{
							graph[i][j] = INF; // Замена -1 на INF для удобства обработки
						}
					}
				}

				from_file.close();
				floydWarshall(graph, V, result_toFile);
				ofstream to_file("result.txt");
				to_file << result_toFile;
				to_file.close();

				cout << endl;
				system("pause");
			}
		}
		break;
		case 2:
		{
			system("cls");
			matrixGenerator();
			system("pause");
		}
		break;
		case 3:
		{
			system("cls");
			seeMatrix();
			system("pause");
		}
		break;
		default:
			break;
		}
	} while (choice);
	return EXIT_SUCCESS;
}