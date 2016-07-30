#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <initializer_list>
#include <vector>
#include <regex>
#include <chrono>
#include <time.h>
#include <random>

using namespace std;
using namespace std::chrono;
int main(int argc, char* argv[])
{	
	random_device rnd;
	cout << rnd.entropy() << endl;
	cout << rnd.min()<< endl;
	cout << rnd.max() << endl;
	system("pause");
	return 0;
}