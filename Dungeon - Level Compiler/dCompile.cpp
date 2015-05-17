#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <ctype.h>

using namespace std;

typedef string strang;
enum ERROR_STATE {NO_ERROR, FILE_ERROR, SYNTAX_ERROR, CONSTRAINT_ERROR};
enum P_STATE {START, ACCEPT, REJECT_W, REJECT_T};
typedef struct point
{
	char xpos;
	char ypos;
}point;

int parse(strang levelContents)
{
	P_STATE state = START;

	int levelSize = levelContents.length();
		for(int i = 0; i < levelSize; i = i + 3)
		{
			if (state == START)
			{
				if (levelContents[i] == 't')
				{
					state = START;
				}

				else if (levelContents[i] == 'w')
				{
					state = ACCEPT;
				}

				else
				{
					cout << "Error - only use t and w (start)" << endl;
					break;
				}
			}

			else if (state == ACCEPT)
			{
				if (levelContents[i] == 't')
				{
					state = REJECT_T;
				}

				else if (levelContents[i] == 'w')
				{
					state = REJECT_W;
				}

				else
				{
					cout << "Error - only use t and w (accept)" << endl;
					break;
				}
			}

			else if (state == REJECT_W || state == REJECT_T)
			{
				cout << "Error - One 'win' block allowed, after all 'trap' blocks." << endl;
				break;
			}

		}

	if (state == ACCEPT) return 0;
	else return 1;

}

typedef struct EntryType{
	strang name;
	strang signature;
	int constraints;
} EntryType;

const int ENTRY_COUNT = 4;
const EntryType ENTRY_TABLE[ENTRY_COUNT] = {
    {"Trap", "t # #", 101},
    {"Win", "w # #", 1},
    {"Spawn", "s # #", 1},
    {"Location", "l # # # #", 101}
};

typedef struct Entry{
	int id;
	vector<int> values;
} Entry;

bool matchDigit(strang &text, size_t &pos, int &value){
	size_t last = pos++;
	value = text[last] - '0';
	if(value >= 0 && value <= 9) return true;
	return false;
}

bool matchSpaces(strang &text, size_t &pos){
	if(text[pos++] != ' ') return false;
	while(text[pos] == ' ') pos++;
	return true;
}

Entry matchEntry(strang text){
	Entry result;
	for(int entryType = 0; entryType < ENTRY_COUNT; entryType++){
		size_t pos = 0;
		strang signature = ENTRY_TABLE[entryType].signature;
		size_t sPos = 0;
		int value;
		vector<int> values;
		bool matching = true;
		while(matching && sPos < signature.size()){
			char c = signature[sPos];
			//cout << sPos << endl;
			switch(c){
				case '#':
					matching = matchDigit(text, pos, value);
					values.push_back(value);
					break;
				case ' ':
					matching = matchSpaces(text, pos);
					break;
				default:
					if(text[pos++] != c) matching = false;
					break;
			}
			sPos++;
		}
		if(matching){
			result.id = entryType;
			result.values = values;
			return result;
		}
	}
	result.id = -1;
	return result;
}

vector<Entry> lexTokens(strang &text, ERROR_STATE &success){
	vector<Entry> results;
	size_t pos = 0;
	int *typeCount = new int[ENTRY_COUNT]();
	//cout << text.size() << endl;
	while(pos < text.size()){
		strang line = "";
		while(pos < text.size() && text[pos] != '\n'){
			line += text[pos++];
		}
		pos++;
		//cout << pos << endl;
		Entry newEntry = matchEntry(line);
		int id = newEntry.id;
		if(id == -1 && pos < text.size()){
			success = SYNTAX_ERROR;
			return results;
		}
		else{
			if(++typeCount[id] > ENTRY_TABLE[id].constraints){
				cout << ENTRY_TABLE[id].name << ": " << typeCount[id] << endl;
				success = CONSTRAINT_ERROR;
				return results;
			}
			results.push_back(newEntry);
		}
	}
	return results;
}

strang parseTokens(vector<Entry> entries){
	strang result = "";
	strang tables[ENTRY_COUNT];
	size_t pos = 0;
	while(pos < entries.size()){
		Entry current = entries[pos];
		int id = current.id;
		for(vector<int>::iterator it = current.values.begin();
			it != current.values.end(); ++it)
		{
			tables[id] += (char)(*it);
		}
		++pos;
	}
	for(int c = 0; c < ENTRY_COUNT; c++){
		cout << ENTRY_TABLE[c].name << ": " << tables[c] << endl;
		result += tables[c];
		result += (char)(-1);
		result += (char)(-1);
	}
	return result;
}

