#pragma once#define _CRT_SECURE_NO_WARNINGS


#ifndef __SNAKE_H_
#define __SNAKE_H_
#define MAX 500
#define LETTERNUM 15

#pragma message("�ҵ�̰���ߣ�")

#include<stdio.h>
#include<conio.h>
#include<time.h>//ʱ��
#include<math.h>//��ѧ
#include<graphics.h>//easyxͼ�ο�ͷ�ļ�
#include <windows.h>//����
#include <mmsystem.h>//����
#pragma comment(lib, "WINMM.LIB")//����


/**************************************************///������
//����ȫ�ֱ�����ʼ��
void inall();
/**************************************************///��
//��ʼ����
void insnake();
//������
void inisnake();
//�ߵ��ƶ�
void way();
//�ߵķ�����ƣ����̣�
void snakemove();
//�ж����Ƿ�ҵ�
void snakedie();
/**************************************************///ʳ��
//�ߵ�ʳ��
void foodxy();
//ʳ�����
void inifood();
//�ж����Ƿ�Ե�ʳ��
void eatfood();
/**************************************************///�������
//��ʼ����
void welcome();
//��������
void finish();
//��Ϸ�����У��� �ո� ���Զ���Ϸ������ͣ����ʼ
void again();
//��ʾ��Ϣ
void text();
//�л�����(��ɫ)
void change();
//�л����棨��ɫ��
void change1();
//��Ϸ�ɹ�����
void success();
/**************************************************///ģʽѡ��
//����5��ģʽ��ͼƬ
void inimage();
//ģʽ�л�
void stylechoose();
//����ģʽ����Ϸ����
void stylehelp();
//��Ϸ��ʼ
void begin();
//����ģʽѡ��
void creatychoose();
/**************************************************///�ṹ��
//�洢�ߵ�����Ľṹ��
typedef struct coor
{
	int x;
	int y;
}coor;
//�ߵĻ�������
struct snake
{
	int num;//�ߵ��ܽ���
	coor xy[MAX];//�洢�ߵ�����Ľṹ������
	char way;

};
//ʳ��Ļ�������
struct food
{
	coor fdxy;
	int flag ;
	int eatgrade ;
};
//��ĸ������
struct letter
{
	int num[LETTERNUM];
};
//����ö��
enum way
{
	right = 72,
	left = 75,
	down = 77,
	up = 80
};
/**************************************************///С��ģʽ
//��ʼ��С��
void insball();
//����С��
void iniball();
//С����ƶ������̣�
void wayball();
//С��ķ�����ƣ����̣�
void ballmove();
//�߸���С���ƶ�
void sbmove();
//С��ײǽ����
void ballwall();
//������
void sbdie();
/**************************************************///С��ģʽ(���)
//С��ķ�����ƣ���꣩
void ballmoveMS();
//�ߵ��ƶ�����꣩
void wayMS();
/**************************************************///�ϰ���
//�ϰ����ʼ��
void insbrr();
//�ϰ������
void inibrr();
//�ϰ�����ײ���
void bomp();
//�ϰ�����ײ��⣨��Ҷ���
void bomp2();
/**************************************************///��ͼ
//�ؿ�һ
void level_1();
//�ؿ���
void level_2();
//�ؿ���
void level_3();
//С��ģʽ��ͼ
void mapballstyle();
//˫��ģʽ��ͼ
void maptwostyle();
//˫��ģʽ��ͼ
void maptogetherstyle();
//��ͼ���ĵ�ͼ(������ѭ����������һ��)
void mapmap();
/**************************************************///����
//����λ��
void trsxy();
//��ʾ��Ϣ
void clue();
//�Ա���
void eattrs();
/**************************************************///��Ҷ�
//��ʼ����(��Ҷ�)
void insnake2();
//������(��Ҷ�)
void inisnake2();
//�ߵ��ƶ������̣�(��Ҷ�)
void way2();
//�ߵķ�����ƣ����̣�(��Ҷ�)
void snakemove2();
//�ж����Ƿ�����(��Ҷ�)
void snakedie2();
//��ʳ����ж�(��Ҷ�)
void eatfood2();
/**************************************************///˫��ģʽ
//�����ߵ���ײ���
void crash();
//��ʾ��Ϣ(˫��ģʽ)
void text2();
/**************************************************///ͬʱ����˫��
//�ߵķ�����ƣ����̣�(��Ҷ�)
void snakemovetwo();
/**************************************************///����
//��ʼ������
void infakesnake();
//���ߵ��ƶ�
void fakeway();
//���ߵķ������
void fakesnakemove();
//���ü��߻�������
void inirealsnake();
/**************************************************///��ͼ�ƶ�
//��ͼ�ƶ�
void mapway();
//��ͼ�ƶ��ߵ�λ��
void mapsnakexy();
//��ͼ�ƶ�ʳ�������ȡ
void mapfoodxy();
//���ͼ�߽��λ��
void mapwallxy();
//���ƴ��ͼ�߽�
void iniwall();
//�߽���ײ���
void bompwall();
//���ü����ж������Ƿ�ҧ���Լ�����
void mapsnakedie();
/**************************************************///��ĸģʽ
//��ĸ��ʼ��
void inletter();
//��ĸ�����ȡ
void letterxy();
//������ĸ
void iniletter();
//����ĸ
void eatletter();
//��ʼ����ĸ��
void library();
//��ĸ����ĸ���ж�
void same();
//��ʾ��Ϣ
void lettertext();
/**************************************************///ģʽ����
//����ģʽ
void freeStyle();
//����ģʽ
void levelStyle();
//����ģʽ-2
void levelStyle_2();
//����ģʽ-3
void levelStyle_3();
//����ģʽ-С�����
void ballStyle();
//����ģʽ-Ѱ��ģʽ
void seekStyle();
//����ģʽ-С�����(���)
void ballStyleMS();
//����ģʽ-˫���趯
void twoStyle();
//����ģʽ-��ͼ��
void mapStyle();
//����ģʽ��˫�ˣ�
void togetherStyle();
//��ĸģʽ
void letterStyle();
//���ģʽ
void randomStyle();

#endif
