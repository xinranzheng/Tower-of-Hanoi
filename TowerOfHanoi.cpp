/*
Programmer: Xinran Zheng

Extra Credit Assignment
ScottEC Part 2
Tower of Hanoi
*/

#include <stack>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

struct Move;
class Tower;

enum LEVEL { EASY = 3, MEDIUM = 6, HARD = 12};
vector<string> INPUTS = {"1", "2", "3", "easy", "medium", "hard"};
vector<string> KEYWORDS = {"move", "undo", "redo"};
stack<Move> MOVES;
stack<Move> UNDID;

struct Disk {
	int size;

	Disk(int theSize) : size(theSize) {}

	Disk(const Disk& rhs) : size(rhs.size) {}
};

struct Move {
	Tower* from;
	Tower* to;

	Move(Tower* x, Tower* y) : from(x), to(y) {} 

	Move(const Move& rhs) : from(rhs.from), to(rhs.to) {}
};

class Tower {
public:
	explicit Tower(int theLevel, int theSize = 0) : level(theLevel), size(theSize) {
		for (int i = level; i > 0; i--) {
			Disk* aDisk = new Disk(i);
			disks.push(aDisk);
		}
	}

	Tower(const Tower& rhs) : level(rhs.level), size(rhs.size) {
		stack<Disk*> tmp = rhs.disks;
		swap(tmp, disks);
	}

	void add(Disk* aDisk) {
		disks.push(aDisk);
		size++;
	}

	Disk* remove() {
		Disk* tmp = disks.top();
		disks.pop();
		size--;
		return tmp;
	}

	int topSize() const {
		return disks.top()->size;
	}

	bool full() const {
		return level == size;
	}

	bool empty() const {
		return size == 0;
	}

	friend void showTowers(Tower& one, Tower& two, Tower& three);

private:
	int level; //difficulty level
	int size; //current size
	stack<Disk*> disks;
};

int welcomeScreen() {
	//displays banner and lets user choose a game difficulty level
	//returns the level chosen as an integer
	cout << "===================================================================" << endl;
	cout << "                     Welcome to Tower of Hanoi                     " << endl;
	cout << "===================================================================" << endl;
	cout << "What level would you like to play?" << endl;
	cout << "1. easy: 3 disks" << endl;
	cout << "2. Medium: 6 disks" << endl;
	cout << "3. Hard: 12 disks" << endl;
	string level;
	cin >> level;
	while (find(INPUTS.begin(), INPUTS.end(), level) == INPUTS.end()) {
		cout << "Sorry, not a valid input. Please try again." << endl;
		cout << "What level would you like to play?" << endl;
		cout << "1. easy: 3 disks" << endl;
		cout << "2. Medium: 6 disks" << endl;
		cout << "3. Hard: 12 disks" << endl;
		cin >> level;
	}
	if (level == "1" || level == "easy") {
		return EASY;
	}
	else if (level =="2" || level == "medium") {
		return MEDIUM;
	}
	return HARD;
}

bool move(Tower& from, Tower& to) {
	//try to make a move
	//returns true if move was successful
	if (!to.empty() && from.topSize() > to.topSize()) {
		cout << "Invalid Move" << endl;
		return false;
	}
	MOVES.push(Move(&from, &to));
	to.add(from.remove());
	return true;
}

void undo() {
	if (MOVES.empty()) {
		cout << "Hey, there's nothing to undo, friend." << endl;
		return;
	}
	MOVES.top().from->add(MOVES.top().to->remove());
	UNDID.push(MOVES.top());
	MOVES.pop();
}

void redo() {
	if (UNDID.empty()) {
		cout << "You already reversed all of the immediate undos." << endl;
		return;
	}
	UNDID.top().to->add(UNDID.top().from->remove());
	MOVES.push(UNDID.top());
	UNDID.pop();
}

void clearUndid(stack<Move>& undid) {
	//clears the UNDID stack when user makes a move
	while (!undid.empty()) {
		undid.pop();
	}
}

void showDisk(int level, Disk* aDisk) {
	string spaces( (level - aDisk->size) / 2 + 3, ' ' );
	cout << spaces;
	cout << string( aDisk->size, '=' );
	cout << spaces;
}

