#include <iostream>
#include <windows.h>
#include <locale.h>
#include <time.h>
#include <conio.h>
#include <cstdio>

using namespace std;

const int N = 10;

int ships_ID = 1;
int cpu_ships_ID = 1;
int ships[N] = { 0 };
int ships_cpu[N] = { 0 };
int CPU_shooting_matrix[N][N] = { 0 };
int show_x = 0, show_y = 0;
int player_ships = 0;
int player_decks = 0;
int cpu_ships = 10;
int cpu_decks = 20;
int turn = 0;

void gotoxy(int x, int y);
void ship_show(int x, int y, int dir, int size_ship);
void shooting_show(int x, int y);
bool ship_in_map(int x, int y, int dir, int size_ship);
bool aim_in_map(int x, int y);
bool set_ship(int map[N][N], int x, int y, int dir, int size_ship);
void map_show(int mass[][N], int map2[][N]);
void Player_ships_alignment(int map[][N], int map2[][N], int size_ship);
void ship_rotate_player(int map[][N], int map2[][N]);
void CPU_ships_alignment(int map[][N], int ship_rate, int ship_count);
void ship_rotate_cpu(int map[][N]);
void player_shoot(int player_map[][N], int cpu_map[][N], int mask[][N]);
void CPU_shoot(int map[][N]);
void splash_screen();

int main() {

	srand(time(NULL));
	int cpu_map[N][N] = { 0 };
	int player_map[N][N] = { 0 };
	int mask[N][N] = { 0 };
	int selection;
	bool exit = 1;
	while (exit) {
		splash_screen();
		cin >> selection;
		system("cls");


		switch (selection) {
		case 1:
			ship_rotate_cpu(cpu_map);
			ship_rotate_player(player_map, mask);
			while (true) {
				player_shoot(player_map, cpu_map, mask);
				CPU_shoot(player_map);
			}
			break;
		case 2:
			ship_rotate_player(player_map, mask);
			ship_rotate_cpu(cpu_map);
			while (true) {
				CPU_shoot(player_map);
				player_shoot(player_map, cpu_map, mask);
			}
			break;
		case 3:
			cout << "Управление:\nw - вверх\ns - вниз\na - влево\nd - вправо\nr - поворот корабля\nEnter - установка корабля/выстрел\n\nПравила как в классическом морском бое.\n";
			system("pause");

			break;
		case 4:
			exit = 0;
			break;
		}
	}
	system("pause");
	return 0;
}

void ship_show(int x, int y, int dir, int size_ship) {
	for (int i = 0; i < size_ship; ++i) {
		gotoxy(x + 3, y + 5);
		cout << '\xDB';
		switch (dir) {
		case 0:
			x++;
			break;
		case 1:
			y++;
			break;
		case 2:
			x--;
			break;
		case 3:
			y--;
			break;

		}
	}
}

void gotoxy(int x, int y) {
	COORD p = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), p);
}

void shooting_show(int x, int y) {
	gotoxy(x + 25, y + 5);
	cout << '+';
}

bool ship_in_map(int x, int y, int dir, int size_ship) {

	bool in_map = 1;
	for (int i = 0; i < size_ship; ++i) {
		if (x < 0 || y < 0 || x >= N || y >= N) {
			in_map = 0;
			break;
		}

		switch (dir) {
		case 0:
			x++;
			break;
		case 1:
			y++;
			break;
		case 2:
			x--;
			break;
		case 3:
			y--;
			break;
		}
	}
	return in_map;
}

bool aim_in_map(int x, int y) {
	bool in_map = 1;
	if (x < 0 || y < 0 || x >= N || y >= N) {
		in_map = 0;
	}
	return in_map;
}

