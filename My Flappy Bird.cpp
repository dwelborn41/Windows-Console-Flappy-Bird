//Made by dwelborn41
//Inspired by javidx9

#include <iostream>
#include <limits>
#include <stdexcept>
#include <windows.h>
#include <thread>
#include <list>

using namespace std;

const int nScreenWidth = 80;
const int nScreenHeight = 50;
wchar_t* screen = new wchar_t[nScreenWidth * nScreenHeight];

//Textures
wchar_t Light = wchar_t(0x2591);
wchar_t Medium = wchar_t(0x2592);
wchar_t Dark = wchar_t(0x2593);
wchar_t Full = wchar_t(0x2588);



void debug(wchar_t* screen, char myChar1, char myChar2) {
	for (int i = 2; i <= 8; i++) {
		screen[1 * nScreenWidth + i] = L'*';
		screen[3 * nScreenWidth + i] = L'*';
	}
	screen[2 * nScreenWidth + 2] = L'*';
	screen[2 * nScreenWidth + 8] = L'*';
	screen[2 * nScreenWidth + 4] = (myChar1);
	screen[2 * nScreenWidth + 6] = (myChar2);;
}
void debug(wchar_t* screen) {
	screen[2 * nScreenWidth + 4] = L' ';
}

//----------------------------------------------------------------------------
struct console
{
	console(unsigned width, unsigned height)
	{
		SMALL_RECT r;
		COORD      c;
		hConOut = GetStdHandle(STD_OUTPUT_HANDLE);
		if (!GetConsoleScreenBufferInfo(hConOut, &csbi))
			throw runtime_error("You must be attached to a human.");

		r.Left =
			r.Top = 0;
		r.Right = width - 1;
		r.Bottom = height - 1;
		SetConsoleWindowInfo(hConOut, TRUE, &r);

		c.X = width;
		c.Y = height;
		SetConsoleScreenBufferSize(hConOut, c);
	}

	~console()
	{
		SetConsoleTextAttribute(hConOut, csbi.wAttributes);
		SetConsoleScreenBufferSize(hConOut, csbi.dwSize);
		SetConsoleWindowInfo(hConOut, TRUE, &csbi.srWindow);
	}

	void color(WORD color = 0x07)
	{
		SetConsoleTextAttribute(hConOut, color);
	}

	HANDLE                     hConOut;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
};
//----------------------------------------------------------------------------
console con(nScreenWidth, nScreenHeight);
//----------------------------------------------------------------------------

void ToScreen(wchar_t* screen, int x, int y, wchar_t myChar) {
	if (!(0 <= x && x < nScreenWidth))
		return;
	if (!(0 <= y && y < nScreenHeight))
		return;
	screen[y * nScreenWidth + x] = myChar;
}

struct groundCoord {
	int x;
	int y;
	int texture;
};
struct coord {
	int x;
	int y;
};

int pipeGap = 9;
class pipe {
public:
	int x;
	int y;
	list<coord> graphics;

	//resets pipe with random height.
	void reset() {
		x = nScreenWidth;
		y = rand() % (nScreenHeight - 25) + 15;
	}

	void set() {
		graphics.clear();
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < (y - pipeGap); j++) {
				graphics.push_back({ x + i,j });
			}
			for (int j = y; j < nScreenHeight - 6; j++) {
				graphics.push_back({ x + i,j });
			}
			//Pipe flare at ends
			graphics.push_back({ x - 1,y });
			graphics.push_back({ x - 1,y + 1 });
			graphics.push_back({ x + 8,y });
			graphics.push_back({ x + 8,y + 1 });
			graphics.push_back({ x - 1,y - pipeGap - 1 });
			graphics.push_back({ x - 1,y - pipeGap - 2 });
			graphics.push_back({ x + 8,y - pipeGap - 1 });
			graphics.push_back({ x + 8,y - pipeGap - 2 });
		}
	}
	// moves pipe to left and setes graphics list
	void nextGraphics() {
		set();
		x--;
	}

	//prints to screen
	void print() {
		for (auto g : graphics) {
			ToScreen(screen, g.x, g.y, Full);
		}
	}

};

class birdie {
public:
	int x;
	int y;
	double posY;
	double velY;
	double velMax;

	//collision coords
	list<coord> birdBox;

	void setBirdBox() {
		birdBox.clear();

		birdBox.push_back({ x, y });
		birdBox.push_back({ x, y + 1 });
		birdBox.push_back({ x - 4, y });
		birdBox.push_back({ x - 4, y + 1 });

		birdBox.push_back({ x - 1, y - 1 });
		birdBox.push_back({ x - 2, y - 1 });
		birdBox.push_back({ x - 3, y - 1 });

		birdBox.push_back({ x - 1, y + 2 });
		birdBox.push_back({ x - 2, y + 2 });
		birdBox.push_back({ x - 3, y + 2 });
	}

