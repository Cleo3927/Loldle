#include <bits/stdc++.h>
#include <ncurses.h>

struct Cecha {
	std::string kraina;
	std::string plec;
	std::string rasa;
	std::string energia;
	std::string dystans;
	std::string pozycje;
	int rok;
};

std::map<std::string, Cecha> postaci;
std::vector<std::string> lista;
std::vector<char> alfabet;

int losuj(int a, int b) {
	std::random_device dev;
	std::mt19937 mt(dev());
	std::uniform_int_distribution<int> los(a, b);
	return los(mt);
}

bool czy_alfabet(char znak) {
	if ('a' <= znak && znak <= 'z')
		return true;
	if ('A' <= znak && znak <= 'Z')
		return true;
	if (znak == ' ' || znak == '\'')
		return true;

	return false;
}

bool cmp(std::string a, std::string b) {
	if (a.size() != b.size())
		return false;

	for (size_t i = 0; i < a.size(); i++)
		if (std::toupper(a[i]) != std::toupper(b[i]))
			return false;

	return true;
}

void autokorekta(std::string prefix, int wskaznik) {
	if (prefix.size() == 0)
		return;
	int ile = 0;
	for (auto x: lista) {
		if (cmp(prefix, x.substr(0, prefix.size()))) {
			printw(x.c_str());
			if (wskaznik == ile)
				printw(" <- ");
			printw("\n");

			ile++;
		}
	}
}

int ile_bohaterow(std::string prefix) {
	int ile = 0;
	for (auto x: lista) {
		if (cmp(prefix, x.substr(0, prefix.size()))) {
			ile++;
		}
	}
	return ile;
}

std::string wez_bohatera(std::string prefix, int wskaznik) {
	int ile = 0;
	for (auto x: lista) {
		if (cmp(prefix, x.substr(0, prefix.size()))) {
			if (wskaznik == ile) { 
				return x;
			}
			ile++;
		}
	}
	return "";
}

int check_position(std::string a, std::string b) {
	std::string pos_a = postaci[a].pozycje + "/";
	std::string pos_b = postaci[b].pozycje + "/";
	
	std::set<std::string> set_a;
	std::set<std::string> set_b;
	std::string tmp = "";	

	for (size_t i = 0; i < pos_a.size(); i++) {
		if (pos_a[i] == '/') {
			set_a.insert(tmp);
			tmp = "";
		} else {
			tmp += pos_a[i];
		}
	}

	bool znal = false;
	
	for (size_t i = 0; i < pos_b.size(); i++) {
		if (pos_b[i] == '/') {
			set_b.insert(tmp);
			if (set_a.count(tmp) > 0)
				znal = true;
			tmp = "";
		} else {
			tmp += pos_b[i];
		}
	}

	if (set_a == set_b)
		return 0;
	
	if (znal)
		return 1;

	return 2;
}

void wypisz_porownanie(std::string a, std::string b) {
	printw("%-13s ", b.c_str());

	// kraina
	if (postaci[a].kraina == postaci[b].kraina)
		attron(COLOR_PAIR(2));
	else
		attron(COLOR_PAIR(1));

	printw("%15s ", postaci[b].kraina.c_str());
	
	// plec
	if (postaci[a].plec == postaci[b].plec)
		attron(COLOR_PAIR(2));
	else
		attron(COLOR_PAIR(1));

	printw("%-6s ", postaci[b].plec.c_str());

	//rasa 
	if (postaci[a].rasa == postaci[b].rasa)
		attron(COLOR_PAIR(2));
	else
		attron(COLOR_PAIR(1));

	printw("%24s ", postaci[b].rasa.c_str());
	
	//energia
	if (postaci[a].energia == postaci[b].energia)
		attron(COLOR_PAIR(2));
	else
		attron(COLOR_PAIR(1));

	printw("%-11s ", postaci[b].energia.c_str());

	//dystans
	if (postaci[a].dystans == postaci[b].dystans)
		attron(COLOR_PAIR(2));
	else
		attron(COLOR_PAIR(1));

	printw("%6s ", postaci[b].dystans.c_str());

	//pozycje;
	if (check_position(a, b) == 0) // ok 
		attron(COLOR_PAIR(2));
	else if (check_position(a, b) == 1) // czesciowo
		attron(COLOR_PAIR(3));
	else
		attron(COLOR_PAIR(1)); // zle

	printw("%-25s ", postaci[b].pozycje.c_str());

	// rok;
	if (postaci[a].rok == postaci[b].rok)
		attron(COLOR_PAIR(2));
	else
		attron(COLOR_PAIR(1));

	char c;
	if (postaci[a].rok > postaci[b].rok)
		c = L'+';
	else if (postaci[a].rok < postaci[b].rok)
		c = L'-';
	else
		c = L'=';
	printw("%4d%c\n", postaci[b].rok, c);

	attroff(COLOR_PAIR(2));
	attroff(COLOR_PAIR(1));
}