bool set_ship(int map[N][N], int x, int y, int dir, int size_ship) {

	int temp_x = x;
	int temp_y = y;

	bool setting_is_possible = 1;
	//Проверка возможности постановки корабля
	for (int i = 0; i < size_ship; ++i) {
		if (x < 0 || y < 0 || x >= N || y >= N) {
			setting_is_possible = 0;
			break;
		}
		if (map[x][y] >= 1) {
			setting_is_possible = 0;
			break;
		}
		if (y < N - 1)
			if (map[x][y + 1] >= 1) {
				setting_is_possible = 0;
				break;
			}
		if (y<0)
			if (map[x][y - 1] >= 1) {
				setting_is_possible = 0;
				break;
			}
		if (x < N - 1)
			if (map[x + 1][y] >= 1) {
				setting_is_possible = 0;
				break;
			}
		if (x < N - 1 && y < N - 1)
			if (map[x + 1][y + 1] >= 1) {
				setting_is_possible = 0;
				break;
			}
		if (x < N - 1 && y > 0)
			if (map[x + 1][y - 1] >= 1) {
				setting_is_possible = 0;
				break;
			}
		if (x > 0)
			if (map[x - 1][y] >= 1) {
				setting_is_possible = 0;
				break;
			}
		if (x > 0 && y > N - 1)
			if (map[x - 1][y + 1] >= 1) {
				setting_is_possible = 0;
				break;
			}
		if (x > 0 && y > 0)
			if (map[x - 1][y - 1] >= 1) {
				setting_is_possible = 0;
				break;
			}
		if (map[x][y - 1] >= 1) {
			setting_is_possible = 0;
			break;
		}
		if (map[x - 1][y + 1] >= 1) {
			setting_is_possible = 0;
			break;
		}

		switch (dir) {
		case 0:
			x++;
			break;
		case 1:
			y++;
			break;
		case 2:
			x--;
			break;
		case 3:
			y--;
			break;
		}
	}


	if (setting_is_possible == 1) {
		x = temp_x;
		y = temp_y;
		for (int i = 0; i < size_ship; ++i) {
			map[x][y] = ships_ID;
			switch (dir) {
			case 0:
				x++;
				break;
			case 1:
				y++;
				break;
			case 2:
				x--;
				break;
			case 3:
				y--;
				break;

			}
		}
		ships[ships_ID] = size_ship;
		ships_ID++;
	}
	return setting_is_possible;
}


void map_show(int map[][N], int map2[][N]) {
	cout << "       Координаты стрельбы игрока:\n";
	if (show_x == 0) cout << "           По вертикали = " << "А" << "\n";
	if (show_x == 1) cout << "           По вертикали = " << "Б" << "\n";
	if (show_x == 2) cout << "           По вертикали = " << "В" << "\n";
	if (show_x == 3) cout << "           По вертикали = " << "Г" << "\n";
	if (show_x == 4) cout << "           По вертикали = " << "Д" << "\n";
	if (show_x == 5) cout << "           По вертикали = " << "Е" << "\n";
	if (show_x == 6) cout << "           По вертикали = " << "Ж" << "\n";
	if (show_x == 7) cout << "           По вертикали = " << "З" << "\n";
	if (show_x == 8) cout << "           По вертикали = " << "И" << "\n";
	if (show_x == 9) cout << "           По вертикали = " << "К" << "\n";
	cout << "          По горизонали = " << show_y + 1 << "\t";
	cout << endl;

	cout << "   АБВГДЕЖЗИК            АБВГДЕЖЗИК\n";
	setlocale(LC_ALL, "C");
	cout << "  \xC9";
	for (int i = 0; i < 10; ++i) cout << "\xCD";
	cout << "\xBB          \xC9";
	for (int i = 0; i < 10; ++i) cout << "\xCD";
	cout << "\xBB\n";
	for (int i = 0; i < 10; ++i) {
		if (i < 9) cout << " " << i + 1 << "\xBA";
		else cout << i + 1 << "\xBA";
		for (int j = 0; j < 10; ++j) {
			if (map[j][i] == 0) cout << "-";
			if (map[j][i] > 0) cout << "\xDB";
			if (map[j][i] == -1) cout << "X";
			if (map[j][i] < -1) cout << "*";
		}
		if (i < 9) cout << "\xBA         " << i + 1 << "\xBA";
		else { cout << "\xBA        " << i + 1 << "\xBA"; }
		for (int j = 0; j < 10; ++j) {
			if (map2[j][i] == 0) cout << "-";
			if (map2[j][i] < 0) cout << "X";
			if (map2[j][i] > 0) cout << "*";
		}
		cout << "\xBA";
		cout << endl;
	}
	cout << "  \xC8";
	for (int i = 0; i < 10; ++i) cout << "\xCD";
	cout << "\xBC          \xC8";
	for (int i = 0; i < 10; ++i) cout << "\xCD";
	cout << "\xBC\n\n";
	setlocale(LC_ALL, "Russian");
	cout << "Кораблей игрока в строю : " << player_ships << endl;
	cout << "Неповрежденных палуб игрока : " << player_decks << endl << endl;
	cout << "Кораблей CPU в строю : " << cpu_ships << endl;
	cout << "Неповрежденных палуб СPU : " << cpu_decks << endl << endl;
	cout << "Ход : " << turn;
	cout << endl;
}

