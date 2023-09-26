#pragma once

//need these for later
#include <iostream>
#include <stack>
#include <vector>
#include <fstream>

using namespace std;

class Scanner
{
public:
	Scanner();
	~Scanner();

	//reads in file data to the buffer
	vector<char> ReadFile();
	
	//ingests the buffer and constructs a set of tokens
	void ScanProgram(vector<char>);
	
	//check if current token is an operator
	void CheckOp(char);
	
	//check if current token is an assignment
	bool CheckAssignment(char, char);

	//check if current token is the start of a line or block comment
	void CheckComment(); 
private:
	//buffer used to store file data
	vector<char> _buffer;


};

Scanner::Scanner()
{
}

Scanner::~Scanner()
{
}

inline vector<char> Scanner::ReadFile()
{
	//instantiate output file stream
	ifstream _fin;
	
	//fifo buffer for reading in a single char
	char _singleChar;
	//buffer to store all chars
	vector<char> _charBuffer; 

	//open the file
	_fin.open("../ProgramFile.txt");

	//while there are chars to read, insert into buffer
	while (_fin.get(_singleChar))
	{
		//push to buffer
		_charBuffer.push_back(_singleChar);
	}
	
	return _charBuffer;
}

inline void Scanner::ScanProgram(vector<char> _buffer)
{
	char _currentChar;
	char _nextChar;

	for (int i = 0; i < _buffer.size(); i++)
	{
		_currentChar = _buffer[i];
		_nextChar = _buffer[i++];

		CheckOp(_currentChar);

		if (!CheckAssignment(_currentChar, _nextChar))
		{
			cout << "\n*** Error: the character ':' does not follow the assignment token form (:=) ***\n";
			break;
		}



	}
}

inline void Scanner::CheckOp(char _currentChar)
{
	if (_currentChar == '('
		|| _currentChar == ')'
		|| _currentChar == '+'
		|| _currentChar == '-'
		|| _currentChar == '*')
	{
		cout << _currentChar << "\n";
	}
}

inline bool Scanner::CheckAssignment(char _currentChar, char _nextChar)
{
	if (_currentChar == ':')
	{
		if (_nextChar != '=')
		{
			return false;
		}
		else
		{
			cout << "\n:=\n";
		}
	}

	return true;
}

inline void Scanner::CheckComment()
{
}