void wypisz_proby(std::string bohater, std::vector<std::string> proby, int licznik) {
	for (int i = licznik; i <= std::min(licznik + 25, (int)proby.size() - 1); i++)
		wypisz_porownanie(bohater, proby[i]);
}

int main() {
	FILE *plik = fopen("postaci.csv", "r");
	char *linia = nullptr;
	size_t dlugosc = 0;
	int tryb = 0;

	std::vector<std::string> proby;
	int licznik_prob = 0;
	
	int max[8] = {0, 0, 0, 0, 0, 0, 0, 0};

	// wczytanie bohaterow
	while (getline(&linia, &dlugosc, plik) != -1) {
		std::string cechy[8];
		std::string wejscie(linia);
		
		size_t last = -1;
		for (size_t i = 0; i < 8; i++) {
			size_t end = wejscie.find(',', last + 2);
			
			cechy[i] = wejscie.substr(last + 1, end - last - 1);
			last = end;
			max[i] = std::max(max[i], (int)cechy[i].size());
		}

		postaci[cechy[0]] = {cechy[1], cechy[2], cechy[3], cechy[4], cechy[5], cechy[6], stoi(cechy[7])};
		lista.push_back(cechy[0]);
	}

	// wylosowanie postaci
	int ktora = losuj(0, postaci.size() - 1);

	// start
	initscr();
	keypad(stdscr, TRUE);
	printw("Podaj losowego bohatera:\n");
	noecho();
	start_color();
	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_BLACK);
	init_pair(3, COLOR_YELLOW, COLOR_BLACK);
		
	while (true) {
		int znak;
		std::string prefix = "";
		int wskaznik = -1;
		std::string wybor = "";

		while (znak = getch()) {
			if (czy_alfabet(znak)) { // dodaje nowy znak
				prefix += znak;
				
				wskaznik = 0;
				tryb = 0;

				clear();
				move(0, 0);
				printw(prefix.c_str());
				printw("\n");
				autokorekta(prefix, wskaznik);
			} else if (znak == KEY_BACKSPACE && tryb == 0) { // usuwam ostatnia litere
				if (prefix.size() > 0)
					prefix.pop_back();
				
				wskaznik = 0;
				clear();
				move(0, 0);
				printw(prefix.c_str());
				printw("\n");
				autokorekta(prefix, wskaznik);
			} else if (znak == KEY_UP) { // do gory
				if (tryb == 0) {
					wskaznik = std::max(0, wskaznik - 1);

					clear();
					move(0, 0);
					autokorekta(prefix, wskaznik);
				} else {
					clear();
					move(0, 0);
					
					wypisz_proby(lista[ktora], proby, licznik_prob);
				}
			} else if (znak == KEY_DOWN) { // do dolu
				if (tryb == 0) {
					wskaznik = std::min(wskaznik + 1, ile_bohaterow(prefix) - 1);
				
					clear();
					move(0, 0);
					autokorekta(prefix, wskaznik);
				} else {
					clear();
					move(0, 0);

					wypisz_proby(lista[ktora], proby, licznik_prob);
				}
			} else if (znak == '\n' && tryb == 0 && prefix.size() > 0 && ile_bohaterow(prefix) > 0) {
				wybor = wez_bohatera(prefix, wskaznik);
				proby.push_back(wybor);
				break;	
			} else if (znak == KEY_LEFT || znak == KEY_RIGHT) {
				tryb ^= 1;

				if (tryb == 1) {
					clear();
					move(0, 0);
					
					wypisz_proby(lista[ktora], proby, licznik_prob);
				}
				else {
					clear();
					move(0, 0);

					printw(prefix.c_str());
					if (prefix.size() > 0)
						printw("\n");
					autokorekta(prefix, wskaznik);
				}
			}	
		}

		printw("WYBRANO: ");
		printw(wybor.c_str());
		printw("\n");

		wypisz_porownanie(lista[ktora], wybor);

		if (lista[ktora] == wybor)
			break;
	}
		
	clear();
	move(0, 0);
	printw("wykorzystano: ");
	printw(std::to_string(proby.size()).c_str());
	printw("\n");
	wypisz_proby(lista[ktora], proby, licznik_prob);

	while (getch() != '\n') {
		clear();
		move(0, 0);
		printw("wykorzystano: ");
		printw(std::to_string(proby.size()).c_str());
		printw("\n");
		wypisz_proby(lista[ktora], proby, licznik_prob);
	}

	endwin();

}