void showTowers(Tower& one, Tower& two, Tower& three) {
	cout << "===================================================================" << endl;
	cout << "                          Tower of Hanoi                           " << endl;
	cout << "===================================================================" << endl;
	int level = one.level;
	int size1 = one.size;
	int size2 = two.size;
	int size3 = three.size;
	//should I even do this?!?!?!
	stack<Disk*> t1 = one.disks;
	stack<Disk*> t2 = two.disks;
	stack<Disk*> t3 = three.disks;
	
	string spaces( (17 - level), ' ' );
	string placeholder(level, ' ');
	for (int i = 0; i < level; i++) {
		cout << spaces;
		if (size1 != level) {
			cout << placeholder;
			size1++;
		}
		else {
			showDisk(level, t1.top());
			t1.pop();
		}
		cout << spaces;
		if (size2 != level) {
			cout << placeholder;
			size2++;
		}
		else {
			showDisk(level, t2.top());
			t2.pop();
		}
		cout << spaces;
		if (size3 != level) {
			cout << placeholder;
			size3++;
		}
		else {
			showDisk(level, t3.top());
			t3.pop();
		}
		cout << spaces << endl;
	}

	//this is the table...
		cout << "___________________________________________________________________" << endl;
		cout << "||             1                 2                3              ||" << endl;	
}

bool play(Tower& one, Tower& two, Tower& three) {
	cout << "Make a move or undo/redo a move." << endl;
	cout << "To make a move, enter 'move'" << endl;
	cout << "Or enter 'undo' or 'redo' to cover up your mistakes" << endl;
	cout << "Unless you want to quit, in which case just enter 'quit'" << endl;
	string input;
	int from;
	int to;
	cin >> input;
	if (input == "quit") {
		return false;
	}
	while (find(KEYWORDS.begin(), KEYWORDS.end(), input) == KEYWORDS.end()) {
		cout << "Just use the right words, thanks." << endl;
		cout << "To make a move, enter 'move'" << endl;
		cout << "Or enter 'undo' or 'redo' to cover up your mistakes" << endl;
		cin >> input;
		if (input == "quit") {
			return false;
		}
	}
	if (input == "move") {
		clearUndid(UNDID);
		//assmumes the user is not an asshole and enters valid numbers
		//too lazy to check for invalid inputs
		cout << "Which tower do you want to move the disk from?" << endl;
		cin >> from;
		cout << "Which tower do you want to move the disk to?" << endl;
		cin >> to;
		Tower* f;
		Tower* t;
		if (from == 1) {f = &one;}
		else if (from == 2) {f = &two;}
		else {f = &three;}
		if (to == 1) {t = &one;}
		else if (to == 2) {t = &two;}
		else {t = &three;}
		while (!move(*f, *t)) {
			cout << "Which tower do you want to move the disk from?" << endl;
			cin >> from;
			cout << "Which tower do you want to move the disk to?" << endl;
			cin >> to;
			if (from == 1) {f = &one;}
			else if (from == 2) {f = &two;}
			else {f = &three;}
			if (to == 1) {t = &one;}
			else if (to == 2) {t = &two;}
			else {t = &three;}
		}
		
	}
	else if (input == "undo") {
		undo();
	}
	else {
		redo();
	}
	showTowers(one, two, three);
	return true;
}

void winScreen() {
	cout << endl;
	cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
	cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
	cout << "         ***          Congrats! You're Done!!!         ***         " << endl;
	cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
	cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
	cout << endl;
}

void sadFace() {
	cout << "             ****************************************" << endl;
	cout << "             ***  :( Don't give up! Try again!!!  ***" << endl;
	cout << "             ****************************************" << endl;
	cout << endl;
}

void towerOfHanoi(int level) {
	//keeps running the game until the user puts all the disks from tower 1 to tower 3
	//maybe add terminate game option when this game actually runs
	Tower one(level, level);
	Tower two(level);
	Tower three(level);
	showTowers(one, two, three);
	while (!three.full()) {
		bool thing = play(one, two, three);
		if (!thing) {
			sadFace();
			break;
		}
	}
	if (three.full()) {
		winScreen();
	}
}

int main() {
	while (true) {
		towerOfHanoi(welcomeScreen());
	}
}