void Player_ships_alignment(int map[][N], int map2[][N], int size_ship) {

	int x = 0, y = 0;
	int dir = 0;
	int count = size_ship;
	int ch;

	while (size_ship == count) {
		setlocale(LC_ALL, "Russian");
		map_show(map, map2);
		setlocale(LC_ALL, "C");
		ship_show(x, y, dir, size_ship);

		int temp_x = x;
		int temp_y = y;
		int temp_dir = dir;


		ch = _getch();

		// Изменить координаты или направление
		switch (ch) {
		case 100: // d вправо
			x++;
			break;
		case 115:  // s вниз
			y++;
			break;
		case 97:  // a влево
			x--;
			break;
		case 119: // w вверх
			y--;
			break;
		case 114: // r поворот
			dir = !dir;
			break;
		case 13: // Enter установка корабля

			if (set_ship(map, x, y, dir, size_ship)) {
				x = 0;
				y = 0;
				dir = 0;
				player_ships += 1;
				player_decks += size_ship;
				size_ship--;
			}
			break;

		}

		if (!ship_in_map(x, y, dir, size_ship)) {
			x = temp_x;
			y = temp_y;
			dir = temp_dir;
		}

		system("cls");
	}
}

void ship_rotate_player(int map[][N], int map2[][N]) {
	int size_ship = 4;
	Player_ships_alignment(map, map2, size_ship);
	size_ship--;
	Player_ships_alignment(map, map2, size_ship);
	Player_ships_alignment(map, map2, size_ship);
	size_ship--;
	Player_ships_alignment(map, map2, size_ship);
	Player_ships_alignment(map, map2, size_ship);
	Player_ships_alignment(map, map2, size_ship);
	size_ship--;
	Player_ships_alignment(map, map2, size_ship);
	Player_ships_alignment(map, map2, size_ship);
	Player_ships_alignment(map, map2, size_ship);
	Player_ships_alignment(map, map2, size_ship);
	setlocale(LC_ALL, "Russian");
	cout << "Все корабли размещены!!! Начинается \"Морской Бой\"!!!";
	Sleep(1500);
	system("cls");
}

void CPU_ships_alignment(int map[][N], int ship_rate, int ship_count) {
	bool setting_is_possible = 1;
	int x, y;
	int dir = 0;
	int temp_x;
	int temp_y;
	int count_ship = 0;
	while (count_ship < ship_count) {
		do {
			x = rand() % N;
			y = rand() % N;

			temp_x = x;
			temp_y = y;

			dir = rand() % 4;

			setting_is_possible = 1;
			for (int i = 0; i < ship_rate; ++i) {
				if (x < 0 || y < 0 || x >= N || y >= N) {
					setting_is_possible = 0;
					break;
				}
				if (map[x][y] >= 1 ||
					map[x][y + 1] >= 1 ||
					map[x][y - 1] >= 1 ||
					map[x + 1][y] >= 1 ||
					map[x + 1][y + 1] >= 1 ||
					map[x + 1][y - 1] >= 1 ||
					map[x - 1][y] >= 1 ||
					map[x - 1][y + 1] >= 1 ||
					map[x - 1][y - 1] >= 1)
				{
					setting_is_possible = 0;
					break;
				}

				switch (dir) {
				case 0:
					x++;
					break;
				case 1:
					y++;
					break;
				case 2:
					x--;
					break;
				case 3:
					y--;
					break;
				}
			}
		} while (setting_is_possible != 1);

		if (setting_is_possible == 1) {
			x = temp_x;
			y = temp_y;
			for (int i = 0; i < ship_rate; ++i) {
				map[x][y] = cpu_ships_ID;
				switch (dir) {
				case 0:
					x++;
					break;
				case 1:
					y++;
					break;
				case 2:
					x--;
					break;
				case 3:
					y--;
					break;

				}
			}
		}
		ships_cpu[cpu_ships_ID] = ship_rate;
		cpu_ships_ID++;
		count_ship++;
	}

}

void ship_rotate_cpu(int map[][N]) {
	int ship_rate = 4;
	int ship_count = 1;
	CPU_ships_alignment(map, ship_rate, ship_count);
	--ship_rate;
	++ship_count;
	CPU_ships_alignment(map, ship_rate, ship_count);
	--ship_rate;
	++ship_count;
	CPU_ships_alignment(map, ship_rate, ship_count);
	--ship_rate;
	++ship_count;
	CPU_ships_alignment(map, ship_rate, ship_count);
}

