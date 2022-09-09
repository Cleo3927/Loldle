#include <bits/stdc++.h>
#include <ncurses.h>

std::map<std::string, std::string> cytaty;
std::vector<std::string> lista;

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

void wypisz_proby(std::string bohater, std::vector<std::string> proby, int licznik) {
	for (int i = licznik; i <= std::min(licznik + 25, (int)proby.size() - 1); i++) {
		attron(COLOR_PAIR(1));	
		printw(proby[i].c_str());
		printw("\n");	
		attroff(COLOR_PAIR(1));	
	}
}

int main() {
	FILE *plik = fopen("../postaci.csv", "r");
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
		lista.push_back(cechy[0]);
	}
	
	plik = fopen("cytaty.csv", "r");
	// wczytanie cytatow 
	while (getline(&linia, &dlugosc, plik) != -1) {
		std::string cechy[2];
		std::string wejscie(linia);
		
		size_t last = -1;
		for (size_t i = 0; i < 8; i++) {
			size_t end = wejscie.find('$', last + 2);
			
			cechy[i] = wejscie.substr(last + 1, end - last - 1);
			last = end;
		}
		
		cytaty[cechy[0]] = cechy[1];
	}

	// wylosowanie postaci
	int ktora = losuj(0, cytaty.size() - 1);

	std::string cytat;

	for (auto x: cytaty) {
		if (ktora == 0) {
			cytat = x.first;
			break;
		}
		ktora--;
	}

	// start
	initscr();
	keypad(stdscr, TRUE);
	printw(cytat.c_str());
	noecho();
	start_color();
	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_BLACK);
		
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
				printw(cytat.c_str());
				printw("\n");
				printw(prefix.c_str());
				printw("\n");
				autokorekta(prefix, wskaznik);
			} else if (znak == KEY_BACKSPACE && tryb == 0) { // usuwam ostatnia litere
				if (prefix.size() > 0)
					prefix.pop_back();
				
				wskaznik = 0;
				clear();
				move(0, 0);
				printw(cytat.c_str());
				printw("\n");
				printw(prefix.c_str());
				printw("\n");
				autokorekta(prefix, wskaznik);
			} else if (znak == KEY_UP) { // do gory
				if (tryb == 0) {
					wskaznik = std::max(0, wskaznik - 1);

					clear();
					move(0, 0);
					
					printw(cytat.c_str());
					printw("\n");
					
					autokorekta(prefix, wskaznik);
				} else {
					clear();
					move(0, 0);
					
					printw(cytat.c_str());
					printw("\n");
					
					wypisz_proby(lista[ktora], proby, licznik_prob);
				}
			} else if (znak == KEY_DOWN) { // do dolu
				if (tryb == 0) {
					wskaznik = std::min(wskaznik + 1, ile_bohaterow(prefix) - 1);
				
					clear();
					move(0, 0);
					printw(cytat.c_str());
					printw("\n");
					
					autokorekta(prefix, wskaznik);
				} else {
					clear();
					move(0, 0);
					printw(cytat.c_str());
					printw("\n");

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
					printw(cytat.c_str());
					printw("\n");
					
					wypisz_proby(lista[ktora], proby, licznik_prob);
				}
				else {
					clear();
					move(0, 0);
					printw(cytat.c_str());
					printw("\n");

					printw(prefix.c_str());
					printw("\n");
					autokorekta(prefix, wskaznik);
				}
			}	
		}

		printw("WYBRANO: ");
		printw(wybor.c_str());
		printw("\n");

		if (cytaty[cytat] == wybor)
			break;
		
		attron(COLOR_PAIR(1));	
		printw("Wrong Answer\n");
		attroff(COLOR_PAIR(1));	

	}
	endwin();
	std::cout << cytat << " " << cytaty[cytat] << "\n";

}