// X: 0 - 83 = 84
// Y: 0 - 47 = 48
//#include <Adafruit_GFX.h>
//#include <Adafruit_PCD8544.h>
#include <math.h>
#include <time.h>
#include "display.h"
#include <string>
#include "Serial.h"

using namespace std;

#define String string

class CardBoy {
private:
	Display display;

	int *keypressed;

	bool GetButtonPress(String key) {

		if (*keypressed == 1 && key == "Up") {
			return true;
		}
		else if (*keypressed == 2 && key == "Down") {
			return true;
		}

		if (*keypressed > 0 && key == "Any") {
			return true;
		}

		return false;
	}

	Serial_ Serial;

public:

	CardBoy(sf::RenderWindow *window, float zoomFactor, int *keypressed) {
		display = Display(window, zoomFactor);
		this->keypressed = keypressed;
	}

	~CardBoy() {

	}

	void (*delay)(int time);
	int(*millis)();
	
	//TODO: ARQUIVO Cardboy.ino ENTRA AQUI

	//TODO: includes no topo
	//#include <Adafruit_GFX.h>
	//#include <Adafruit_PCD8544.h>
	//#include <math.h>
	//#include <time.h>

	// X: 0 - 83 = 84
	// Y: 0 - 47 = 48

	struct coord {
		float x;
		float y;

		coord(float _x, float _y) {
			x = _x;
			y = _y;
		}
	};

	//TODO: não há necessidade de uso dessas variáveis
	// PORTAS USADAS PELO DISPLAY
	//const int pd1 = 8;
	//const int pd2 = 9;
	//const int pd3 = 10;
	//const int pd4 = 11;
	//const int pd5 = 12;

	//TODO: display substituido pelo da SFML
	//Adafruit_PCD8544 display = Adafruit_PCD8544(pd1, pd2, pd3, pd4, pd5);

	enum state
	{
		INITIALIZING = -1,
		TITLE_SCREEN = 0,
		MAIN_MENU = 1,
		SNAKE = 2,
		PONG = 3,
		BREAKOUT = 4,
		ENDGAME = 5
	};

	state current = INITIALIZING;

	float currentTime = 0;
	float lastTime = 0;
	float deltaTime = 0;

	const int buttonPin[4] = { 2, 3, 4, 5 }; //up, down, left, right
	String endGameFeedback = "";

	typedef struct 
	{
		coord ball = coord(42, 24);
		float players[2] = { 24 - 48 / 4 / 2, 24 - 48 / 4 / 2 };

		coord ballDir = coord(1, 0);
	} Pong;

	typedef struct
	{
		int rows = 5;
		int blocksPerRow = 5;
		coord ball = coord(42, 24);
		coord ballDir = coord(1, 0);
		float ballSpeed = 20;

		float player = 42;

		int *blocks;
		bool *blockLife;

	} Breakout;

	Pong* game_pong;
	Breakout* game_breakout;
	bool endedGame = false;

	coord RotateVector(coord vector, float graus) {
		float ang = 0.0174533f * graus;

		float x = cos(ang) * vector.x - sin(ang) * vector.y;
		float y = sin(ang) * vector.x + cos(ang) * vector.y;

		return coord(x, y);
	}

	void InitPong() {
		free(game_pong);
		game_pong = (Pong*)malloc(sizeof(Pong));

		game_pong->ball = coord(42, 12 + (rand() % 24));

		game_pong->players[0] = 24 - 48 / 4 / 2;
		game_pong->players[1] = 24 - 48 / 4 / 2;

		game_pong->ballDir = RotateVector(coord(1, 0), rand() % 360);

		endedGame = false;
		current = PONG;
	}

	void InitBreakout() {
		free(game_breakout);
		game_breakout = (Breakout*)malloc(sizeof(Breakout));

		game_breakout->rows = 5;
		game_breakout->blocksPerRow = 5;

		game_breakout->ball = coord(42, 30);
		game_breakout->player = 42;

		game_breakout->ballDir = RotateVector(coord(1, 0), -(rand() % 90 + 45));
		game_breakout->ballSpeed = 20;

		game_breakout->blocks = (int*)malloc((game_breakout->blocksPerRow+1)*game_breakout->rows * sizeof(int));
		game_breakout->blockLife = (bool*)malloc((game_breakout->blocksPerRow + 1)*game_breakout->rows * sizeof(bool));
		for (int i = 0; i < (game_breakout->blocksPerRow + 1)*game_breakout->rows; i++) {
			game_breakout->blockLife[i] = true;
		}

		endedGame = false;
		current = BREAKOUT;
	}

	void drawError()
	{
		display.setTextSize(2);
		display.setCursor(0, 6);
		display.println("ERROR");
	}

	void drawTitleScreen()
	{
		display.setTextSize(2);
		display.setCursor(0, 6);
		display.println("CARDBOY");

		display.drawLine(0, 3, 83, 3, 1);
		display.drawLine(0, 22, 83, 22, 1);

		display.setTextSize(1);
		display.setCursor(0, 35);
		display.println("*press start*");


		if (GetButtonPress("Any")) {
			//InitPong();
			InitBreakout();
		}
	}