	int animation = 25;
	//call each frame print for animation
	void print() {
		if (animation < 4)
			printN();
		else if (animation < 8)
			printD();
		else if (animation < 15)
			printN();
		else
			printU();
		if (animation < 15)
			animation++;

	}

	//still frames used in print
	void printN() {
		ToScreen(screen, x, y, L'@');
		ToScreen(screen, x - 1, y, L' ');
		ToScreen(screen, x - 2, y, L' ');
		ToScreen(screen, x - 3, y, L' ');
		ToScreen(screen, x - 1, y - 1, L'*');
		ToScreen(screen, x - 2, y - 1, L'*');
		ToScreen(screen, x - 3, y - 1, L'*');
		ToScreen(screen, x - 4, y, L'*');
		ToScreen(screen, x - 3, y + 1, L'-');
		ToScreen(screen, x - 4, y + 1, L'-');
		ToScreen(screen, x - 5, y + 1, L'-');
		ToScreen(screen, x - 3, y + 2, L'*');
		ToScreen(screen, x - 2, y + 2, L'*');
		ToScreen(screen, x - 1, y + 2, L'*');
		ToScreen(screen, x, y + 1, L'>');
		ToScreen(screen, x - 1, y + 1, L' ');
		ToScreen(screen, x - 2, y + 1, L' ');
		ToScreen(screen, x - 4, y + 2, L' ');
	};
	void printU() {
		ToScreen(screen, x, y, L'@');
		ToScreen(screen, x - 1, y, L' ');
		ToScreen(screen, x - 2, y, L' ');
		ToScreen(screen, x - 3, y, L' ');
		ToScreen(screen, x - 1, y - 1, L'*');
		ToScreen(screen, x - 2, y - 1, L'*');
		ToScreen(screen, x - 3, y - 1, L'*');
		ToScreen(screen, x - 4, y, L'\\');
		ToScreen(screen, x - 3, y + 1, L'\\');
		ToScreen(screen, x - 4, y + 1, L'*');
		ToScreen(screen, x - 3, y + 2, L'*');
		ToScreen(screen, x - 2, y + 2, L'*');
		ToScreen(screen, x - 1, y + 2, L'*');
		ToScreen(screen, x, y + 1, L'>');
		ToScreen(screen, x - 1, y + 1, L' ');
		ToScreen(screen, x - 2, y + 1, L' ');
		ToScreen(screen, x - 4, y + 2, L' ');
		ToScreen(screen, x - 5, y + 1, L' ');
	};
	void printD() {
		ToScreen(screen, x, y, L'@');
		ToScreen(screen, x - 1, y, L' ');
		ToScreen(screen, x - 2, y, L' ');
		ToScreen(screen, x - 3, y, L' ');
		ToScreen(screen, x - 1, y - 1, L'*');
		ToScreen(screen, x - 2, y - 1, L'*');
		ToScreen(screen, x - 3, y - 1, L'*');
		ToScreen(screen, x - 4, y, L'*');
		ToScreen(screen, x - 3, y + 1, L'/');
		ToScreen(screen, x - 4, y + 2, L'/');
		ToScreen(screen, x - 4, y + 1, L'*');
		ToScreen(screen, x - 3, y + 2, L'*');
		ToScreen(screen, x - 2, y + 2, L'*');
		ToScreen(screen, x - 1, y + 2, L'*');
		ToScreen(screen, x, y + 1, L'>');
		ToScreen(screen, x - 1, y + 1, L' ');
		ToScreen(screen, x - 2, y + 1, L' ');
		ToScreen(screen, x - 5, y + 1, L' ');
	};
};


