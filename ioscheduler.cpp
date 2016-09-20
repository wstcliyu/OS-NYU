#include <stack>
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <cstring>
#include <stdlib.h>
#include <iomanip>
#include <string>
#include <sstream>
#include <queue>
#include <vector>
#include <unistd.h>
using namespace std;


//Rachit Mehrotra
//rm4149@nyu.edu

class IO_Operation {
    public:
        int _timestamp;
        int _diskloc;
        int _instNum;
IO_Operation(int t, int l, int i)
{
	_diskloc=l;
	_instNum=i;
	_timestamp=t;
}
};

class IOScheduler {

public:
	vector<IO_Operation> _readyvector;
	virtual IO_Operation getIO_Operation( bool right, int& current) = 0;
	void schedule(vector<IO_Operation> _insvector) 
	{
	int current = 0;
	int previous = 0;
	int total_time = 0;
	
	int tot_wait = 0;
	int max_wait = 0;
	int count = 0;
	int _inscounter = _insvector.size();
	int tot_movement = 0;
	int tot_TA = 0;
	bool found = false;
	IO_Operation instr=IO_Operation(0, 0, 0);
	while (count != _inscounter) 
	{
		if (!found && !_readyvector.empty()) 
		{
			bool diff = current - previous >= 0 ? true : false;
			instr = this->getIO_Operation( diff, current);
			found = true;
			tot_movement += abs(current - instr._diskloc);
			int curr_wait = total_time - instr._timestamp;
			tot_wait += curr_wait;
			if (curr_wait > max_wait) 
				max_wait = curr_wait;
			
		}
		if (found) 
		{
			int arr_time = total_time + abs(current - instr._diskloc);
			while (!_insvector.empty() && _insvector[0]._timestamp <= arr_time) 
			{
				IO_Operation temp = this->addToQueue(_insvector, found);
				total_time = temp._timestamp;
			}
			count++;
			if (instr._diskloc != current) 
			{
				previous = current;
			}
			current = instr._diskloc;
			total_time = arr_time;
			found = false;
			int tt = total_time - instr._timestamp;
			tot_TA += tt;
			
		} 
		else 
		{
			IO_Operation temp = this->addToQueue(_insvector, found);
			total_time = temp._timestamp;
		}
		if (this->refreshReady(_readyvector))
		 {
			previous = current;
		}
	}
	printresult(total_time,tot_movement,max_wait,tot_TA,tot_wait,count);
	
	}
	 void printresult(int total_time,int tot_movement,int max_wait,int tot_TA,int tot_wait,int count)
	{
		double avg_TA = (double) tot_TA / count;
		double avg_wait = (double) tot_wait / count;
		printf("SUM: %d %d %.2lf %.2lf %d\n", total_time, tot_movement,avg_TA, avg_wait, max_wait);
	}

	static bool _rightsort(IO_Operation m, IO_Operation n) 
	{
		if(m._diskloc == n._diskloc)
		{
			return m._instNum < n._instNum;
		}
		else
		{
			return m._diskloc < n._diskloc;
		}
	}
	static bool _leftsort(IO_Operation m, IO_Operation n) 
	{
		if( m._diskloc == n._diskloc)
		{
			return n._instNum > n._instNum;
		}
		else
		{
			return m._diskloc < n._diskloc;
		}
			
	}
	virtual bool refreshReady(vector<IO_Operation>& ready) 
	{
	return false;
	}

	virtual IO_Operation addToQueue(vector<IO_Operation>& _insvector, bool locating) 
	{
	IO_Operation temp = _insvector.front();
	_insvector.erase(_insvector.begin());
	_readyvector.push_back(temp);
	return temp;
	}
};

class FIFO: public IOScheduler 
{
public:
	IO_Operation getIO_Operation(bool right, int& current) 
{
	
	IO_Operation temp = _readyvector.front();
	_readyvector.erase(_readyvector.begin());
	return temp;
}

};

class SSTF: public IOScheduler {
public:
IO_Operation getIO_Operation(bool right, int& current) 
{
	unsigned int location = 0xffffffff; //-1 in unsigned int
	int index = -1;
	for (int i = 0; i < _readyvector.size(); ++i) 
	{
		int temploc = abs(current - _readyvector[i]._diskloc);
		if (temploc < location) 
		{
			location = temploc;
			index = i;
		}
	}
	IO_Operation temp = _readyvector[index];
	_readyvector.erase(_readyvector.begin() + index);
	return temp;
}
};

