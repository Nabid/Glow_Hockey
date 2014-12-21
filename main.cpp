#include <GL/glut.h>
#include <cstdio>
#include <iostream>
#include <cmath>

using namespace std;

#define RUNNING true
#define STOPPED false

int width = 350;
int height = 500;
bool game_status = STOPPED;
bool game_over = false;
double direction = 0.5;

// color structure
struct Color {
	double r, g, b;
	Color() {
		r = g = b = 0.0;
	}
} RED, GREEN, BLUE, YELLOW, WHITE, BLACK;

// circles used in game
struct Circle {
	double x, y, radius;
	int score;
	Circle() {
		x = y = radius = 0.0;
		score = 0;
	}
} ball, player_1, player_2;

// resets game contents
void gameReset() {
	// ball in center
	ball.x = width/2;
	ball.y = height/2;
	ball.radius = 20;
	// player 1 in player 1's position
	player_1.x = width/2;
	player_1.y = 60;
	player_1.radius = 30;
	//player_1.score = 0;
	// player 2 in player 2's position;
	player_2.x = width/2;
	player_2.y = height-60;
	player_2.radius = 30;
	//player_2.score = 0;
}

// creates color
void createColor() {
	RED.r = 1.0;
	GREEN.g = 1.0;
	BLUE.b = 1.0;
	YELLOW.r = YELLOW.g = 1.0;
	WHITE.r = WHITE.g = WHITE.b = 1.0;
}

void init() {
	glClearColor(0, 0, 0, 0);
	glShadeModel(GL_SMOOTH);
	//glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D( 0, width, 0, height );
	//glOrtho(0, 1, 0, 1, -1, 1);
}

void reshape( int w, int h ) {
	width = w; height = h;
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    //glMatrixMode(GL_PROJECTION);
    //glLoadIdentity();
    init();
    glutPostRedisplay();
}

void drawCircle( int x, int y, int radius, int X, Color c ) {
	double PI = 2 * acos(0.0);
	// draw circle
	glColor3f( c.r, c.g, c.b );
	glBegin(X);
	for(int i=0; i<=360; i+=360/200) {
		double a = x + cos( i * PI / 180.0 ) * radius;
		double b = y + sin( i * PI / 180.0 ) * radius;
		glVertex3f(a, b, 0.0);
	}
	glEnd();
}

void drawLine( int x1, int y1, int x2, int y2, Color c ) {
	glColor3f( c.r, c.g, c.b );
	glBegin(GL_LINE_LOOP);
		glVertex2i(x1, y1);
		glVertex2i(x2, y2);
	glEnd();
}

void drawRect( int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, Color c ) {
	glColor3f( c.r, c.g, c.b );
	glBegin(GL_POLYGON);
		glVertex2i(x1, y1);
		glVertex2i(x2, y2);
		glVertex2i(x3, y3);
		glVertex2i(x4, y4);
	glEnd();
}

void drawBoard() {
	// draw center circle
	drawCircle( width/2, height/2, 50, GL_LINE_LOOP, WHITE );
	drawCircle( width/2, height/2, 45, GL_LINE_LOOP, WHITE );
	// draw player 1 goal post
	drawCircle( width/2, 0, 50, GL_LINE_LOOP, WHITE );
	drawCircle( width/2, 0, 45, GL_LINE_LOOP, WHITE );
	// draw player 2 goal post
	drawCircle( width/2, height, 50, GL_LINE_LOOP, WHITE );
	drawCircle( width/2, height, 45, GL_LINE_LOOP, WHITE );
	// draw center line
	drawLine( 0, height/2-2, width, height/2-2, WHITE );
	drawLine( 0, height/2+2, width, height/2+2, WHITE );
	// draw side bars
	// left
	drawRect( 0, 0, 10, 0, 10, height/2-2, 0, height/2-2, BLUE );
	drawRect( 0, height/2+3, 10, height/2+3, 10, height, 0, height, RED );
	// right
	drawRect( width-10, 0, width, 0, width, height/2-2, width-10, height/2-2, GREEN );
	drawRect( width-10, height/2+3, width, height/2+3, width, height, width-10, height, YELLOW );
	// bottom
	drawRect( 10, 0, width/2-50, 0, width/2-50, 10, 10, 10, BLUE );
	drawRect( width/2+50, 0, width-10, 0, width-10, 10, width/2+50, 10, GREEN );
	// top
	drawRect( 10, height-10, width/2-50, height-10, width/2-50, height, 10, height, RED );
	drawRect( width/2+50, height-10, width-10, height-10, width-10, height, width/2+50, height, YELLOW );
}

void drawPlayers() {
	//D2( ball.x, ball.y );
	//D2( player_1.x, player_1.y );
	//D2( player_2.x, player_2.y );
	// draw ball
	drawCircle( ball.x, ball.y, ball.radius, GL_POLYGON, YELLOW );
	// draw first player
	drawCircle( player_1.x, player_1.y, player_1.radius, GL_POLYGON, RED );
	drawCircle( player_1.x, player_1.y, 15, GL_POLYGON, BLACK );
	// draw second player
	drawCircle( player_2.x, player_2.y, player_1.radius, GL_POLYGON, GREEN );
	drawCircle( player_2.x, player_2.y, 15, GL_POLYGON, BLACK );
}

int dist( Circle a, Circle b ) {
	return sqrt( (double)(a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y) );
}

void printScore() {
	cout << "-------------------" << endl;
	cout << "Player 1 - " << player_1.score << endl;
	cout << "Player 2 - " << player_2.score << endl;
	cout << "===================" << endl;

	if( player_1.score >= 5 || player_2.score >= 5 ) {
		cout << "W I N N E R : ";
		if( player_1.score > player_2.score ) cout << "PLAYER 1" << endl;
		else cout << "PLAYER 2" << endl;
		cout << "GAME OVER" << endl;
		game_over = true;
	}
}

