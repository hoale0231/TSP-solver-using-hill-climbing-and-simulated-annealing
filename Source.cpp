#include <iostream>
#include <vector>
#include <ctime>
#include <iomanip>
#include <climits>
#include <cmath>
#include <utility>
#include <algorithm>
using namespace std;
class Map
{
	vector<vector<int>> distance;
public:
	Map(int n = 8) {
		distance.resize(n);
		for (size_t i = 0; i < n; i++)
		{
			distance[i].resize(n);
			for (size_t j = i; j < n; j++)
			{
				if (i == j) distance[i][j] = 0;
				else distance[i][j] = rand() % 10000 + 1;
			}
		}
		for (size_t i = 0; i < n; i++)
		{
			for (size_t j = i; j < n; j++)
			{
				distance[j][i] = distance[i][j];
			}
		}
	}
	void printDistance() {
		for (size_t i = 0; i < distance.size(); i++)
		{
			for (size_t j = 0; j < distance[i].size(); j++)
			{
				cout << setw(4) << distance[i][j] << " ";
			}
			cout << endl;
		}
	}
	int result(const vector<int> &arr) {
		int rs = 0;
		for (size_t i = 0; i < distance.size() - 1; i++)
		{
			rs += distance[arr[i]][arr[i + 1]];
		}
		rs += distance[arr[0]][arr[distance.size() - 1]];
		return rs;
	}
	int Delta(vector<int> &arr, int i, int j) {
		int beforei = (i - 1 < 0) ? arr.size() - 1: i - 1;
		int afteri = (i + 1 == arr.size()) ? 0 : i + 1;
		int beforej = (j - 1 < 0) ? arr.size() - 1: j - 1;
		int afterj = (j + 1 == arr.size()) ? 0 : j + 1;
		int beforeSwap = distance[arr[beforei]][arr[i]] + distance[arr[i]][arr[afteri]] + distance[arr[beforej]][arr[j]] + distance[arr[j]][arr[afterj]];
		swap(arr[i], arr[j]);
		int afterSwap = distance[arr[beforei]][arr[i]] + distance[arr[i]][arr[afteri]] + distance[arr[beforej]][arr[j]] + distance[arr[j]][arr[afterj]];
		swap(arr[i], arr[j]);
		return afterSwap - beforeSwap;
	}
	int HillClimbing(bool print = false, bool greedy_ = false) {
		vector<int> arr;

		if (greedy_)
			arr = greedy();
		else
			for (size_t i = 0; i < distance.size(); i++)
			{
				arr.push_back(i);
			}
		int bestResult = this->result(arr);
		if (print) printArr(arr, bestResult);
		for (size_t i = 0; i < distance.size(); i++)
		{
			for (size_t j = i + 1; j < distance.size(); j++) {
				int delta = Delta(arr, i, j);
				if (delta < 0) {
					swap(arr[i], arr[j]);
					bestResult += delta;
					if (print) printArr(arr, bestResult);
					i = -1;
					break;
				}
			}
		}
		return bestResult;
	}
	int SimulatedAnnealing(bool greedy_ = false) {
		vector<int> arr;

		if (greedy_)
			arr = greedy();
		else
			for (size_t i = 0; i < distance.size(); i++)
			{
				arr.push_back(i);
			}
		int bestResult = this->result(arr);
		//cout << s << "  " << bestResult << endl;

		for (double T = 1000; T > 0.00001; T *= 0.9999) {
			for (int r = 0; r < 10; r++) {
				int i = rand() % distance.size();
				int j = rand() % distance.size();
				while (i == j) {
					j = rand() % distance.size();
				}
				int delta = Delta(arr, i, j);

				if (delta < 0 || exp(-(double)delta / T) >= getRand()) {
					bestResult += delta;
					swap(arr[i], arr[j]);
				}
			}
		}
		return bestResult;
	}

	vector<int> greedy() {
		vector<int> result;
		int min = 0;
		int pos = 0;
		int current = 0;
		bool* visited = new bool[distance.size()];
		for (size_t i = 0; i < distance.size(); i++)
		{
			visited[i] = false;
		}
		for (size_t i = 0; i < distance.size(); i++)
		{
			visited[current] = true;
			result.push_back(current);
			min = INT_MAX;
			for (size_t j = 0; j < distance.size(); j++)
			{
				if (!visited[j] && distance[current][j] < min) {
					min = distance[current][j];
					pos = j;
				}
			}
			current = pos;
		}
		delete[] visited;
		return result;
	}

	int bruteForce()
	{
		vector<int> arr;
		pair<int, vector<int>> bestPath; //first element is the length of the best path, second one is the vector that saves the path
		for (size_t i = 0; i < distance.size(); i++)
		{
			arr.push_back(i);
		}
		bestPath.first = INT_MAX;
		do
		{
			int pathLen = result(arr);
			//printArr(arr, pathLen);
			if (pathLen < bestPath.first)
			{
				bestPath.first = pathLen;
				bestPath.second = arr;
			}
		} while (next_permutation(arr.begin(), arr.end()));
		cout << "Best path found: ";
		printArr(bestPath.second, bestPath.first);
		return bestPath.first;
	}
	~Map() {};

private:
	void printArr(vector<int> arr, int result) {
		for (size_t i = 0; i < arr.size(); i++)
		{
			cout << arr[i];
			if (i != arr.size() - 1) cout << "->";
		}
		cout << " " << result << endl;
	}
	double getRand() {
		return (double)rand() / RAND_MAX;
	}

	//static int getPos(char c) {
	//	return (int)c - 65;
	//}
	//static char getChar(int i) {
	//	return (char)(i + 65);
	//}
};


int main() {
	srand(clock());
	int countHill = 0;
	int countSA = 0;
	int n = 10;
	Map m(10);
	m.bruteForce();
	int x = m.HillClimbing();
	cout << "Hill Climbing: " << x << endl;
	int z = m.HillClimbing(false, true);
	cout << "Hill Climbing greedy: " << z << endl;
	for (int i = 0; i < 10; i++)
	{
		cout << "====================================" << endl;
		cout << "Simulated Annealing: " << m.SimulatedAnnealing() << endl;
		cout << "Simulated Annealing greedy: " << m.SimulatedAnnealing(true) << endl;
	}


	/*if (z < x) x = z;
	for (int i = 0; i < n; i++)
	{

		m.printDistance();

		//cout << "==============================" << endl;
		int y = m.SimulatedAnnealing();
		cout  << "Simulated Annealing: " << y << endl;
		z = m.SimulatedAnnealing(true);
		cout << "Simulated Annealing greedy: " << z << endl;
		if (z < y) y = z;
		if (x < y) {
			countHill++;
		}
		if (y > x) {
			countSA++;
		}
		cout << "===============================================================" << endl;
	}
	cout << "Hill Climbing: " << countHill << endl << "Simulated Annealing: " << countSA;
	//Map m;

	//m.printDistance();
	//m.HillClimbing();
	//m.SimulatedAnnealing();*/

}
