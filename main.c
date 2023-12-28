#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <termios.h>

// x y coord system is wack with row-col formula for arrays, works with this ratio
#define mapSizeY 70
#define mapSizeX 40
#define FRAMERATE 60

struct termios oldt,newt;

struct Ball {	
	double x;
	double y;
	double prevX;
	double prevY;
	double vx;
	double vy;
	double initialVelocity;
	double angle; 
	int collision;
};

char userInput = -1; 
char menuInput = -1;
char ballInput = 0;
//map[row = y][column = x] 
char map[mapSizeX][mapSizeY]; 
int simulationTime = 0;
double gravity = -9.81;

void setNonCanonicalMode();
void setCanonicalMode();
void drawMenu();
void clearScreen();
void drawPhysicsSim();
void updatePhysicsSim();
void fillMap();
void userMoveBall();
int isOutOfBounds();
void setMapXY(double x, double y, char c);
void setBallPos(double x,double y);




struct Ball ball = {1, 1, 0, 0, 0, 0, 20, 45, 0};
int main()
{
	int i = 0;
	int count = 0;
	init(); 

	//menu screen
	clearScreen();
	drawMenu();
	read(STDIN_FILENO, &menuInput,1); 
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
		read(STDIN_FILENO, &userInput,1); 

		//wait for user to view simulation and update for next frame
		sleep(1/FRAMERATE);

		updatePhysicsSim();
		count++;
		if(count == FRAMERATE) 
		{
			simulationTime++;
			count = 0;
		}
	}
	clearScreen();
	setCanonicalMode();
	return 0;
}

/* ---- BALL.H ----- */
//maybe map.h for bounds?
int isOutOfBounds(int x, int y)
{
	if(x < mapSizeX && x >= 0 && y < mapSizeY && y >= 0)
		return 0;
	else
		return 1;
}

void setMapXY(double x, double y, char c)
{
	if(!isOutOfBounds(x, y))
		map[(int)y][(int)x] = c;
}

void setBallPos(double x,double y) 
{
	if(!isOutOfBounds(ball.x, ball.y))
		map[(int)y][(int)x] = '@';
}
//check ball boundary
	//set ball.collision = 1;

/* ---- END BALL.H ---- */
void clearScreen()
{
/*	\e[2J -clear screen 
		\e[H -move cursor to top left	 */
	printf("\e[2J\e[H"); 
}

void init()
{
	int i = 0;
	
	//create default map with dots
	for(i = 0; i < mapSizeX; i++)
	{
		for(int j = 0; j < mapSizeY; j++)
		{
			map[i][j] = '.';
		}	
	}
	
	//create map boundary markers in Y Axis
	for(i = 0; i < mapSizeX; i++)
	{
		if(i % 5 == 0 || i == 0)
			map[i][0] = i;
		else
			map[i][0] = '|';
	}
	//create map boundary markers in X Axis
	for(i = 0; i < mapSizeY; i++)
	{
		if(i % 5 == 0 || i == 0)
		
			map[0][i] = i;
		else
			map[0][i] = '-';
			
	}
	//calculate initial velocity components	
	ball.vx = ball.initialVelocity * cos( (double) ball.angle) / FRAMERATE;
	ball.vy = ball.initialVelocity * sin( (double) ball.angle) / FRAMERATE;

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
	printf("	s -> start the simulation\n");	
	printf("	q -> quit\n");	
}



void drawPhysicsSim()
{	
	setBallPos(ball.x, ball.y);

	//x (map array row) increases upwards instead of downwards
	for(int i = mapSizeX - 1; i >= 0; i--)
	{
		for(int j = 0; j < mapSizeY; j++)
		{
			//X axis numbers
			if(j == 0 && i % 5 == 0)
				printf("%d", map[i][j]);
			//Y axis numbers
			else if(i == 0 && j % 5 == 0)
				printf("%d", map[i][j]);
			//print (0,0)
			else if(i == 0 && j == 0)
				printf("%c", map[i][j]);
			else
				printf("%c", map[i][j]);
		}
		printf("\n");
	}
	
	printf("Simulation Time: %d seconds \n", simulationTime);	
	printf("Framerate: %d FPS \n", FRAMERATE);
	
	printf("Cannon Angle: %g degrees\n", ball.angle);
	printf("Cannon Velocity: %g m/s\n", ball.initialVelocity);
	printf("Ball Position (x,y) = (%g m, %g m)\n", ball.x, ball.y);
	printf("Ball Velocity (x,y) = (%g m/s, %g m/s)\n", ball.y, ball.vy);
	printf("Gravity: %g m/s\n", gravity);
	
	printf("\n");
}

void updatePhysicsSim()
{
	if(ball.collision == 1) 
		setBallPos(ball.prevX, ball.prevY);
	else
	{
	//update velocities due to accelerations
	ball.vx; // + air resistance
	ball.vy += gravity  / (FRAMERATE * FRAMERATE);

	//store previous location to clear
	ball.prevX = ball.x;
	ball.prevY = ball.y;	
	//update ball position
	ball.x += ball.vx;
	ball.y += ball.vy;

	//clear old ball position
	if(ball.x != ball.prevX || ball.y != ball.prevY)
		setMapXY(ball.prevX, ball.prevY, '.');
	
	if(isOutOfBounds(ball.x, ball.y))
		ball.collision = 1;
	}
		
} 

void userMoveBall() {
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
