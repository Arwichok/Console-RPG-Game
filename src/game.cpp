#include <stdio.h>
// for printf()
#include <windows.h>
// for system()
#include <conio.h>
// for getch()
#include <string>
// for string
#include <sstream>
// #include <iomanip>
// #include <iostream>

using namespace std;

struct entity
{
	int x;
	int y;
};
struct entityM
{
	int x;
	int y;
	int width;
	int height;
};

struct shape
{
	string sh[15];
	int x;
	int y;
	int comX;
	int comY;
	int width;
	int height;
};

struct field
{
	string name;
	int width;
	int height;
};
enum keycodes { STOP, YES, NO, UP, DOWN, LEFT, RIGHT, EXIT, SPACE };

void write(string);
void writeText(string text, int wid);
void init();
void draw();
void input();
void logic();
string toString(int i);
bool isExistChar(struct entity arr[], int x, int y);
char renderChar(int x, int y);
void windowMode(int width, int height);
string getMenuString(int n);
void setShapeSize(shape&);
char getShapeChar(shape sh, int x, int y);
bool isShapeActivate(shape, entity);
bool isShapeActivate(shape, shape);
void setEntityPos(entity&, shape);
void setEntityPos(entity&, int x, int y);
void attackPlayer();
void attackEnemy();
void setShapePos(shape&, shape);
void setShapePos(shape&, int x, int y);


const int width = 21, height = 15;
const int rigthMenu = 10;
const int windowWidth = width + 4 + rigthMenu, windowHeight = height+4;
const char border = '#';
const char flower = '*';
const char dollar = '$';
const char houseMap[][6] = {" ___ ",
							"/___\\",
							"|[ ]|"};

const field mainMap = { "mainMap", width, height };
const field dialogMap = { "dialogMap", width, 3 };
const field fightMap = { "fightMap", width, height };
const string firstText = "Hello my friends! I hear you very strong, you can kill one monster? [y/n]";
const string shopText = "Hello! You want buy 2 attack($2)? [y/n]";
const entityM house = { 10, 10, sizeof(houseMap[0])-1, sizeof(houseMap)/sizeof(houseMap[0])};

entity player = { 5, 3 };
entity flowers[15];
entity money[8];
string fil = "";
bool gameOver = false;
field currentField = mainMap;
keycodes currentCode = STOP;
int moneycount = 0;
int attack = 1;

int enemyLife = 9;
int playerLife = 4;

// shape houseShape = { 5, 10, { " ___ ", ""}}

shape houseShape = { {" ___ ",
					  "/___\\",
					  "|[S]|" }, 5, 5, 2, 2, 4, 3};

shape portalShape = { {"[F]"}, 17, 13, 1, 0, 3, 1 };
shape enemy1 = { {" ___ ",
				  "(-_-)", 
				  " \\+/ "}, -5, -5, 1, 1, 5, 3 };
shape enemy2 = { {" ___ ",
				  "(o_o)", 
				  " \\+/ "}, -5, -5, 1, 1, 5, 3 };
shape enemy3 = { {" ___ ",
				  "(X_X)", 
				  " \\+/ "}, -5, -5, 1, 1, 5, 3 };
shape enemy = { {" ___ ",
				  "(-_-)", 
				  " \\+/ "}, width/2, height/2-2, 1, 1, 5, 3 };


shape rightArrowShape = { {"->"}, -5, -5, 0, 0, 2, 1 };
shape leftArrowShape = { {"<-"}, -5, -5, 0, 0, 2, 1 };
shape arrowShape = { {"->"}, -5, -5, 0, 0, 2, 1 };


shape wallShape = { {"#","#","#"}, -5, -5, 0, 0, 1, 3 };
shape playerShape = { {"&"}, 17, 14, 0, 0, 1, 1 };


int main()
{
	// FOR TEST /////


	// system("pause");
	// FOR TEST /////

	init();
	while(!gameOver)
	{
		draw();
		input();
		logic();
	}

	return 0;
}
void init()
{
	windowMode(windowWidth, windowHeight);
	system("title Console RPG Game");

	setShapeSize(houseShape);
	setShapeSize(portalShape);
	setShapeSize(enemy1);
	setShapeSize(enemy2);
	setShapeSize(enemy3);
	setShapeSize(enemy);
	setShapeSize(arrowShape);
	setShapeSize(playerShape);
	setShapeSize(wallShape);

	for(int f = 0; f < sizeof(flowers)/8; f++)
	{
		entity t = { (rand() % (width-2) + 1), (rand() % (height-2) + 1) };
		flowers[f] = t;
	}
	for(int m = 0; m < sizeof(money)/8; m++)
	{
		entity t = { (rand() % (width-2) + 1), (rand() % (height-2) + 1) };
		money[m] = t;
	}
}

