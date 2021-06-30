#pragma once#define _CRT_SECURE_NO_WARNINGS


#ifndef __SNAKE_H_
#define __SNAKE_H_
#define MAX 500
#define LETTERNUM 15

#pragma message("我的贪吃蛇！")

#include<stdio.h>
#include<conio.h>
#include<time.h>//时间
#include<math.h>//数学
#include<graphics.h>//easyx图形库头文件
#include <windows.h>//音乐
#include <mmsystem.h>//音乐
#pragma comment(lib, "WINMM.LIB")//音乐


/**************************************************///主函数
//所有全局变量初始化
void inall();
/**************************************************///蛇
//初始化蛇
void insnake();
//绘制蛇
void inisnake();
//蛇的移动
void way();
//蛇的方向控制（键盘）
void snakemove();
//判断蛇是否挂掉
void snakedie();
/**************************************************///食物
//蛇的食物
void foodxy();
//食物绘制
void inifood();
//判断蛇是否吃掉食物
void eatfood();
/**************************************************///界面控制
//开始界面
void welcome();
//结束界面
void finish();
//游戏过程中，按 空格 可以对游戏进行暂停、开始
void again();
//显示信息
void text();
//切换界面(黄色)
void change();
//切换界面（黑色）
void change1();
//游戏成功界面
void success();
/**************************************************///模式选择
//加载5种模式的图片
void inimage();
//模式切换
void stylechoose();
//各种模式的游戏规则
void stylehelp();
//游戏开始
void begin();
//创意模式选择
void creatychoose();
/**************************************************///结构体
//存储蛇的坐标的结构体
typedef struct coor
{
	int x;
	int y;
}coor;
//蛇的基本属性
struct snake
{
	int num;//蛇的总节数
	coor xy[MAX];//存储蛇的坐标的结构体数组
	char way;

};
//食物的基本属性
struct food
{
	coor fdxy;
	int flag ;
	int eatgrade ;
};
//字母的属性
struct letter
{
	int num[LETTERNUM];
};
//方向枚举
enum way
{
	right = 72,
	left = 75,
	down = 77,
	up = 80
};
/**************************************************///小球模式
//初始化小球
void insball();
//绘制小球
void iniball();
//小球的移动（键盘）
void wayball();
//小球的方向控制（键盘）
void ballmove();
//蛇跟随小球移动
void sbmove();
//小球撞墙反弹
void ballwall();
//蛇死亡
void sbdie();
/**************************************************///小球模式(鼠标)
//小球的方向控制（鼠标）
void ballmoveMS();
//蛇的移动（鼠标）
void wayMS();
/**************************************************///障碍物
//障碍物初始化
void insbrr();
//障碍物绘制
void inibrr();
//障碍物碰撞检测
void bomp();
//障碍物碰撞检测（玩家二）
void bomp2();
/**************************************************///地图
//关卡一
void level_1();
//关卡二
void level_2();
//关卡三
void level_3();
//小球模式地图
void mapballstyle();
//双蛇模式地图
void maptwostyle();
//双人模式地图
void maptogetherstyle();
//地图动的地图(不加入循环，仅产生一次)
void mapmap();
/**************************************************///宝藏
//宝藏位置
void trsxy();
//提示消息
void clue();
//吃宝藏
void eattrs();
/**************************************************///玩家二
//初始化蛇(玩家二)
void insnake2();
//绘制蛇(玩家二)
void inisnake2();
//蛇的移动（键盘）(玩家二)
void way2();
//蛇的方向控制（键盘）(玩家二)
void snakemove2();
//判断蛇是否死亡(玩家二)
void snakedie2();
//吃食物的判断(玩家二)
void eatfood2();
/**************************************************///双人模式
//两条蛇的碰撞检测
void crash();
//显示信息(双人模式)
void text2();
/**************************************************///同时控制双蛇
//蛇的方向控制（键盘）(玩家二)
void snakemovetwo();
/**************************************************///假蛇
//初始化假蛇
void infakesnake();
//假蛇的移动
void fakeway();
//假蛇的方向控制
void fakesnakemove();
//利用假蛇绘制真蛇
void inirealsnake();
/**************************************************///地图移动
//地图移动
void mapway();
//地图移动蛇的位置
void mapsnakexy();
//地图移动食物坐标获取
void mapfoodxy();
//大地图边界的位置
void mapwallxy();
//绘制大地图边界
void iniwall();
//边界碰撞检测
void bompwall();
//利用假蛇判断真蛇是否咬到自己死亡
void mapsnakedie();
/**************************************************///字母模式
//字母初始化
void inletter();
//字母坐标获取
void letterxy();
//绘制字母
void iniletter();
//吃字母
void eatletter();
//初始化字母库
void library();
//字母与字母库判断
void same();
//显示信息
void lettertext();
/**************************************************///模式类型
//自由模式
void freeStyle();
//闯关模式
void levelStyle();
//闯关模式-2
void levelStyle_2();
//闯关模式-3
void levelStyle_3();
//创意模式-小球控制
void ballStyle();
//创意模式-寻宝模式
void seekStyle();
//创意模式-小球控制(鼠标)
void ballStyleMS();
//创意模式-双蛇舞动
void twoStyle();
//创意模式-地图动
void mapStyle();
//共生模式（双人）
void togetherStyle();
//字母模式
void letterStyle();
//随机模式
void randomStyle();

#endif