	void drawMainMenu()
	{
		display.fillRect(0, 0, 39, 11, BLACK);
		display.setTextSize(1);

		display.setTextColor(WHITE);
		display.setCursor(2, 2);
		display.println("SELECT");

		display.drawRect(0, 0, 83, 48, BLACK);

		display.setTextColor(BLACK);

		display.setCursor(15, 15);
		display.println("SNAKE");

		display.setCursor(15, 25);
		display.println("PONG");

		display.setCursor(15, 35);
		display.println("BREAKOUT");

	}

	void drawEndScreen()
	{
		display.setTextSize(1);
		display.setCursor(0, 6);
		display.println(endGameFeedback);

		if (endedGame) {
			display.setTextSize(1);
			display.setCursor(0, 35);
			display.println("*press start*");
		}
		else {
			delay(3000);
			endedGame = true;
		}

		if (GetButtonPress("Any")) {

			//InitPong();
			InitBreakout();
		}
	}

	void prepareScene()
	{
		/*
		clear display
		check state
		draw new scene accordingly
		*/
		display.clearDisplay();

		switch (current)
		{
		default:
		{
			drawError();
			break;
		}
		case 0:
		{
			drawTitleScreen();
			break;
		}
		case 1:
		{
			drawMainMenu();
			break;
		}
		case 2:
		{

			break;
		}
		case 3:
		{
			pong();
			break;
		}
		case 4:
		{
			breakout();
			break;
		}
		case 5:
		{
			drawEndScreen();
			break;
		}
		}
	}

	void setup()
	{
		//Serial.begin(9600);
		srand(time(0));

		//TODO: não usamos essas interfaces
		//for (int i = 0; i < 4; i++) {
		//	pinMode(buttonPin[i], INPUT);
		//}

		display.begin();
		display.setContrast(50);
		display.clearDisplay();
		display.setTextColor(BLACK);

		display.setTextSize(2);
		display.setCursor(0, 6);
		display.println("CARDBOY");

		display.drawLine(0, 3, 83, 3, 1);
		display.drawLine(0, 22, 83, 22, 1);

		display.display();

		delay(1500);

		current = TITLE_SCREEN;
		//current = PONG;

		lastTime = millis();
	}

	/*
	LOOP BASIC STRUCTURE:
	draw current scene
	check for inputs
	apply game logic
	display scene
	*/

	void loop()
	{
		lastTime = currentTime;
		currentTime = millis();
		deltaTime = (currentTime - lastTime) / 1000;
		prepareScene();

		//Serial.println(rand());

		display.display();
	}

	/* --------------------------------------------------------- */

	coord Bounce(coord vector, coord normal, float delta) {

		bool wall = sqrt(pow(delta, 2)) > 5;

		if (wall) {
			coord rin = coord(-vector.x, -vector.y);

			float cos1 = normal.x * rin.x + normal.y * rin.y;
			return coord(2 * normal.x * cos1 - rin.x, 2 * normal.y * cos1 - rin.y);
		}

		float ang = -90 * delta;
		int I = ang >= 0 ? 1 : -1;

		ang = I * max(sqrt(pow(ang, 2)), 22.5f);
		ang = I * min(sqrt(pow(ang, 2)), 67.5f);

		//Serial.println(ang);
		//Serial.println(I);
		//Serial.println(sqrt(pow(ang, 2)));

		return RotateVector(normal, ang);
	}

	void pong() {

		game_pong->ball.x += (game_pong->ballDir.x * 20) * deltaTime;
		game_pong->ball.y += (game_pong->ballDir.y * 20) * deltaTime;

		//movimentos  

		//player
		if (GetButtonPress("Up")) {
			game_pong->players[0] -= 15 * deltaTime;
		}
		else if (GetButtonPress("Down")) {
			game_pong->players[0] += 15 * deltaTime;
		}

		//ai
		char dir = ((game_pong->ball.y - 6 - game_pong->players[1]) > 0 ? 1 : -1);
		game_pong->players[1] += 15 * dir * deltaTime;

		//colisões
		if (game_pong->ball.x > 82) {
			//player 1 ponto
			endGameFeedback = "You win!";
			current = ENDGAME;
			//right = false;
		}

		if (game_pong->ball.x < 2) {
			//player 2 ponto
			endGameFeedback = "You lose!";
			current = ENDGAME;
			//right = true;
		}

		if (game_pong->ball.y > 46) {
			game_pong->ballDir = Bounce(game_pong->ballDir, coord(0, -1), 10);
		}
		else if (game_pong->ball.y < 2) {
			game_pong->ballDir = Bounce(game_pong->ballDir, coord(0, 1), 10);
		}

		//bola colide dentro da parede -> dot
		//players
		if (game_pong->ball.x < 6) {

			if (game_pong->ball.y > game_pong->players[0] && game_pong->ball.y < game_pong->players[0] + 48 / 4) {
				game_pong->ballDir = Bounce(game_pong->ballDir, coord(1, 0), 1 + ((game_pong->players[0] - game_pong->ball.y) / 12) * 2);
			}
		}

		if (game_pong->ball.x > 84 - 6) {

			if (game_pong->ball.y > game_pong->players[1] && game_pong->ball.y < game_pong->players[1] + 48 / 4) {
				game_pong->ballDir = Bounce(game_pong->ballDir, coord(-1, 0), -(1 + ((game_pong->players[1] - game_pong->ball.y) / 12) * 2));
			}
		}




		display.drawFastVLine(42, 0, 48, 1);
		display.drawRect(0, 0, 84, 48, 1);

		display.drawFastVLine(4, game_pong->players[0], 48 / 4, 1);
		display.drawFastVLine(6, game_pong->players[0], 48 / 4, 1);

		display.drawFastVLine(84 - 4, game_pong->players[1], 48 / 4, 1);
		display.drawFastVLine(84 - 6, game_pong->players[1], 48 / 4, 1);

		display.fillCircle(game_pong->ball.x, game_pong->ball.y, 2, 1);

		//Serial.println(ball.x);
		//Serial.println(ball.y);
	}

