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
			cout << "����������:\nw - �����\ns - ����\na - �����\nd - ������\nr - ������� �������\nEnter - ��������� �������/�������\n\n������� ��� � ������������ ������� ���.\n";
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
	//�������� ����������� ���������� �������
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
	cout << "       ���������� �������� ������:\n";
	if (show_x == 0) cout << "           �� ��������� = " << "�" << "\n";
	if (show_x == 1) cout << "           �� ��������� = " << "�" << "\n";
	if (show_x == 2) cout << "           �� ��������� = " << "�" << "\n";
	if (show_x == 3) cout << "           �� ��������� = " << "�" << "\n";
	if (show_x == 4) cout << "           �� ��������� = " << "�" << "\n";
	if (show_x == 5) cout << "           �� ��������� = " << "�" << "\n";
	if (show_x == 6) cout << "           �� ��������� = " << "�" << "\n";
	if (show_x == 7) cout << "           �� ��������� = " << "�" << "\n";
	if (show_x == 8) cout << "           �� ��������� = " << "�" << "\n";
	if (show_x == 9) cout << "           �� ��������� = " << "�" << "\n";
	cout << "          �� ���������� = " << show_y + 1 << "\t";
	cout << endl;

	cout << "   ����������            ����������\n";
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
	cout << "�������� ������ � ����� : " << player_ships << endl;
	cout << "�������������� ����� ������ : " << player_decks << endl << endl;
	cout << "�������� CPU � ����� : " << cpu_ships << endl;
	cout << "�������������� ����� �PU : " << cpu_decks << endl << endl;
	cout << "��� : " << turn;
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

		// �������� ���������� ��� �����������
		switch (ch) {
		case 100: // d ������
			x++;
			break;
		case 115:  // s ����
			y++;
			break;
		case 97:  // a �����
			x--;
			break;
		case 119: // w �����
			y--;
			break;
		case 114: // r �������
			dir = !dir;
			break;
		case 13: // Enter ��������� �������

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
	cout << "��� ������� ���������!!! ���������� \"������� ���\"!!!";
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

			// �������� ���������� ��� �����������
			switch (ch) {
			case 100: // d ������
				x++;
				break;
			case 115:  // s ����
				y++;
				break;
			case 97:  // a �����
				x--;
				break;
			case 119: // w �����
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

		if (show_x == 0) cout << "������� �� �����������: " << "�-" << show_y + 1;
		if (show_x == 1) cout << "������� �� �����������: " << "�-" << show_y + 1;
		if (show_x == 2) cout << "������� �� �����������: " << "�-" << show_y + 1;
		if (show_x == 3) cout << "������� �� �����������: " << "�-" << show_y + 1;
		if (show_x == 4) cout << "������� �� �����������: " << "�-" << show_y + 1;
		if (show_x == 5) cout << "������� �� �����������: " << "�-" << show_y + 1;
		if (show_x == 6) cout << "������� �� �����������: " << "�-" << show_y + 1;
		if (show_x == 7) cout << "������� �� �����������: " << "�-" << show_y + 1;
		if (show_x == 8) cout << "������� �� �����������: " << "�-" << show_y + 1;
		if (show_x == 9) cout << "������� �� �����������: " << "�-" << show_y + 1;
		Sleep(1000);
		system("cls");

		if (cpu_map[x][y] >= 1) {
			ships_cpu[cpu_map[x][y]]--;

			if (ships_cpu[cpu_map[x][y]] <= 0) {
				cout << "������� ���������� ��������!" << endl;
				turn += 1;
				cpu_ships -= 1;
				cpu_decks -= 1;
				Sleep(1500);
				system("cls");
				cout << "������� �������������� �������!";
				Sleep(1500);
				system("cls");
			}
			else {
				cout << "������� ���������� �����!" << endl;
				turn += 1;
				cpu_decks -= 1;
				Sleep(1500);
				system("cls");
				cout << "������� �������������� �������!";
				turn += 1;
				Sleep(1500);
				system("cls");
			}

			cpu_map[x][y] = -1;
			mask[x][y] = -1;
		}
		else {
			mask[x][y] = 1;
			cout << "�� ������������!!!" << endl;
			again = 0;
			Sleep(1500);
			system("cls");
		}
	}
}


void CPU_shoot(int map[][N]) {
	cout << "������� CPU: ";
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

		if (x == 0) cout << "����������: � - " << y + 1;
		if (x == 1) cout << "����������: � - " << y + 1;
		if (x == 2) cout << "����������: � - " << y + 1;
		if (x == 3) cout << "����������: � - " << y + 1;
		if (x == 4) cout << "����������: � - " << y + 1;
		if (x == 5) cout << "����������: � - " << y + 1;
		if (x == 6) cout << "����������: � - " << y + 1;
		if (x == 7) cout << "����������: � - " << y + 1;
		if (x == 8) cout << "����������: � - " << y + 1;
		if (x == 9) cout << "����������: � - " << y + 1;
		Sleep(1000);
		system("cls");
		cout << "������� CPU: ";
		if (map[x][y] >= 1) {
			ships[map[x][y]]--;

			if (ships[map[x][y]] <= 0) {
				cout << "��� ������� ��������!" << endl;
				CPU_shooting_matrix[x][y] = -1;
				turn += 1;
				player_ships -= 1;
				player_decks -= 1;
				map[x][y] = -1;
				Sleep(1500);
				system("cls");
				cout << "CPU �������� ��� ���!";

			}
			else {
				cout << "��� ������� �����!" << endl;
				CPU_shooting_matrix[x][y] = -1;
				turn += 1;
				player_decks -= 1;
				Sleep(1500);
				system("cls");
				cout << "CPU �������� ��� ���!";
			}

			map[x][y] = -1;

		}
		else {
			cout << "CPU �����������!" << endl;
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
	cout << "    ���� ����:\n\n";
	cout << " 1. ����� ��������.\n";
	cout << " 2. ��������� ��������.\n";
	cout << " 3. ������� ����.\n";
	cout << " 4. �����.\n\n";
	cout << "��� ����� (1-4): ";

}