void input()
{
	char ch = getch();
	switch(ch)
	{
		case 'w':
			currentCode = UP;
			break;
		case 'a':
			currentCode = LEFT;
			break;
		case 's':
			currentCode = DOWN;
			break;
		case 'd':
			currentCode = RIGHT;
			break;
		case '\x1b':
			currentCode = EXIT;
			break;
		case 'y':
			currentCode = YES;
			break;
		case 'n':
			currentCode = NO;
			break;
		case '\x20':
			currentCode = SPACE;
	}
}
void draw()
{
	 fil = "\n ";
	for(int t = 0; t < currentField.width+2; t++)
	{
		fil+=border;
	}
	fil+="\n";
	for(int y = 0; y < currentField.height; y++)
	{
		fil+=" ";
		fil+=border;
		for(int x = 0; x < currentField.width; x++)
		{
			fil+=renderChar(x, y);
		}
		fil+=border;
		fil+=getMenuString(y);
		fil+="\n";
	}
	fil+=" ";
	for(int t = 0; t < currentField.width+2; t++)
	{
		fil+=border;
	}

	fil+="\n";
	system("cls");
	printf("%s", fil.c_str());

	if(currentField.name == dialogMap.name) 
		writeText(shopText, width);
}
char renderChar(int x, int y)
{	
	char ret = ' ';
	if(currentField.name == mainMap.name)
	{
		if(x == playerShape.x && y == playerShape.y)
		{
			return getShapeChar(playerShape, x, y);
		}

		if(ret == ' ')
			ret = getShapeChar(houseShape, x, y);
		if(ret == ' ')
			ret = getShapeChar(portalShape, x, y);

		if(ret == ' ')
			for(int i = 0; i < sizeof(flowers)/8; i++){
				if(flowers[i].x == x && flowers[i].y == y) 
					ret = flower;
			}
		if(ret == ' ')
			for(int i = 0; i < sizeof(money)/8; i++){
				if(money[i].x == x && money[i].y == y) 
					ret = dollar;
	}
	}
	else if(currentField.name == fightMap.name)
	{
		if(x == playerShape.x && y == playerShape.y)
		{
			return getShapeChar(playerShape, x, y);
		}
		if(ret == ' ')
			ret = getShapeChar(enemy, x, y);

		if(ret == ' ') 
			ret = getShapeChar(arrowShape, x, y);
		if(ret == ' ')
			ret = getShapeChar(wallShape, x, y);
	}
	return ret;
}
void write(string str)
{
	printf("%s", str.c_str());
}
string toString(int i)
{
	stringstream conv;
	conv << i;
	return conv.str();
}
void windowMode(int width, int height)
{
	stringstream convert;
	convert << "mode con: cols=" << width << " lines=" << height;
	system(convert.str().c_str());
}

void logic()
{
	// player control
	if(currentField.name == mainMap.name || currentField.name == fightMap.name)
	{
		switch(currentCode)
		{
			case UP:
				playerShape.y--;
				break;
			case LEFT:
				playerShape.x--;
				break;
			case DOWN:
				playerShape.y++;
				break;
			case RIGHT:
				playerShape.x++;
				break;
		}

		Beep(700, 100);
		// Beep(800, 100);


		if(playerShape.x < 0) playerShape.x = 0;
		if(playerShape.x >= currentField.width) playerShape.x = currentField.width-1;
		if(playerShape.y < 0) playerShape.y = 0;
		if(playerShape.y >= currentField.height) playerShape.y = currentField.height-1;
	
	}

	if(currentField.name == mainMap.name){
		switch(currentCode)
		{
			case EXIT:
				gameOver = true;
				break;
		}
		for(int b = 0; b < sizeof(money)/8; b++)
		{
			if(playerShape.x == money[b].x && playerShape.y == money[b].y)
			{
				moneycount++;
				money[b].x = money[b].y = -1;
				Beep(800, 100);
				Beep(700, 50);
			}
		}

		if(isShapeActivate(houseShape, playerShape)){
			currentField = dialogMap;
		}
		if(isShapeActivate(portalShape, playerShape)){
			currentField = fightMap;
			setShapePos(playerShape, 3, height/2);
		}
	}


	if(currentField.name == dialogMap.name)
	{
		switch(currentCode)
		{
			case YES:
				if(moneycount >= 2){
					moneycount-=2;
					attack+=2;
				}
				break;
			case NO:
			case EXIT:
				currentField = mainMap;
				setShapePos(playerShape, houseShape.x, houseShape.y+1);
				break;
		}
	}
	if(currentField.name == fightMap.name)
	{
		switch(currentCode)
		{
			case SPACE:
				attackPlayer();
				attackEnemy();
				break;
			case EXIT:
				currentField = mainMap;
				setShapePos(playerShape, portalShape.x, portalShape.y+1);
				break;
		}
	}
	currentCode = STOP;
}

