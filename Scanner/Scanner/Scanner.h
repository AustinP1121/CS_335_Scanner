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

	//get the index where the end of the line comment is
	int PostLineCommentIndex(vector<char>, int);

	//get the index where the end of the block comment is
	int PostBlockCommentIndex(vector<char>, int);

	//checkss for the end of comment symbols / or */
	bool CheckForEndOfComment(char);
private:
	//buffer used to store file data
	vector<char> _buffer;


};

Scanner::Scanner()
{
	_buffer = ReadFile();
	ScanProgram(_buffer);
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
		if (_singleChar == ' ')
		{
			continue;
		}
		//push to buffer
		_charBuffer.push_back(_singleChar);
	}
	
	return _charBuffer;
}

inline void Scanner::ScanProgram(vector<char> _buffer)
{
	char _currentChar;
	char _nextChar;

	for (int _currentIndex = 0; _currentIndex < _buffer.size(); _currentIndex++)
	{
		_currentChar = _buffer[_currentIndex];
		_nextChar = _buffer[_currentIndex+=1];

		//check if the current char is an operator
		CheckOp(_currentChar);

		//check if the current char is an assignment operator and assure it's used correctly
		if (CheckAssignment(_currentChar, _nextChar) != false)
		{
			cout << "\n*** Error: the character ':' does not follow the assignment token form (:=) ***\n";
			break;
		}

		//check if the current char and the following char are the beginning of a block comment
		else if (_currentChar == '/' && _nextChar == '*')
		{
			//get where the code resumes
			_currentIndex = PostBlockCommentIndex(_buffer, _currentIndex);
		}

		//check if the current and following char are the beginning of a line comment
		else if(_currentChar == '/' && _nextChar == '/')
		{
			//get where the code resumes
			_currentIndex = PostLineCommentIndex(_buffer, _currentIndex);
		}

		else if (_currentChar == '/')
		{
			cout << _currentChar << endl;
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

	return false;
}

inline int Scanner::PostLineCommentIndex(vector<char> _buffer, int _currentIndex)
{
	//index of the start of comment after symbols
	_currentIndex += 2;

	//set the size of the buffer
	int _bufferSize = _buffer.size();
	//get the current char
	char _currentChar = _buffer[_currentIndex];
	//get the next char
	char _nextChar = _buffer[_currentIndex++];
	
	for (int i = _currentIndex; i < _bufferSize; i++)
	{
		//get the current char
		_currentChar = _buffer[i];

		//check for end of line comment
		if (_currentChar == '\n')
		{
			break;
		}

		//continue
		_currentIndex = i;
	}

	return _currentIndex;
}

inline int Scanner::PostBlockCommentIndex(vector<char> _buffer, int _currentIndex)
{
	//first char inside the block comment
	_currentIndex += 1;

	//set the size of the buffer
	int _bufferSize = _buffer.size();
	//get the current char
	char _currentChar = _buffer[_currentIndex];
	//get the next char
	char _nextChar = _buffer[++_currentIndex];

	for (int i = _currentIndex; i < _bufferSize; i++)
	{
		//end of block comment found
		if(_currentChar == '*' && _nextChar == '/')
		{
			//jump to the code
			_currentIndex = 2+i;

			return _currentIndex;
		}

		//continue
		else
		{
			//error if the current index exceeds the available number of elements in buffer
			if (++i > _bufferSize - 1)
			{
				cout << "\n*** ERROR: block comment incomplete! ***\n";
				return _bufferSize-1;
			}
			else
			{
				_currentChar = _buffer[i];
				_nextChar = _buffer[++i];
			}
		}
	}

	//return the index
	return _currentIndex;
}

