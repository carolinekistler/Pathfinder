#include "pathfinder.h"
#include <iostream>
#include <chrono>



int main()
{
	char map[4][6] = { {'s','g','g','e','e','e'},
						{'e','w','e','e','f','e'},
						{'e','w','e','g','w','w'},
						{'e','e','e','w','b','w'} };
	chrono::high_resolution_clock m_clock;
	long long time = chrono::duration_cast<chrono::milliseconds> (m_clock.now().time_since_epoch()).count();
	
	pathfinder(map);
	printMap(map);

	long long time2 = chrono::duration_cast<chrono::milliseconds> (m_clock.now().time_since_epoch()).count();
	cout << time2 - time << endl;
}
