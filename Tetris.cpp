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

enum { //�e�g���~�m7��
	SHAPE_I,
	SHAPE_O,
	SHAPE_S,
	SHAPE_Z,
	SHAPE_J,
	SHAPE_L,
	SHAPE_T,
	SHAPE_MAX
};

typedef struct {//�e�g���~�m�����p�\����
	int width, height;
	int date[SHAPE_HEIGHT_MAX][SHAPE_WIDTH_MAX];
}SHAPE;

typedef struct {//��ʕ\���p�\����
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

void DrawScreen() {//��ʕ`�ʗp
	memcpy(screen, field, sizeof field);

		for(int i = 0; i < mino.shape.height; i++) {
			for (int j = 0; j < mino.shape.width; j++) {
				if (mino.shape.date[i][j]) {
					if (!screen[mino.y + i][mino.x + j]) {//�I�΂ꂽ�e�g���~�m�Ɠ����`��\��
						screen[mino.y + i][mino.x + j] |= 1;
					}
					else {//���Ƀu���b�N������ꍇ�̓Q�[���I�[�o�[�p�ϐ����P�ɕς���
						mino.gameover = 1;
					}
				}
			}
		}

		system("cls");//��ʂ̃N���A��

		cout << "���݂̃X�R�A�F" << mino.score << endl << "next" << endl;

		for (int i = 0; i < SHAPE_HEIGHT_MAX; i++) {//Next�\��
			for (int j = 0; j < SHAPE_WIDTH_MAX; j++) {
				if (mino.nextshape.date[i][j]) {
					cout << "��";
				}
				else {
					cout << "�@";
				}
			}
			cout << endl;
		}

		for (int i = 0; i < FIELD_HEIGHT; i++) {//�Q�[����ʂ̕`��
			cout << "��";//����
			for (int j = 0; j < FIELD_WIDTH; j++) {
				if (screen[i][j]!=0) {
					cout << "��";
				}
				else {
					cout << "�@";
				}
			}
			cout << "��";//�E��
			cout << endl;
		}
		for (int i = 0; i < FIELD_WIDTH+2; i++) {//���i�\��
			cout << "��";
		}
}

void SpinShape() {//�e�g���~�m��]
	{
		MINO newMino = mino;//������x�R�s�[����
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

void InitMino() {//�e�g���~�m�������ʒu�ɔz�u
	mino.shape = mino.nextshape;
	mino.nextshape = shapes[rand() % SHAPE_MAX];
	mino.x = (FIELD_WIDTH - mino.shape.width) / 2;
	mino.y = 0;
}

void Init() {//�������p
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
		if (nowClock >= lastClock + INTERVAL - count) {//�e�g���~�m�������ŗ�����p�ɂ��Ă�
			count += 0.001;
			lastClock = nowClock;

			MINO lastMino = mino;
			mino.y++;

			if (PenetrationField()) {
				mino = lastMino;

				for (int i = 0; i < mino.shape.height; i++) {//�ݒu�e�g���~�m��ۑ�
					for (int j = 0; j < mino.shape.width; j++) {
						if (mino.shape.date[i][j]) {
							field[mino.y + i][mino.x + j] |= 1;
						}
					}
				}
				for (int i = 0; i < FIELD_HEIGHT; i++) {//���C��������������
					bool comleted = true;
					for (int j = 0; j < FIELD_WIDTH; j++) {
						if (!field[i][j]) {
							comleted = false;
							break;
						}
					}
					if (comleted) {//���C���������������u���b�N�����ɂ��炵�Ă���
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
			if (mino.gameover) {//�I��
				cout << endl << "����̃X�R�A�F" << mino.score << "�@GAME OVER";
				_getch();
				return 0;

			}
			DrawScreen();
		}

		if (_kbhit()) {//����
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
			case 's'://��
				mino.y++;
				break;
			case 'a'://��
				mino.x--;
				break;
			case 'd'://�E
				mino.x++;
				break;
			default://�e�g���~�m��]
				SpinShape();
				break;
			}
			if (PenetrationField()) {
				mino = lastMino; //�Ǌђʂł��Ȃ��悤�Ɋђʂ���ꍇ�����������Ƃɂ���
			}
			DrawScreen();
		}
	}
}