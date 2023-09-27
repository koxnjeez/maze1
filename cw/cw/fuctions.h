void SetCursor(int x, int y, int color, string text)
{
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE); //���������� ����� ���� ������� (����������)
    COORD position;
    position.X = x; // ��������� ���������
    position.Y = y;
    SetConsoleCursorPosition(h, position); // ��������� ������� � ����������
	SetConsoleTextAttribute(h, color); // ��������� �����
	cout << text; // ����� ������ ������
}

void HideCursor(bool vision, int size) {
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	// �������� �������� ������ � �������
	CONSOLE_CURSOR_INFO info;
	info.bVisible = false;
	info.dwSize = 100;
	SetConsoleCursorInfo(h, &info);
}

void EndProgram(bool fail) {
	system("cls"); // ������� ������
	if (fail == false) {
		cout << "You win!!!\n\n"; // main() - ���������� ����������
	}
	else {
		cout << "You lost :(\n\n"; // main() - ���������� ����������
	}
	Sleep(INFINITE); // ����� �� ���� ������� ���� ������������ �� ������� ���������
}

void walls(int x, int y, int width, int height, int arr[][60], int elem) {
	if (x == 0 || y == 0 || x == width - 1 || y == height - 1) {
		arr[y][x] = elem;
	}
}

void entry_exit(int x, int y, int width, int height, int arr[][60], int elem) {
	if (x == 0 && y == 2 || x == 1 && y == 2 || x == 2 && y == 2 || x == width - 1 && y == height - 3 || x == width - 2 && y == height - 3 || x == width - 3 && y == height - 3) {
		arr[y][x] = elem;
	}
}

void check(int x, int y, int arr[][60], int item, int elem, int probability) {
	if (arr[y][x] == item) {
		int r;
		r = rand() % probability;
		if (r != 0)
		{
			arr[y][x] = elem;
		}
	}
}