int main()
{
	//Create Screen Buffer
	//MADE GLOBAL: wchar_t* screen = new wchar_t[nScreenWidth * nScreenHeight];
	for (int i = 0; i < nScreenWidth * nScreenHeight; i++) screen[i] = L' ';
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;

	bool GameOver = false;

	//game loop;
	while (true) {
		Sleep(1);

		int score = 0;
		bool scored = false;
		int frame = 0;
		bool frameCalculated = 0;
		int flicker = 0;

		//GROUND
		groundCoord ground[nScreenWidth];
		for (int i = 0; i < nScreenWidth; i++) {
			ground[i].x = i;
			ground[i].y = 44;
			ground[i].texture = i % 6;
		}

		//BIRD
		birdie Bird;
		Bird.x = 35;
		Bird.y = 22;
		Bird.posY = 22.0;
		Bird.velY = 0.0;
		Bird.velMax = 1.5;

		double accY = 0.04;

		//PIPES
		pipe Pipe1;
		Pipe1.x = nScreenWidth + 2;
		Pipe1.y = nScreenHeight / 2;
		pipe Pipe2 = Pipe1;

		bool hitPipe = false;

		//space key
		bool bKeySpace = false;
		bool bKeySpaceOld = false;
		bool Jump = false;

		//initialize time;
		auto t1 = chrono::system_clock::now();



		//look for space press (latch)
		bKeySpace = (0x8000 & GetAsyncKeyState((unsigned char)('\x20'))) != 0;
		if (bKeySpace && !bKeySpaceOld) {
			Jump = true;
			hitPipe = false;
			GameOver = false;
		}
		bKeySpaceOld = bKeySpace;


		while (!GameOver) {

			if (!hitPipe) {
				//look for space press (latch)
				bKeySpace = (0x8000 & GetAsyncKeyState((unsigned char)('\x20'))) != 0;
				if (bKeySpace && !bKeySpaceOld)
					Jump = true;
				bKeySpaceOld = bKeySpace;
			}

			//SETS UP FRAME
			if (!frameCalculated) {

				//clear
				for (int i = 0; i < nScreenWidth * nScreenHeight; i++) screen[i] = L' ';

				//Print Ground and update textures
				{
					for (int i = 0; i < nScreenWidth; i++) {
						if (0 <= ground[i].texture && ground[i].texture < 3)
							ToScreen(screen, ground[i].x, ground[i].y, Medium);
						if (3 <= ground[i].texture && ground[i].texture < 6)
							ToScreen(screen, ground[i].x, ground[i].y, Dark);

					}
					if (!hitPipe) {
						if (frame % 2 == 0) {
							for (int i = 0; i < nScreenWidth; i++) {
								ground[i].texture++;
								ground[i].texture = ground[i].texture % 6;
							}
						}
					}
				}

				//Still Backgroud
				// No Background

				//Pipes
				{   //print pipes
					if (Pipe1.x == ((nScreenWidth / 2) - 3))
						Pipe2.reset();

					if (Pipe1.x == -8)
						Pipe1.reset();

					if (!hitPipe) {
						if (frame % 2 == 1) {
							Pipe1.nextGraphics();
							Pipe2.nextGraphics();
						}
					}

					Pipe1.print();
					Pipe2.print();
				}

				//Bird Physics
				{
					//Bird Jump 
					{
						if (Jump) {
							Bird.velY = -0.6;
							Jump = false;
							Bird.animation = 0;

							//prevents jump if too high
							if (Bird.y < 0)
								Bird.velY = 0;
						}
					}

					//COLLISION DETECTION
					{if (Bird.y > 41)
						GameOver = true;
					if (Bird.y < 0)
						Bird.velY = 0.05;
					}

					//Gravity Physics

					//slight increase to acceleration at top curve to make for better gameplay
					if (-0.1 < Bird.velY && Bird.velY < 0.1)
						accY = 0.1;

					Bird.posY = Bird.posY + Bird.velY;
					Bird.velY = Bird.velY + accY;
					if (Bird.velY > Bird.velMax)
						Bird.velY = Bird.velMax;

					Bird.y = int(Bird.posY);
					Bird.setBirdBox();
					accY = 0.06; //reset acceleration value.
				}

				Bird.print();

				//checking frame rate
				{/*
				screen[frame] = '*';
				if (frame == 29)
					for (int i = 0; i < 30; i++) screen[i] = L' ';
				*/}

				//FRAME COUNT
				{
					frame++;
					frame = frame % 30;
				}

				//TOGGLE FOR FRAME CALCULATED
				frameCalculated = true;

				//PIPE COLLISION
				{
					for (auto p : Pipe1.graphics)
						for (auto b : Bird.birdBox)
							if (b.x == p.x && b.y == p.y) {
								hitPipe = true;
							}

					for (auto p : Pipe2.graphics)
						for (auto b : Bird.birdBox)
							if (b.x == p.x && b.y == p.y) {
								hitPipe = true;
							}
				}

				//SCORE
				{
					if (((Bird.x == Pipe1.x + 7) || (Bird.x == Pipe2.x + 7)) && !scored) {
						score++;
						scored = true;
					}
					if ((Bird.x == Pipe1.x + 8) || (Bird.x == Pipe2.x + 8))
						scored = false;
					wsprintf(&screen[2 * nScreenWidth + nScreenWidth / 2 - 5], L" Score: %d", score);
				}
			}

			//CHECK TIME
			if ((chrono::system_clock::now() - t1) > (33ms))
			{
				//PRINT SCREEN
				WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
				//RESET TIME;
				t1 = chrono::system_clock::now();
				//TOGGLE FOR FRAME CALCULATION
				frameCalculated = false;


			}
			//Reduces CPU Usage
			Sleep(1);
		}
	}
}