class SCAN: public IOScheduler {
public:
	IO_Operation getIO_Operation(bool right, int& current) 
{
	int index;
	if (right) 
	{
		sort(_readyvector.begin(), _readyvector.end(), _rightsort);
	} else 
	{
		sort(_readyvector.begin(), _readyvector.end(), _leftsort);
	}
	if (!right) 
	{
		index = 0;
		for (int i = _readyvector.size() - 1; i >= 0; --i) 
		{
			if (_readyvector[i]._diskloc <= current) 
			{
				index = i;
				break;
			}
		}
	} else 
	{
		index = _readyvector.size() - 1;
		for (int i = 0; i <= _readyvector.size()-1; ++i) 
		{
			if (_readyvector[i]._diskloc >= current) 
			{
				index = i;
				break;
			}
		}
	}
	IO_Operation temp = _readyvector[index];
	_readyvector.erase(_readyvector.begin() + index);
	return temp;
}
};

class CSCAN: public IOScheduler {
public:
	IO_Operation getIO_Operation(bool right, int& current) 
{
	int index = 0;
	sort(_readyvector.begin(), _readyvector.end(), _rightsort);
	for (int i = 0; i <= _readyvector.size()-1; ++i) 
	{
		if (_readyvector[i]._diskloc >= current) 
		{
			index = i;
			break;
		}
	}
	IO_Operation temp = _readyvector[index];
	_readyvector.erase(_readyvector.begin() + index);
	return temp;
}

};

class FSCAN: public IOScheduler {
private:
	vector<IO_Operation> _insvectorBuf;
public:
	bool refreshReady(vector<IO_Operation>& _readyvector)
 {
	if (!_readyvector.empty()) 
	{	
		return false;
	} else 
	{
		_readyvector = _insvectorBuf;
		_insvectorBuf.clear();
		return true;
	}
}
IO_Operation addToQueue(vector<IO_Operation>& _insvector, bool found) 
{
	IO_Operation temp = _insvector.front();
	_insvector.erase(_insvector.begin());
	if (found==false) 
	{
		_readyvector.push_back(temp);
	} 
	else 
	{
		_insvectorBuf.push_back(temp);
	}
	return temp;
}
	IO_Operation getIO_Operation(bool right,int& current) 
{
	int index;
	if (!right) 
	{
		sort(_readyvector.begin(), _readyvector.end(), _leftsort);
	} else 
	{
		sort(_readyvector.begin(), _readyvector.end(), _rightsort);
	}
	if (right) 
	{
		index = _readyvector.size() - 1;
		for (int i = 0; i <= _readyvector.size()-1; i++) 
		{
			if (_readyvector[i]._diskloc >= current) 
			{
				index = i;
				break;
			}
		}
	} 
	else if (!right) {
		index = 0;
		for (int i = _readyvector.size() - 1; i >= 0; i--) 
		{
			if (_readyvector[i]._diskloc <= current) 
			{
				index = i;
				break;
			}
		}
	}
	IO_Operation temp = _readyvector[index];
	_readyvector.erase(_readyvector.begin() + index);
	return temp;
}
};



IOScheduler* sched = new FIFO();
vector<IO_Operation> instrVector;
//Read Algo and store IO_Operations in vector
void read_input(int argc, char* argv[])
{
	int c;
	while ((c = getopt(argc, argv, "s:")) != -1) {
		switch (c) {
		case 's':
			if (optarg[0] == 'i') 
				sched = new FIFO();
			else if (optarg[0] == 'j') 
				sched = new SSTF();
			 else if (optarg[0] == 's') 
				sched = new SCAN();
			 else if (optarg[0] == 'c') 
				sched = new CSCAN();
			else if (optarg[0] == 'f') 
				sched = new FSCAN();
			break;
		default: cout<<"Invalid Arguments!";	
		}
	}
	ifstream infile(argv[argc-1]);
	int i=0;
	int arr_time,loc;
	string inp;
	stringstream inp_stream;
	if (infile.is_open())
	{
		while(getline(infile,inp))
		{
			if(inp.at(0)=='#')
				continue;
			arr_time=0;
			loc=0;
			inp_stream.clear();
			inp_stream.str(inp);
			inp_stream >> arr_time >> loc;
			instrVector.push_back(IO_Operation(arr_time,loc,i));
			i++;
		}
	}
	infile.close();
}

int main(int argc, char* argv[]) {
		
	read_input(argc,argv);
	sched->schedule(instrVector);
	return 0;
}