void writeText(string text, int wid)
{
	for(int s = 0; s < text.size(); s++)
	{
		if(s == 0) printf(" ");
		if(s % wid == 0) {
			if(text[s-1] != ' ' && s!= 0) printf("-");
			printf("\n ");
		}
		printf("%c", text[s]);
		Sleep(10);
	}
}
bool isExistChar(struct entity* arr, int x, int y)
{
	for(int i = 0; i < sizeof(arr); i++){
		if(arr[i].x == x && arr[i].y == y) 
			return true;
	}
	return false;
}
string getMenuString(int n)
{

	switch(n)
	{
		case 0:
			return " $ - " + toString(moneycount); break;
		case 2:
			return " a - " + toString(attack); break;
		case 6:
			return " control:"; break;
		case 7: 
			return " wasd "; break;
		case 8: 
			return " attack: "; break;
		case 9:
			return " space"; break;
		case 10:
			return " exit: esc"; break;
	}
	if(currentField.name == fightMap.name)
	{
		if(n == 4) return " p - " +toString(playerLife);
		if(n == 5) return " e - " +toString(enemyLife);
	}

	return "";
}
void setShapeSize(shape& s)
{	
	int i = 0;
	while(!s.sh[i].empty())
		i++;
	s.width = s.sh[0].size();
	s.height = i;
}
char getShapeChar(shape shep, int x, int y)
{
	int lx = x - shep.x, ly = y - shep.y;
	if(lx >= 0 && lx < shep.width && ly >= 0 && ly < shep.height)
	{
		return shep.sh[ly][lx];
	}
	return ' ';
}
bool isShapeActivate(shape shep, entity ent)
{
	if((shep.x + shep.comX) == ent.x 
		&& (shep.y + shep.comY) == ent.y) 
			return true;
	return false;
}
bool isShapeActivate(shape shep, shape she)
{
	if((shep.x + shep.comX) == she.x 
		&& (shep.y + shep.comY) == she.y) 
			return true;
	return false;
}
void setEntityPos(entity& en, shape sh)
{
	en.x = sh.x + sh.comX-1;
	en.y = sh.y + sh.comY;
}
void setEntityPos(entity& en, int x, int y)
{
	en.x = x;
	en.y = y;
}
void setShapePos(shape& sh, shape otsh)
{
	sh.x = otsh.x;
	sh.y = otsh.y;
}
void setShapePos(shape& sh, int x, int y)
{
	sh.x = x;
	sh.y = y;
}
bool isCollisionShape(shape sh1, shape sh2)
{
	for(int my = 0; my < sh1.height; my++)
	{
		for(int mx = 0; mx < sh1.width; mx++)
		{
			for(int iy = 0; iy < sh2.height; iy++)
			{
				for(int ix = 0; ix < sh2.width; ix++)
				{
					if((sh1.x+mx) == (sh2.x+ix) && (sh1.y+my) == (sh2.y+iy)) 
						return true;
				}
			}
		}
	}
	return false;
}
void attackPlayer()
{

	if(playerLife == 0) return;

	int direction; // wasd - uldr - 0123;
	if(enemy.x > playerShape.x) {

		direction = 3;
		arrowShape.sh[0] = rightArrowShape.sh[0];
	}
	else if(enemy.x <= playerShape.x) {
		direction = 1;
		arrowShape.sh[0] = leftArrowShape.sh[0];
	}
	setShapePos(arrowShape, playerShape);

	while(!(arrowShape.x > width || arrowShape.x < 0)){

		if(direction == 3) arrowShape.x++;
		else if(direction == 1) arrowShape.x--;

		draw();
		Sleep(50);
		if(isCollisionShape(arrowShape, enemy))
		{
			Beep(700, 100);

			arrowShape.x = arrowShape.y = -5;
			if(enemyLife>0){
				enemyLife-=attack;
			}
			
			if(enemyLife<=0)
			{
				enemy.sh[1] = enemy3.sh[1];
				enemyLife=0;

			}else if(enemyLife>0)
			{
				enemy.sh[1] = enemy2.sh[1];
				draw();
				enemy.sh[1] = enemy1.sh[1];
			}
			break;
		}
	}

	for(int i = 0; i < width; i++)
	{
		arrowShape.x = playerShape.x+i;
		arrowShape.y = playerShape.y;
	}
}
void attackEnemy()
{

	if(enemyLife == 0) return;

	entity enemyPos = { 0, 0 };
	int direction = 0; // wasd - uldr - 0123;
	
	if(width/2 > playerShape.x) {
		enemyPos.x = width - enemy.width - 1;
	}
	else if(width/2 <= playerShape.x)
		enemyPos.x = 1;

	enemyPos.y = playerShape.y;
	
	if(enemyPos.y+enemy.height >= height) 
		enemyPos.y = height-enemy.height;

	while(true)
	{
		draw();
		if(enemyPos.x == enemy.x && enemyPos.y == enemy.y)
			break;
		Sleep(50);

		if(enemyPos.x > enemy.x) enemy.x++;
		else if(enemyPos.x < enemy.x) enemy.x--;
		if(enemyPos.y > enemy.y) enemy.y++;
		else if(enemyPos.y < enemy.y) enemy.y--;
	}

	setShapePos(wallShape, enemy);

	while(true)
	{
		if(isCollisionShape(playerShape, wallShape))
		{
			playerLife-=2;
			wallShape.x = wallShape.y = -5;
			if(playerLife <= 0) {
				playerLife = 0;
				gameOver = true;
			}
			break;
		}
		if(wallShape.x > width || wallShape.x < 0) break;
		if(enemy.x > playerShape.x) wallShape.x--;
		else if(enemy.x <= playerShape.x) wallShape.x++;
		draw();
		Sleep(50);
	}
}