void checkGoal() {
	// player 1 goal post
	if( ball.x > width/2-50 && ball.x < width/2+50 ) {
		if( ball.y >= height-ball.radius-10 ) {
			cout << "Player 1 Wins" << endl;
			player_1.score++;
			game_status = STOPPED;
		} else if( ball.y <= ball.radius+10 ) {
			cout << "Player 2 Wins" << endl;
			player_2.score++;
			game_status = STOPPED;
		}
	}
	if( !game_status ) {
		gameReset();
		printScore();
	}
}

void moveBall() {
	// change direction
	// along y_axis
	if( ball.y <= 0+10+ball.radius ) direction *= -1;
	if( ball.y >= height-10-ball.radius ) direction *= -1;
	//D2( dist( ball, player_1 ), dist( ball, player_2 ) );
	if( dist( ball, player_1 ) <= ball.radius + player_1.radius ) direction *= -1;
	if( dist( ball, player_2 ) <= ball.radius + player_2.radius ) direction *= -1;

	ball.y += direction;

	// along x_axis
	int ax = rand() %2;
	if( ax == 1 ) ball.x++;
	else ball.x--;

	if( ball.x <= 0+10+ball.radius ) ball.x++;
	if( ball.x >= width-10-ball.radius ) ball.x--;

	checkGoal();
}

void display() {
	glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// draw board
	drawBoard();

	// set field position
	if( !game_status ) gameReset();

	// draw players and ball
	drawPlayers();

	if( game_status ) moveBall();

	// text --------------------------------------------------
	string p1 = "Player 2";
    string p2 = "Player 1";
    string p1s, p2s;
    switch( player_1.score ) {
		case 0: p1s = "0"; break;
		case 1: p1s = "1"; break;
		case 2: p1s = "2"; break;
		case 3: p1s = "3"; break;
		case 4: p1s = "4"; break;
		case 5: p1s = "5 Winner"; break;
    }
    switch( player_2.score ) {
		case 0: p2s = "0"; break;
		case 1: p2s = "1"; break;
		case 2: p2s = "2"; break;
		case 3: p2s = "3"; break;
		case 4: p2s = "4"; break;
		case 5: p2s = "5 Winner"; break;
    }
	//glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, width, 0.0, height);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
	glColor3f(1.0f, 0.0f, 0.0f); //needs to be called before RasterPos

    void * font1 = GLUT_BITMAP_9_BY_15;
    void * font2 = GLUT_BITMAP_9_BY_15;
    void * font3 = GLUT_BITMAP_9_BY_15;
    void * font4 = GLUT_BITMAP_9_BY_15;

	// player 1
	glRasterPos2i(width-100, height/2+30);
    for(string::iterator i = p1.begin(); i != p1.end(); ++i) {
        char c = *i;
        glutBitmapCharacter(font1, c);
    }

	// player 2
    glRasterPos2i(width-100, height/2-20);
    for(string::iterator i = p2.begin(); i != p2.end(); ++i) {
        char c = *i;
        glutBitmapCharacter(font2, c);
    }

	// player 1 score
    glRasterPos2i(width-100, height/2+10);
    for(string::iterator i = p1s.begin(); i != p1s.end(); ++i) {
        char c = *i;
        glutBitmapCharacter(font3, c);
    }

	// player 2 score
    glRasterPos2i(width-100, height/2-40);
    for(string::iterator i = p2s.begin(); i != p2s.end(); ++i) {
        char c = *i;
        glutBitmapCharacter(font4, c);
    }

    glEnable(GL_TEXTURE_2D);
    glutSwapBuffers();
    // end text -------------------------------------------------

    glFlush();
}

void key( unsigned char key, int x, int y ) {
	//printf( "Key: %c, x: %d, y: %d\n", key, x, y );
	int inc = 5;
	switch( key ) {
		case 27:
			exit(EXIT_SUCCESS);
			break;
		// player 1
		// left
		case 'a':
			if( player_1.x > 10+player_1.radius ) player_1.x-=inc;
			break;
		// right
		case 's':
			if( player_1.x < width-player_1.radius-10 ) player_1.x+=inc;
			break;
		// up
		case 'w':
			if( player_1.y < height/2-player_1.radius ) player_1.y+=inc;
			break;
		// down
		case 'z':
			if( player_1.y > 10+player_1.radius ) player_1.y-=inc;
			break;
		// player 2
		// left
		case 'j':
			if( player_2.x > 10+player_2.radius ) player_2.x-=inc;
			break;
		// right
		case 'k':
			if( player_2.x < width-player_2.radius-10 ) player_2.x+=inc;
			break;
		// up
		case 'i':
			if( player_2.y < height-player_2.radius-10 ) player_2.y+=inc;
			break;
		// down
		case 'm':
			if( player_2.y > height/2+player_2.radius ) player_2.y-=inc;
			break;
		// starts game
		case '1':
			if( !game_over ) game_status = RUNNING;
	}
    glutPostRedisplay();
}

void idle() {
	glutPostRedisplay();
}

int main( int argc, char** argv ) {
	createColor();

	glutInit( &argc, argv );
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(width, height);
    glutInitWindowPosition(0, 0);
	//glutEnterGameMode();
	glutCreateWindow("Glow Hockey");
	//gluOrtho2D( 0, width, 0, height );
	init();
	glutIdleFunc(idle);
	glutDisplayFunc(display);
	glutKeyboardFunc(key);
    glutReshapeFunc(reshape);

	glutMainLoop();
 }
