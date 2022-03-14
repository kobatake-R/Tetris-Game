#include <iostream>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
using namespace std;
#define FIELD_WIDTH (10)
#define FIELD_HEIGHT (20)

#define SHAPE_WIDTH_MAX (4)
#define SHAPE_HEIGHT_MAX (4)

#define FPS (1)
#define INTERVAL (1000/FPS)

enum { //テトリミノ7種
	SHAPE_I,
	SHAPE_O,
	SHAPE_S,
	SHAPE_Z,
	SHAPE_J,
	SHAPE_L,
	SHAPE_T,
	SHAPE_MAX
};

typedef struct {//テトリミノ実装用構造体
	int width, height;
	int date[SHAPE_HEIGHT_MAX][SHAPE_WIDTH_MAX];
}SHAPE;

typedef struct {//画面表示用構造体
	int x, y;
	int score;
	int gameover;
	SHAPE shape;
	SHAPE nextshape;
}MINO;

SHAPE shapes[SHAPE_MAX] = {
	//SHAPE_I
	{
		4,4,
		{
			{0,0,0,0},
			{1,1,1,1},
			{0,0,0,0},
			{0,0,0,0}
		}
	},
	//SHAPE_O
	{
		2,2,
		{
			{1,1},
			{1,1}
		}
	},
	//SHAPE_S
	{
		3,3,
		{
			{0,1,1},
			{1,1,0},
			{0,0,0}
		}
	},
	//SHAPE_Z
	{
		3,3,
		{
			{1,1,0},
			{0,1,1},
			{0,0,0}
		}
	},
	//SHAPE_J
	{
		3,3,
		{
			{1,0,0},
			{1,1,1},
			{0,0,0}
		}
	},
	//SHAPE_L
	{
		3,3,
		{
			{0,0,1},
			{1,1,1},
			{0,0,0}
		}
	},
	//SHAPE_T
	{
		3,3,
		{
			{0,1,0},
			{1,1,1},
			{0,0,0}
		}
	},
};

int field[FIELD_HEIGHT][FIELD_WIDTH];
int screen[FIELD_HEIGHT][FIELD_WIDTH];

MINO mino;

bool PenetrationField() {
	for (int i = 0; i < mino.shape.height; i++) {
		for (int j = 0; j < mino.shape.width; j++) {
			if (mino.shape.date[i][j]) {
				if ((mino.y+i<0)||(mino.y+i>=FIELD_HEIGHT)||(mino.x+j<0)||(mino.x+j>=FIELD_WIDTH)) {
					return true;
				}
				if (field[mino.y + i][mino.x + j]) {
					return true;
				}
			}
		}
	}
	return false;
}

void DrawScreen() {//画面描写用
	memcpy(screen, field, sizeof field);

		for(int i = 0; i < mino.shape.height; i++) {
			for (int j = 0; j < mino.shape.width; j++) {
				if (mino.shape.date[i][j]) {
					if (!screen[mino.y + i][mino.x + j]) {//選ばれたテトリミノと同じ形を表示
						screen[mino.y + i][mino.x + j] |= 1;
					}
					else {//既にブロックがある場合はゲームオーバー用変数を１に変える
						mino.gameover = 1;
					}
				}
			}
		}

		system("cls");//画面のクリア化

		cout << "現在のスコア：" << mino.score << endl << "next" << endl;

		for (int i = 0; i < SHAPE_HEIGHT_MAX; i++) {//Next表示
			for (int j = 0; j < SHAPE_WIDTH_MAX; j++) {
				if (mino.nextshape.date[i][j]) {
					cout << "■";
				}
				else {
					cout << "　";
				}
			}
			cout << endl;
		}

		for (int i = 0; i < FIELD_HEIGHT; i++) {//ゲーム画面の描写
			cout << "□";//左壁
			for (int j = 0; j < FIELD_WIDTH; j++) {
				if (screen[i][j]!=0) {
					cout << "■";
				}
				else {
					cout << "　";
				}
			}
			cout << "□";//右壁
			cout << endl;
		}
		for (int i = 0; i < FIELD_WIDTH+2; i++) {//下段表示
			cout << "□";
		}
}

void SpinShape() {//テトリミノ回転
	{
		MINO newMino = mino;//情報を一度コピーする
		for (int i = 0; i < mino.shape.height; i++) {
			for (int j = 0; j < mino.shape.width; j++) {
				newMino.shape.date[mino.shape.width - 1 - j][i] = mino.shape.date[i][j];
			}
		}
		mino = newMino;
	}
}

void Bottomshape() {
	for (int i = 0; i < FIELD_HEIGHT; i++) {
		if (screen[mino.y + i][mino.x]) {
			mino.y += i-1;
		}
	}
}

void InitMino() {//テトリミノを初期位置に配置
	mino.shape = mino.nextshape;
	mino.nextshape = shapes[rand() % SHAPE_MAX];
	mino.x = (FIELD_WIDTH - mino.shape.width) / 2;
	mino.y = 0;
}

void Init() {//初期化用
	memset(field, 0, sizeof field);
	InitMino();
	DrawScreen();
}

int main() {
	srand((unsigned int)time(NULL));
	mino.nextshape = shapes[rand() % SHAPE_MAX];
	Init();
	double count = 0;
	clock_t lastClock = clock();
	while (1) {
		clock_t nowClock = clock();
		if (nowClock >= lastClock + INTERVAL - count) {//テトリミノが自動で落ちる用にしてる
			count += 0.001;
			lastClock = nowClock;

			MINO lastMino = mino;
			mino.y++;

			if (PenetrationField()) {
				mino = lastMino;

				for (int i = 0; i < mino.shape.height; i++) {//設置テトリミノを保存
					for (int j = 0; j < mino.shape.width; j++) {
						if (mino.shape.date[i][j]) {
							field[mino.y + i][mino.x + j] |= 1;
						}
					}
				}
				for (int i = 0; i < FIELD_HEIGHT; i++) {//ライン消しをした時
					bool comleted = true;
					for (int j = 0; j < FIELD_WIDTH; j++) {
						if (!field[i][j]) {
							comleted = false;
							break;
						}
					}
					if (comleted) {//ライン消しをした時ブロックを下にずらしてくる
						for (int j = 0; j < FIELD_WIDTH; j++) {
							field[i][j] = 0;
						}
						for (int k = i; k >= 1; k--) {
							for (int l = 0; l < FIELD_WIDTH; l++) {
								field[k][l] = field[k - 1][l];
								field[k - 1][l] = 0;
							}
						}
						mino.score += 1000;
					}
				}
				InitMino();
			}
			if (mino.gameover) {//終了
				cout << endl << "今回のスコア：" << mino.score << "　GAME OVER";
				_getch();
				return 0;

			}
			DrawScreen();
		}

		if (_kbhit()) {//操作
			MINO lastMino = mino;
			switch (_getch())
			{
			case 'w':
				//mino.y += 2;
				for (int i = 0; i < FIELD_HEIGHT; i++) {
					if (!screen[mino.y][mino.x]) {
						if (screen[mino.y + i][mino.x]) {
							mino.y += i - 1;
							break;
						}
					}
				}
				mino.y += FIELD_HEIGHT;
				break;
			case 's'://下
				mino.y++;
				break;
			case 'a'://左
				mino.x--;
				break;
			case 'd'://右
				mino.x++;
				break;
			default://テトリミノ回転
				SpinShape();
				break;
			}
			if (PenetrationField()) {
				mino = lastMino; //壁貫通できないように貫通する場合無かったことにする
			}
			DrawScreen();
		}
	}
}