ERROR_STATE compileTokens(strang &levelName, strang &compiledLevel){
	ifstream fs(levelName.c_str());
	if(!fs.good()) return FILE_ERROR;
	strang levelContents = "";
	while(fs.good())
	{
		char c = fs.get();
		if(c != EOF){
			levelContents = levelContents + c;
		}
	}
	ERROR_STATE success = NO_ERROR;
	cout << "lexing" << endl;
	vector<Entry> terms = lexTokens(levelContents, success);
	if(success != NO_ERROR) return success;
	cout << "parsing" << endl;
	compiledLevel = parseTokens(terms);
	return success;
}

/*
int lexer(strang levelContents)
{
	int levelSize = levelContents.length();
	if (levelSize == 0)
	{
		cout << "Empty File" << endl;
		return 1;
	}

	for(int i = 0; i < levelSize; i = i + 3)
	{
		if (levelContents[i] == 'w' || levelContents[i] == 't') {}
		else return 1;

		if (levelContents[i+1]-'0' >= 0 && levelContents[i+1]-'0' <= 9) {}
		else return 1;

		if (levelContents[i+2]-'0' >= 0 && levelContents[i+2]-'0' <= 9) {}
		else return 1;
	}

	return 0;
}

int compile(strang &levelName, strang &compiledLevel)
{
	ifstream fs(levelName.c_str());
	strang levelContents = "";
	while(fs.good())
	{
		char c = fs.get();
		if(isspace(c) == 0 && c != EOF){
			levelContents = levelContents + c;
		}
	}
 	//cout << levelContents << '|' << endl;
	int lexedLevel = lexer(levelContents);
	if (lexedLevel == 0)
	{
		int parsedLevel = parse(levelContents);
		if (parsedLevel == 0)
		{
			int levelSize = levelContents.length();
			point *trapPoints,*treasurePoints;
			trapPoints = new point[levelSize];
			treasurePoints = new point[levelSize];
			int j = 0;
			int k = 0;

			for(int i = 0; i < levelSize; i=i+3)
			{
				if (levelContents[i] == 't')
				{
					trapPoints[j].xpos = levelContents[i+1];
					trapPoints[j].ypos = levelContents[i+2];
					j++;
				}

				else if (levelContents[i] == 'w')
				{
					treasurePoints[k].xpos = levelContents[i+1];
					treasurePoints[k].ypos = levelContents[i+2];
					k++;
				}
			}

			unsigned trapLength = j;
			unsigned treasureLength = k;
			for(unsigned c = 0; c < trapLength; c++)
			{
				compiledLevel += trapPoints[c].xpos-'0';
				compiledLevel += trapPoints[c].ypos-'0';
			}
			compiledLevel += (char)(-1);
			compiledLevel += (char)(-1);

			for(unsigned c = 0; c < treasureLength; c++)
			{
				compiledLevel += treasurePoints[c].xpos-'0';
				compiledLevel += treasurePoints[c].ypos-'0';
			}
			size_t cLength = compiledLevel.length();
			for(unsigned c = 0; c < cLength; c++)
			{
				cout << (int)compiledLevel[c];
			}

			return 0;
		}

		else return 1;
	}

	else return 2;

}

*/

void writeLevel(strang &levelName, strang &compiledLevel){
	strang outputName = levelName.substr(0,levelName.find_last_of('.')) + ".dat";
	ofstream output(outputName.c_str());
	output << compiledLevel;
}

int main()
{
	strang levelName;
	strang compiledLevel;
    cout << "Enter full path of level you wish to compile" << endl;
    cin >> levelName;
    ERROR_STATE i = compileTokens(levelName,compiledLevel);
    if (i == NO_ERROR) cout << "Level Compile Successful" << endl;
    else if (i == FILE_ERROR) cout << "Compile Unsuccessful, File not found" << endl;
    else if (i == SYNTAX_ERROR) cout << "Compile Unsuccessful, Syntax Error" << endl;
    else if (i == CONSTRAINT_ERROR) cout << "Compile Unsuccessful, Constraint Error" << endl;
    writeLevel(levelName,compiledLevel);
    return 0;
}
