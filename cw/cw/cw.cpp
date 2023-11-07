#include "settings.h"
#include "fuctions.h"

// enumeration - перечисление
enum Derections { LEFT = 75, RIGHT = 77, DOWN = 80, UP = 72 };
enum colors { BLACK = 0, GRAY = 8, YELLOW = 14, RED = 12, PINK = 13, GREEN = 2 };
enum elems { HALL = 0, WALL = 1, COIN = 2, ENEMY = 3, MEDICINE_KIT = 4 };

int main()
{
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE); //уникальный номер окна консоли (дескриптор)
	system("title Maze"); // меняем название файла в консоли

	HideCursor(false, 100); // скрываем мигающий курсор в консоли

	srand(time(0));
	const int width = 60, height = 20;
	int maze[height][width] = {};

	// коридоры - 0     стены - 1     монетки - 2     враги - 3     аптечки - 4

	///////////////////////////////////////////////////////////////////////
	// установка лабиринта

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			maze[y][x] = rand() % 5;

			walls(x, y, width, height, maze, WALL); // установка стен

			entry_exit(x, y, width, height, maze, HALL); // определение точки входа и выхода

			check(x, y, maze, ENEMY, HALL, 10); // уменьшение вероятности появления врага (10%)
			
			check(x, y, maze, MEDICINE_KIT, HALL, 20); // уменьшение вероятности появления автечки (5%)

			if (maze[y][x] == HALL) {
				SetCursor(x, y, BLACK, " ");
			}
			else if (maze[y][x] == WALL) { 
				SetCursorChar(x, y, GRAY, (char)177); 
			}
			else if (maze[y][x] == COIN) {
				SetCursorChar(x, y, YELLOW, (char)249);
			}
			else if (maze[y][x] == ENEMY) {
				SetCursorChar(x, y, RED, (char)1);
			}
			else if (maze[y][x] == MEDICINE_KIT) {
				SetCursor(x, y, GREEN, "+");
			}
			else {
				cout << maze[y][x];
			}
		}
		cout << endl;
	}

	///////////////////////////////////////////////////////////////////////
	// управление персонажем

	// установка курсора в любую координату консоли
	COORD position;
	position.X = 0;
	position.Y = 2;
	SetCursorChar(0, 2, PINK, (char)1);

	int coins = 0; // количество подобраных монет
	int health = 100; // здоровье
	bool lose = false; // индикатор проигрыша
	bool write = false; // показатель прошлого шага (нужно ли вернуть аптечку или нет)

	while (true) {

		///////////////////////////////////////////////////////////////////////
		// показатель здоровья

		COORD health_info;
		health_info.X = width + 1;
		health_info.Y = 3;
		SetConsoleCursorPosition(h, health_info);
		SetConsoleTextAttribute(h, YELLOW); // желтый
		cout << "HEALTH - " << health << "%";

		///////////////////////////////////////////////////////////////////////
		// передвижение

		int code = _getch(); // get character - получение кода нажатой клавиши
		if (code == 224) // если была нажата стрелочка
		{
			code = _getch();
		}

		SetCursor(position.X, position.Y, BLACK, " ");

		COORD kit_pos; // создание пустых координат аптечки
		kit_pos.X = 0;
		kit_pos.Y = 0;
		if (write) // 2 этап процесса возврата аптечки
		{
			kit_pos.X = position.X; // записывание координат аптечки (до шага)
			kit_pos.Y = position.Y;
			write = false; // обнуление переменной после записи координат
		}

		if (code == RIGHT && maze[position.Y][position.X + 1] != WALL) {
			position.X++; // смещение вправо
		}
		else if (code == LEFT && maze[position.Y][position.X - 1] != WALL) {
			position.X--; // смещение влево
		}
		else if (code == UP && maze[position.Y - 1][position.X] != WALL) {
			position.Y--; // смещение вверх
		}
		else if (code == DOWN && maze[position.Y + 1][position.X] != WALL) {
			position.Y++; // смещение вниз
		}

		if (kit_pos.X > 0) // если доступны координаты аптечки, возвращаем ее на место (3 этап процесса возврата аптечки)
		{
			SetCursor(kit_pos.X, kit_pos.Y, GREEN, "+");
		}

		if (maze[position.Y][position.X] == ENEMY) // проверка наличия врага
		{
			health -= 25;
			maze[position.Y][position.X] = HALL;
			health_info.X += 12;
			SetCursor(health_info.X, health_info.Y, YELLOW, " ");
			if (health == 0) {
				lose = true;
				MessageBoxA(0, "Закончилось здоровье!", "Поражение :(", MB_OK);
				break;
			}
		}
		
		if (maze[position.Y][position.X] == MEDICINE_KIT && health != 100) // использование аптечки
		{
			health += 25;
			maze[position.Y][position.X] = HALL;
		}
		else if (maze[position.Y][position.X] == MEDICINE_KIT && health == 100) // 1 этап процесса возврата аптечки
		{
			write = true;
		}

		SetCursorChar(position.X, position.Y, PINK, (char)1);

		///////////////////////////////////////////////////////////////////////
		// проверка на выход из лабиринта

		if (position.X == width - 1 && position.Y == height - 3) {
			MessageBoxA(0, "Найден выход из лабиринта!", "ПОБЕДА!", MB_OK);
			break;
		}

		///////////////////////////////////////////////////////////////////////
		// проверка на подбор монетки

		if (maze[position.Y][position.X] == COIN) {
			coins++;
			maze[position.Y][position.X] = HALL;
			SetCursor(width + 1, 1, YELLOW, "COINS - ");
			SetCursor(width + 9, 1, YELLOW, to_string(coins));
		}
	}

	EndProgram(lose);
}