void player_shoot(int player_map[][N], int cpu_map[][N], int mask[][N]) {
	int x = 0;
	int y = 0;
	int ch;
	bool again = 1;
	while (again) {
		bool shoot = 1;
		while (shoot) {
			map_show(player_map, mask);
			shooting_show(x, y);
			int temp_x = x;
			int temp_y = y;


			ch = _getch();

			// Изменить координаты или направление
			switch (ch) {
			case 100: // d вправо
				x++;
				break;
			case 115:  // s вниз
				y++;
				break;
			case 97:  // a влево
				x--;
				break;
			case 119: // w вверх
				y--;
				break;
			case 13:
				shoot = 0;
				break;
			}

			if (!aim_in_map(x, y)) {
				x = temp_x;
				y = temp_y;
			}

			show_x = x;
			show_y = y;
			system("cls");

		}

		if (show_x == 0) cout << "Выстрел по координатам: " << "А-" << show_y + 1;
		if (show_x == 1) cout << "Выстрел по координатам: " << "Б-" << show_y + 1;
		if (show_x == 2) cout << "Выстрел по координатам: " << "В-" << show_y + 1;
		if (show_x == 3) cout << "Выстрел по координатам: " << "Г-" << show_y + 1;
		if (show_x == 4) cout << "Выстрел по координатам: " << "Д-" << show_y + 1;
		if (show_x == 5) cout << "Выстрел по координатам: " << "Е-" << show_y + 1;
		if (show_x == 6) cout << "Выстрел по координатам: " << "Ж-" << show_y + 1;
		if (show_x == 7) cout << "Выстрел по координатам: " << "З-" << show_y + 1;
		if (show_x == 8) cout << "Выстрел по координатам: " << "И-" << show_y + 1;
		if (show_x == 9) cout << "Выстрел по координатам: " << "К-" << show_y + 1;
		Sleep(1000);
		system("cls");

		if (cpu_map[x][y] >= 1) {
			ships_cpu[cpu_map[x][y]]--;

			if (ships_cpu[cpu_map[x][y]] <= 0) {
				cout << "Корабль противника потоплен!" << endl;
				turn += 1;
				cpu_ships -= 1;
				cpu_decks -= 1;
				Sleep(1500);
				system("cls");
				cout << "Делайте дополнительный выстрел!";
				Sleep(1500);
				system("cls");
			}
			else {
				cout << "Корабль противника ранен!" << endl;
				turn += 1;
				cpu_decks -= 1;
				Sleep(1500);
				system("cls");
				cout << "Делайте дополнительный выстрел!";
				turn += 1;
				Sleep(1500);
				system("cls");
			}

			cpu_map[x][y] = -1;
			mask[x][y] = -1;
		}
		else {
			mask[x][y] = 1;
			cout << "Вы промахнулись!!!" << endl;
			again = 0;
			Sleep(1500);
			system("cls");
		}
	}
}


void CPU_shoot(int map[][N]) {
	cout << "Выстрел CPU: ";
	bool again = 1;
	while (again) {
		int x, y;

		bool shoot_is_right = 1;
		do {
			x = rand() % N;
			y = rand() % N;
			Sleep(200);
			if (CPU_shooting_matrix[x][y] == 0) {
				shoot_is_right = 0;
			}
		} while (shoot_is_right);

		if (x == 0) cout << "Координаты: А - " << y + 1;
		if (x == 1) cout << "Координаты: Б - " << y + 1;
		if (x == 2) cout << "Координаты: В - " << y + 1;
		if (x == 3) cout << "Координаты: Г - " << y + 1;
		if (x == 4) cout << "Координаты: Д - " << y + 1;
		if (x == 5) cout << "Координаты: Е - " << y + 1;
		if (x == 6) cout << "Координаты: Ж - " << y + 1;
		if (x == 7) cout << "Координаты: З - " << y + 1;
		if (x == 8) cout << "Координаты: И - " << y + 1;
		if (x == 9) cout << "Координаты: К - " << y + 1;
		Sleep(1000);
		system("cls");
		cout << "Выстрел CPU: ";
		if (map[x][y] >= 1) {
			ships[map[x][y]]--;

			if (ships[map[x][y]] <= 0) {
				cout << "Ваш корабль потоплен!" << endl;
				CPU_shooting_matrix[x][y] = -1;
				turn += 1;
				player_ships -= 1;
				player_decks -= 1;
				map[x][y] = -1;
				Sleep(1500);
				system("cls");
				cout << "CPU стреляет еще раз!";

			}
			else {
				cout << "Ваш корабль ранен!" << endl;
				CPU_shooting_matrix[x][y] = -1;
				turn += 1;
				player_decks -= 1;
				Sleep(1500);
				system("cls");
				cout << "CPU стреляет еще раз!";
			}

			map[x][y] = -1;

		}
		else {
			cout << "CPU промахнулся!" << endl;
			turn += 1;
			CPU_shooting_matrix[x][y] = -1;
			map[x][y] = -2;
			again = 0;
		}

		Sleep(1500);
		system("cls");
	}

}

