#include <stdio.h>
#include <stdlib.h> 
#include <termios.h> 
#include <unistd.h>

#define mapSizeX 25
#define mapSizeY 50

void setNonCanonicalMode();
void setCanonicalMode();
void drawMenu();
void clearScreen();
void drawPhysicsSim();
void updatePhysicsSim();
void fillMap();

struct termios oldt,newt;

// struct Cannon {
// 	int angle;
//	int startVelocity; //split into x and y?
//	int cannonBallMass;
//}cannon;

struct Ball {	
	int x;
	int y;
	int dx;
	int dy;
	int ddx;
	int ddy;
}ball = {0,0,0,0,0,0};

char userInput;
char map[mapSizeX][mapSizeY]; 

int main()
{
	/*	\e[2J -clear screen 
		\e[H -move cursor to top left	 */

	setNonCanonicalMode();
	fillMap();
	int ballPrevX;
	int ballPrevY;	

	while(1)
	{
	clearScreen();
	read(STDIN_FILENO,&userInput,1);
	//drawMenu();
	drawPhysicsSim();
	updatePhysicsSim(); 


	sleep(0.03);
	if(userInput == 'q') break;
	
	//drawing ballZ
	map[ball.x][ball.y] = '@';
	
	//store previous location to clear
	ballPrevX = ball.x;
	ballPrevY = ball.y;	
	switch(userInput)
	{
		case('j'): //down 
			if(ball.x < mapSizeX) ball.x += 1;
			break;
		case('h'): //left
			if(ball.y > 0) ball.y -= 1;
			break;
		case('l'): //right
			if(ball.y < mapSizeY) ball.y += 1;
			break;
		case('k'): //up
			if(ball.x > 0) ball.x -= 1;
			break;
		default:
			break;
	}
	//reset userInput
	userInput = '1';
	//reset map
	if(ball.x != ballPrevX || ball.y != ballPrevY)	map[ballPrevX][ballPrevY] = '.';
	}
	setCanonicalMode();
	return 0;
}

void clearScreen()
{
	printf("\e[2J\e[H"); 
}

void fillMap()
{
	for(int i = 0; i < mapSizeX; i++)
	{
		for(int j = 0; j < mapSizeY; j++)
		{
			map[i][j] = '.';
		}	
	}

}

void drawMenu(){
	/*	clear screen	*/
	printf("Welcome to the physics simulation!\n"); 
	printf("Press q to quit!\n");
	printf("Press s to start the simulation\n");
}

void drawPhysicsSim()
{
	for(int i = 0; i < mapSizeX; i++)
	{
		for(int j = 0; j < mapSizeY; j++)
		{
			printf("%c", map[i][j]);
		}
		printf("\n");
	}

//draw screen with green grass and blue air
//draw cannon on bottom left of screen
//show angle in degrees 
	//change angle with up and down arrow keys
//shoot cannon with enter

//show ball as @ sign 
	//updatePhysicsSim() will calculate next ball location
}

void updatePhysicsSim()
{
}

void setNonCanonicalMode()
{
	// Save old terminal settings
	tcgetattr(STDIN_FILENO, &oldt);

	// Set terminal to non-canonical mode, wait for 1 key, no timeout
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO); 
	newt.c_cc[VMIN] = 0;
	newt.c_cc[VTIME] = 1;
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
}


void setCanonicalMode()
{
	// Restore old terminal settings
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
}