	void breakout() {

		game_breakout->ball.x += (game_breakout->ballDir.x * game_breakout->ballSpeed) * deltaTime;
		game_breakout->ball.y += (game_breakout->ballDir.y * game_breakout->ballSpeed) * deltaTime;

		if (GetButtonPress("Up")) {
			game_breakout->player -= 40 * deltaTime;
		}
		else if (GetButtonPress("Down")) {
			game_breakout->player += 40 * deltaTime;
		}

		if (game_breakout->ball.x > game_breakout->player && game_breakout->ball.x < (game_breakout->player + 20)) {
			if (game_breakout->ball.y > 48-5) {
				const float f = (1 - (((game_breakout->ball.x - game_breakout->player) / 20) * 2));
				game_breakout->ballDir = Bounce(game_breakout->ballDir, coord(0, -1), f);
			}
		}

		bool win = true;
		for (int i = 0; i < (game_breakout->blocksPerRow + 1)*game_breakout->rows; i++) {
			if (game_breakout->blockLife[i] == true) {
				win = false;
				break;
			}
		}

		if (win) {
			endGameFeedback = "You Win!";
			current = ENDGAME;
		}

		if (game_breakout->ball.y > 48) {
			endGameFeedback = "You Lose!";
			current = ENDGAME;

			//game_breakout->ballDir = Bounce(game_breakout->ballDir, coord(0, -1), 10);
		}
		else if (game_breakout->ball.y < 2) {
			game_breakout->ballDir = Bounce(game_breakout->ballDir, coord(0, 1), 10);
		}

		if (game_breakout->ball.x < 2) {
			game_breakout->ballDir = Bounce(game_breakout->ballDir, coord(1, 0), 10);
		}
		else if (game_breakout->ball.x > 84 - 2) {
			game_breakout->ballDir = Bounce(game_breakout->ballDir, coord(-1, 0), 10);
		}

		const int blockSize = 84 / game_breakout->blocksPerRow;

		for (int i = 0; i < (game_breakout->blocksPerRow + 1)*game_breakout->rows; i++) {
			if (game_breakout->blockLife[i] == true) {
				const int j = i%(game_breakout->blocksPerRow+1);
				const int y = (i/ (game_breakout->blocksPerRow + 1)*5);
				const int deltaX = j + (-(y % 2) * blockSize / 2);
				const int x = (j*blockSize + deltaX);

				if (game_breakout->ball.x > x && game_breakout->ball.x < (x + blockSize)) {
					if (game_breakout->ball.y < y + 1 && game_breakout->ball.y > y - 1) {
						const float f = -(1-(((game_breakout->ball.x - x) / blockSize)*2));
						game_breakout->ballDir = Bounce(game_breakout->ballDir, coord(0, (game_breakout->ballDir.y>0)?-1:1),f);
						game_breakout->blockLife[i] = false;
						game_breakout->ballSpeed = 8.0f + max(game_breakout->ballSpeed,((game_breakout->blocksPerRow + 1)-j) * 2.0f);
					}
				}

				display.drawFastHLine(x, y, blockSize, 1);
			}
		}

		display.fillCircle(game_breakout->ball.x, game_breakout->ball.y, 2, 1);
		display.drawFastHLine(game_breakout->player, 48 - 5, 20, 1);

	}

	//TODO: Essa função não entra, pois foi substituida
	//bool GetButtonPress(String key) {
	//	if (key == "Up") {
	//		return digitalRead(buttonPin[0]);
	//	}
	//	else if (key == "Down") {
	//		return digitalRead(buttonPin[1]);
	//	}
	//	/*else if (key == "Left"){
	//	return digitalRead(buttonPin[2]);
	//	}
	//	else if (key == "Right"){
	//	return digitalRead(buttonPin[3]);
	//	}*/

	//	if (key == "Any") {
	//		for (int i = 0; i < 2; i++) {
	//			if (digitalRead(buttonPin[i])) {
	//				return true;
	//			}
	//		}
	//	}

	//	return false;
	//}	
};


