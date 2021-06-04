#include <iostream>
#include <fstream>

using namespace std;

struct Field {
	int value;
	int index;
	Field *prev;
	Field *next;
};

struct State {
	int id;
	int readValue;
	int nextId;
	int writeValue;
	int move;
	State *next;
};

Field *head, *tail, *selector;

State *zero, *pos, *neg;

void updateList(int start, int end, int value) {
	Field *elem;
	if (head == nullptr) {
		head = new Field();
		tail = new Field();
		head->value = -1;
		head->index = 0;
		head->next = tail;
		head->prev = nullptr;
		tail->value = -1;
		tail->index = 1;
		tail->next = nullptr;
		tail->prev = head;
	}
	while (tail->index < end) {
		elem = new Field();
		elem->value = -1;
		elem->index = tail->index + 1;
		elem->next = nullptr;
		elem->prev = tail;
		tail->next = elem;
		tail = elem;
	}
	elem = head;
	while (elem->index != start) {
		elem = elem->next;
	}
	for (int i = 0; i < end - start + 1; i++) {
		elem->value = value;
		elem = elem->next;
	}
}

void createList() {
	ifstream file("list.txt");
	string input;
	int a, b, c, k;
	while (getline(file, input)) {
		string ch;
		k = 0, a = 0, b = 0, c = 0;
		ch = input[k];
		while (ch != ":") {
			a = a * 10 + stoi(ch);
			ch = input[++k];
		}
		ch = input[++k];
		while (ch != " ") {
			b = b * 10 + stoi(ch);
			ch = input[++k];
		}
		ch = input[++k];
		if (ch == "b") {
			c = -1;
		} else {
			c = stoi(ch);
		}
		updateList(a, b, c);
	}
	file.close();
}

void outputList() {
	ofstream output;
	Field *current;
	string val;
	int a;
	output.open("output.txt");
	current = head;
	while (current != nullptr) {
		a = current->value;
		if (a == -1) {
			val = "b";
		} else if (a == 0) {
			val = "0";
		} else {
			val = "1";
		}
		output << val;
		current = current->next;
	}
	output.close();
}

void addRight() {
	Field *elem = new Field;
	elem->value = -1;
	elem->index = tail->index + 1;
	elem->next = nullptr;
	elem->prev = tail;
	tail->next = elem;
	tail = elem;
}

void addLeft() {
	Field *elem = new Field;
	elem->value = -1;
	elem->index = head->index - 1;
	elem->next = head;
	elem->prev = nullptr;
	head->prev = elem;
	head = elem;
}

void setSelector(int position) {
	if (position < head->index) {
		while (position < head->index) {
			addLeft();
		}
		selector = head;
	} else if (position > tail->index) {
		while (position > tail->index) {
			addRight();
		}
		selector = tail;
	} else {
		int a = (head->index + tail->index) / 2;
		if (position > a) {
			selector = tail;
			while (selector->index != position) {
				selector = selector->prev;
			}
		} else {
			selector = head;
			while (selector->index != position) {
				selector = selector->next;
			}
		}
	}
}

void initPointers() {
	head = nullptr;
	tail = nullptr;
	zero = nullptr;
	pos = nullptr;
	neg = nullptr;
}

void updateStates(int start, int read, int next, int write, int move) {
	State *elem, *current;
	if (zero == nullptr) {
		zero = new State();
		pos = new State();
		neg = new State();
		zero->id = 0;
		zero->readValue = read;
		zero->nextId = next;
		zero->writeValue = write;
		zero->move = move;
		zero->next = nullptr;
		pos->id = -1;
		neg->id = -2;
	} else {
		current = zero;
		while (current->next != nullptr) {
			current = current->next;
		}
		elem = new State();
		elem->id = start;
		elem->readValue = read;
		elem->nextId = next;
		elem->writeValue = write;
		elem->move = move;
		elem->next = nullptr;
		current->next = elem;
		current = elem;
	}
}

void createStates() {
	ifstream file("states.txt");
	string input, ch;
	int s, r, n, w, m, i;
	while (getline(file, input)) {
		s = 0;
		r = 0;
		n = 0;
		w = 0;
		m = 0;
		i = 0;
		ch = input[i];
		while (ch != " ") {
			s = s * 10 + stoi(ch);
			ch = input[++i];
		}
		ch = input[++i];
		if (ch == "b") {
			r = -1;
		} else {
			r = stoi(ch);
		}
		i += 2;
		ch = input[i];
		while (ch != " ") {
			if (ch == "+") {
				n = -1;
				ch = input[++i];
				break;
			} else if (ch == "-") {
				n = -2;
				ch = input[++i];
				break;
			} else {
				n = n * 10 + stoi(ch);
				ch = input[++i];
			}
		}
		ch = input[++i];
		if (ch == "b") {
			w = -1;
		} else {
			w = stoi(ch);
		}
		i += 2;
		ch = input[i];
		if (ch == "-") {
			m = -1;
		} else {
			m = 1;
		}
		updateStates(s, r, n, w, m);
	}
	file.close();
}

void executeProgram() {
	State *current = zero;
	int state = 0;
	while ((current != nullptr)) {
		if ((current->id == state) && (current->readValue == selector->value)) {
			break;
		}
		current = current->next;
	}
	if (current->readValue == selector->value) {
		while ((state > -1) && (current != nullptr)) {
			selector->value = current->writeValue;
			if (current->move == 1) {
				if (selector->next == nullptr) {
					addRight();
				}
				selector = selector->next;
			} else {
				if (selector->prev == nullptr) {
					addLeft();
				}
				selector = selector->prev;
			}
			state = current->nextId;
			current = zero->next;
			while ((current != nullptr)) {
				if ((current->id == state)
						&& (current->readValue == selector->value)) {
					break;
				}
				current = current->next;
			}
		}
		if (state == -1) {
			cout << "Uspesno izvrsen program." << endl;
		} else {
			cout << "Greska!\nProverite ulazne komponente." << endl;
		}
	} else {
		cout << "Greska!\nProverite ulazne komponente." << endl;
	}
}

int main() {
	initPointers();
	cout
			<< "Turing-ova masina\nl - Ucitaj listu\nq - Ucitaj stanja\ns - Pokreni rad aplikacije\np - Ispisi listu\nend - Zavrsi sa radom"
			<< endl;
	string ans;
	int p;
	cin >> ans;
	while (ans != "end") {
		if (ans == "l") {
			createList();
			cout << "Lista je ucitana." << endl;
		} else if (ans == "q") {
			createStates();
			cout << "Stanja su ucitana." << endl;
		} else if (ans == "s") {
			cout << "Unesite pocetnu lokaciju programa: " << endl;
			cin >> p;
			setSelector(p);
			executeProgram();
		} else if (ans == "p") {
			outputList();
			cout << "Lista je ispisana." << endl;
		} else {
			cout << "Nepostojeca funkcija!\nMolimo unesite ispravnu funkciju:"
					<< endl;
		}
		cin >> ans;
	}
	return 0;
}