void splash_screen() {
	setlocale(LC_ALL, "C");
	system("cls");
	printf("\xC9");
	for (int i = 0; i < 108; ++i) {
		printf("\xCD");
	}
	printf("\xBB\n\xBA");
	for (int i = 0; i < 108; ++i) {
		printf(" ");
	}
	printf("\xBA\n\xBA");
	for (int i = 0; i < 50; ++i) {
		printf(" ");
	}
	printf("       \xFE\xFE\xFE\xFE                    \xFE\xFE\xFE\xFE                       \xBA\n\xBA");
	for (int i = 0; i < 108; ++i) {
		printf(" ");
	}
	printf("\xBA\n\xBA             \xDB\xDB   \xDB\xDB  \xDB\xDB\xDB\xDB  \xDB\xDB\xDB\xDB   \xDB\xDB\xDB\xDB  \xDB\xDB  \xDB\xDB  \xDB\xDB\xDB\xDB  \xDB\xDB  \xDB\xDB      \xDB\xDB\xDB\xDB\xDB  \xDB\xDB\xDB\xDB  \xDB\xDB  \xDB\xDB                      \xBA\n");
	printf("\xBA             \xDB\xDB   \xDB\xDB \xDB\xDB  \xDB\xDB \xDB\xDB \xDB\xDB \xDB\xDB  \xDB\xDB \xDB\xDB  \xDB\xDB \xDB\xDB  \xDB\xDB \xDB\xDB \xDB\xDB\xDB      \xDB\xDB    \xDB\xDB  \xDB\xDB \xDB\xDB \xDB\xDB\xDB                      \xBA\n");
	printf("\xBA             \xDB\xDB\xDB \xDB\xDB\xDB \xDB\xDB  \xDB\xDB \xDB\xDB \xDB\xDB \xDB\xDB     \xDB\xDB\xDB\xDB   \xDB\xDB  \xDB\xDB \xDB\xDB\xDB\xDB\xDB\xDB      \xDB\xDB\xDB\xDB\xDB \xDB\xDB  \xDB\xDB \xDB\xDB\xDB\xDB\xDB\xDB                      \xBA\n");
	printf("\xBA             \xDB\xDB\xDB\xDB\xDB\xDB\xDB \xDB\xDB  \xDB\xDB \xDB\xDB\xDB   \xDB\xDB  \xDB\xDB \xDB\xDB  \xDB\xDB \xDB\xDB  \xDB\xDB \xDB\xDB\xDB  \xDB      \xDB\xDB  \xDB \xDB\xDB  \xDB\xDB \xDB\xDB\xDB  \xDB                      \xBA\n");
	printf("\xBA             \xDB\xDB \xDB \xDB\xDB  \xDB\xDB\xDB\xDB  \xDB\xDB     \xDB\xDB\xDB\xDB  \xDB\xDB  \xDB\xDB  \xDB\xDB\xDB\xDB  \xDB\xDB  \xDB\xDB      \xDB\xDB\xDB\xDB\xDB  \xDB\xDB\xDB\xDB  \xDB\xDB  \xDB\xDB                      \xBA\n");
	printf("\xBA");
	for (int i = 0; i < 108; ++i) {
		printf(" ");
	}
	printf("\xBA\n\xBA");
	for (int i = 0; i < 50; ++i) {
		printf(" ");
	}
	printf("(c) IT-Step Exam work. All rights reserved. 2017          \xBA\n\xBA");
	for (int i = 0; i < 108; ++i) {
		printf(" ");
	}
	printf("\xBA\n\xC8");
	for (int i = 0; i < 108; ++i) {
		printf("\xCD");
	}
	printf("\xBC\n\n\n");
	setlocale(LC_ALL, "Russian");
	cout << "    МЕНЮ ИГРЫ:\n\n";
	cout << " 1. Игрок начинает.\n";
	cout << " 2. Компьютер начинает.\n";
	cout << " 3. Правила игры.\n";
	cout << " 4. Выход.\n\n";
	cout << "Ваш выбор (1-4): ";

}