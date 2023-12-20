#include <stdio.h>
#include <stdlib.h> 
#include <termios.h> 
#include <unistd.h>
#include <math.h>

#define mapSizeX 25
#define mapSizeY 50


struct termios oldt,newt;

// struct Cannon {
// 	int angle;
//	int startVelocity; //split into x and y?
//	int cannonBallMass;
//}cannon;

struct Ball {	
	int x;
	int y;
	int prevX;
	int prevY;
	int initialVelocity;
	int angle; 
}ball = {0, 0, 0,0, 10, 45};

char userInput = -1; 
char menuInput = -1;
char ballInput = 0;
char map[mapSizeX][mapSizeY]; 
int simulationTime = 0;


void setNonCanonicalMode();
void setCanonicalMode();
void drawMenu();
void clearScreen();
void drawPhysicsSim();
void updatePhysicsSim();
void fillMap();
void userMoveBall();

void setMapXY(int x, int y, char c) {map[y][x] = c;}
void setBallPos(int x,int y) {map[y][x] = '@';}

int main()
{
	//init
	fillMap();

	//menu screen
	clearScreen();
	drawMenu();
	read(STDIN_FILENO,&menuInput,1); 
	switch(menuInput)
		{
			case('q'):
				exit(0);
			case('s'):
				break;
		}menuInput = -1;

	//physics screen
	setNonCanonicalMode();
	while(userInput != 'q')
	{
		clearScreen();
		drawPhysicsSim();

		read(STDIN_FILENO,&userInput,1); 

		//simulation time is currently tied to framerate
		simulationTime++;
		printf("The current simulation time is: %d\n", simulationTime);
		//wait for user to view simulation and update for next frame
		sleep(1);
		updatePhysicsSim(); 
		
	}
	clearScreen();
	setCanonicalMode();
	return 0;
}

void clearScreen()
{
/*	\e[2J -clear screen 
		\e[H -move cursor to top left	 */
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
//draw screen with green grass and blue air
//draw cannon on bottom left of screen
//show angle in degrees 
	//change angle with up and down arrow keys

// ---->> along bottom edge for "Time" axis
// ---->> along left edge for "Height" axis

}

void drawMenu()
{
	printf("Welcome to the physics simulation!\n"); 
	printf("Please choose an option:\n\n");
	printf("s -> start the simulation\n");	
	printf("q -> quit\n");	
}



void drawPhysicsSim()
{
	setBallPos(ball.x, ball.y);

	for(int i = mapSizeX - 1; i >= 0; i--)
	{
		for(int j = 0; j < mapSizeY; j++)
		{
			printf("%c", map[i][j]);
		}
		printf("\n");
	}
}

void updatePhysicsSim()
{
	//drawing ballZ
	
	//store previous location to clear
	ball.prevX = ball.x;
	ball.prevY = ball.y;	
	/*	
	//calculate next ball position based on velocity and angle
	int dx, dy, vx, vy, t1, t2;
	
	t1 = simulationTime;
	t2 = simulationTime + 1;
	//vx = v * sin(cannon.angle); // needs to be converted to degrees for percentage
	//vy = v * cos(cannon.angle);

	vx = ball.initialVelocity / 2;
	vy = ball.initialVelocity / 2;
	
	dx = vx * (t2 - t1);
	dy = vy * (t2 - t1);

	//store new position in map		
	*/ 
	//move ball diagonally across the screen
	
	static int diag = 0;	
	if(diag ==0)
	{
		ball.x++;
		diag = 1;
	}
	else
	{
		ball.y++;
		diag = 0;
	}
	//clear old ball position
	if(ball.x != ball.prevX || ball.y != ball.prevY) setMapXY(ball.prevX, ball.prevY, '.');
	
}

void userMoveBall()
{
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
	userInput = -1;
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
