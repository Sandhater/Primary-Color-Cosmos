#pragma once

#pragma region 头文件引用

// Windows 头文件: 
#include <windows.h>

// C 运行时头文件
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <time.h>
#include <mmsystem.h>

// 资源头文件
#include "resource.h"
#pragma comment(lib, "Msimg32.lib")			//图象处理的函数接口，例如：透明色的位图的绘制TransparentBlt函数
#pragma comment(lib, "Winmm.lib")


// TODO:  在此处引用程序需要的其他头文件
#include <vector>
#include <math.h>
#include <queue>
#include <stack>
#include <algorithm>


#pragma endregion

#pragma region 补充键值定义

#define VK_A 0x41
#define VK_B 0x42
#define VK_C 0x43
#define VK_D 0x44
#define VK_E 0x45
#define VK_F 0x46
#define VK_G 0x47
#define VK_H 0x48
#define VK_I 0x49
#define VK_J 0x4A
#define VK_K 0x4B
#define VK_L 0x4C
#define VK_M 0x4D
#define VK_N 0x4E
#define VK_O 0x4F
#define VK_P 0x50
#define VK_Q 0x51
#define VK_R 0x52
#define VK_S 0x53
#define VK_T 0x54
#define VK_U 0x55
#define VK_V 0x56
#define VK_W 0x57
#define VK_X 0x58
#define VK_Y 0x59
#define VK_Z 0x5A
#define VK_0 0x30
#define VK_1 0x31
#define VK_2 0x32
#define VK_3 0x33
#define VK_4 0x34
#define VK_5 0x35
#define VK_6 0x36
#define VK_7 0x37
#define VK_8 0x38
#define VK_9 0x39

#pragma endregion

#pragma region 宏定义

#define WINDOW_TITLEBARHEIGHT	0			//标题栏高度
#define WINDOW_WIDTH			1920		//游戏窗口宽度
#define WINDOW_HEIGHT			1080		//游戏窗口高度

#define STAGE_STARTMENU			0		//开始页面的ID
#define STAGE_HELP				-1		//帮助页面的ID
#define STAGE_FAIL				-2		//失败页面的ID
#define STAGE_VICTORY			-3		//胜利页面的ID
#define STAGE_INTRODUCE			-4		//背景介绍页面的ID
#define STAGE_1					1		//第一个游戏场景的ID
#define STAGE_2					2		//第二个游戏场景的ID
#define STAGE_3					3		//第三个游戏场景的ID


#define UNIT_SIZE_X				60		//单位的宽度
#define UNIT_SIZE_Y				60		//单位的高度
#define UNIT_LAST_FRAME			23		//单位的动画帧最大值

#define ENEMY_SIZE_X			60		//敌人的宽度
#define ENEMY_SIZE_Y			60		//敌人的高度
#define ENEMY_LAST_FRAME		11		//敌人的动画帧最大值

#define BOSS_SIZE_X				100		//BOSS的宽度
#define BOSS_SIZE_Y				100		//BOSS的高度

//单位类型定义
#define UNIT_TYPE_RED			0		//红色形态
#define UNIT_TYPE_YELLOW		1		//黄色形态
#define UNIT_TYPE_BLUE			2		//蓝色形态

//单位状态定义
#define UNIT_STATE_HOLD			0		//静止
#define UNIT_STATE_WALK			1		//行走
#define UNIT_STATE_ATTACK		2		//攻击
#define UNIT_STATE_S			3		//使用S技能
#define UNIT_STATE_D			4		//使用D技能
#define UNIT_STATE_F			5		//使用F技能

//单位方向定义
#define UNIT_DIRECT_RIGHT		0		//向右
#define UNIT_DIRECT_LEFT		1		//向左


//敌人类型定义
#define ENEMY_TYPE_A			0
#define ENEMY_TYPE_B			1
#define ENEMY_TYPE_C			2
#define BOSS					3

//敌人状态定义
#define ENEMY_STATE_HOLD		0		//静止
#define ENEMY_STATE_WALK		1		//行走
#define ENEMY_STATE_ATTACK		2		//攻击

//敌人方向定义
#define ENEMY_DIRECT_RIGHT		0		//向右
#define ENEMY_DIRECT_LEFT		1		//向左

//砖块网格定义
#define BLOCK_ROW_COUNT			16		//砖块横向最大数
#define BLOCK_COL_COUNT			27		//砖块纵向最大数

//砖块尺寸定义
#define BLOCK_WIDTH				120		//砖块宽度
#define BLOCK_HEIGHT			40		//砖块高度

//砖块类型定义
#define BLOCK_SOLID				0		//实体砖块
#define BLOCK_THORN				1		//尖刺砖块
#define BLOCK_SAND				2		//沙子砖块

//场景道具类型定义
#define ITEM_HP					0		//恢复体力道具
#define ITEM_MP					1		//恢复法力道具
#define ITEM_EXP				2		//增加经验道具

//道具尺寸定义
#define ITEM_SIZE_X				60		//道具宽度
#define ITEM_SIZE_Y				60		//道具高度

//道具奖励内容定义
#define ITEM_HP_VALUE			30
#define ITEM_MP_VALUE			30
#define ITEM_EXP_VALUE			80

//背景
#define BG_SRC_COUNT			6		//背景资源数量
#define BG_COLUMNS				1		//背景列数
#define BG_ROWS					1		//背景行数
#define BG_ROWS_SKY				3		//背景天空行数
#define BG_ROWS_LAND			9		//背景地面行数
#define BG_CELL_WIDTH			64		//背景单格宽度
#define BG_CELL_HEIGHT			64		//背景单格高度

//消息类型定义
#define NOTICE_HP_RECOVER		0		//生命值恢复
#define NOTICE_MP_RECOVER		1		//法力值恢复
#define NOTICE_EXP_ADD			2		//经验值增加
#define NOTICE_LEVELUP_2		3		//升到2级
#define NOTICE_LEVELUP_3		4		//升到3级
#define NOTICE_LEVELUP_4		5		//升到4级
#define NOTICE_LEVELUP_5		6		//升到5级
#define NOTICE_MANA_LIMIT		7		//法力值不够

//消息横幅尺寸
#define NOTICE_SIZE_X			320		//横幅宽度
#define NOTICE_SIZE_Y			72		//横幅高度

//按钮定义
#define INDICATOR_WIDTH			96		//选项指针宽度
#define INDICATOR_HEIGHT		56		//选项指针

#define BUTTON_STARTGAME			1001	//开始游戏按钮ID
#define BUTTON_STARTGAME_WIDTH		284		//开始游戏按钮宽度
#define BUTTON_STARTGAME_HEIGHT		68		//开始游戏按钮高度

#define BUTTON_HELP					1002	//游戏帮助按钮ID
#define BUTTON_HELP_WIDTH			284		//游戏帮助按钮宽度
#define BUTTON_HELP_HEIGHT			68		//游戏帮助按钮高度

#define BUTTON_EXITGAME				1003	//退出游戏按钮ID
#define BUTTON_EXITGAME_WIDTH		284		//退出游戏按钮宽度
#define BUTTON_EXITGAME_HEIGHT		68		//退出游戏按钮高度

#define BUTTON_CONTINUE				1004	//继续游戏按钮ID
#define BUTTON_CONTINUE_WIDTH		284		//继续游戏按钮宽度
#define BUTTON_CONTINUE_HEIGHT		68		//继续游戏按钮高度

#define BUTTON_HOME					1005	//返回主页按钮ID
#define BUTTON_HOME_WIDTH			284		//返回主页按钮宽度
#define BUTTON_HOME_HEIGHT			68		//返回主页按钮高度

#define BUTTON_BACK					1006	//返回开始页面按钮ID
#define BUTTON_BACK_WIDTH			68		//返回开始页面按钮宽度
#define BUTTON_BACK_HEIGHT			68		//返回开始页面按钮高度

//传送门定义
#define GATE_SIZE_X					60		//传送门宽度
#define GATE_SIZE_Y					120		//传送门高度
#define GATE1_LEFT					60		//上一关卡传送门左边界
#define GATE1_UP					880		//上一关卡传送门上边界
#define GATE2_LEFT					1800	//下一关卡传送门左边界
#define GATE2_UP					80		//下一关卡传送门上边界


//计时器定义
#define TIMER_GAMETIMER				100		//游戏的默认计时器ID
#define TIMER_GAMETIMER_ELAPSE		30		//默认计时器刷新间隔的毫秒数

#define TIMER_ATTACK				200		//攻击行为计时器
#define TIMER_ATTACK_ELAPSE			720		//攻击行为计时器时长

#define TIMER_S						300		//S技能行为计时器
#define TIMER_S_ELAPSE				720		//S技能行为计时器时长

#define TIMER_D						400		//D技能行为计时器
#define TIMER_D_ELAPSE				720		//D技能行为计时器时长

#define TIMER_F						500		//F技能行为计时器
#define TIMER_F_ELAPSE				720		//F技能行为计时器时长

#define TIMER_SHOCKED				600		//红色形态D技能造成的控制效果计时器
#define TIMER_SHOCKED_ELAPSE		2000	//红色形态D技能造成的控制效果计时器时长

#define TIMER_NOTICE				700		//消息横幅计时器
#define TIMER_NOTICE_ELAPSE			1000	//消息横幅计时器间隔

#define TIMER_ITEM					800		//最终关卡法力道具生成计时器
#define TIMER_ITEM_ELAPSE			15000	//最终关卡法力道具生成计时器间隔

#define TIMER_ENEMY_ATTACK			900		//敌人攻击计时器
#define TIMER_ENEMY_ATTACK_ELAPSE	720		//敌人攻击计时器时长

#define TIMER_INTRODUCE				1000	//背景介绍计时器
#define TIMER_INTRODUCE_ELAPSE		12000	//背景介绍计时器间隔

//其他定义
#define UNIT_DEFENCE				1		//单位伤害抵抗
#define UNIT_SPEED					6.0		//单位行走速度	
#define UNIT_JUMP_SPEED				-6.0	//单位跳跃垂直速度
#define UNIT_JUMP_HEIGHT_LIMIT		200.0	//跳跃高度限制
#define UNIT_FALL_SPEED				8.0		//单位下落垂直速度
#define UNIT_UPGRADE_NEED			200		//单位每次升级所需经验值
#define UNIT_ATTACK_DISTANCE		80		//单位攻击距离
#define UNIT_MANA_RECOVER			1		//单位每帧恢复法力值

#define CONTAINER_LENGTH			720		//血量或法力槽长度
#define CONTAINER_WIDTH				16		//血量或法力槽宽度
#define BAR_X						48		//血量或法力槽横坐标
#define HEALTH_BAR_Y				48		//血量槽纵坐标
#define MANA_BAR_Y					80		//法力槽纵坐标

//碰撞判定误差范围
#define TOLERANCE					9

#pragma endregion


#pragma region 全局变量

//选项指针y方向位置
int INDICATOR_Y[] = { WINDOW_HEIGHT / 2.55 + 0.12 * BUTTON_STARTGAME_HEIGHT,
	WINDOW_HEIGHT / 2.55 + 2.12 * BUTTON_STARTGAME_HEIGHT, WINDOW_HEIGHT / 2.55 + 4.12 * BUTTON_STARTGAME_HEIGHT };


//三形态的普通攻击伤害值
int attack_damages[] = { 40,30,30 };
//三个形态的移动速度修正值
double speed_mods[] = { -2.0, 0, 2.0 };
//三形态的伤害抵抗修正值
double defence_mods[] = { -0.3,0,0.3 };
//三形态S技能攻击范围
int S_distances[] = { 80, 80, 120 };
//三形态S技能伤害值
int S_damages[] = { 100,60,60 };
//三形态S技能法力消耗
int S_costs[] = { 30,10,25 };
//三形态D技能法力消耗
int D_costs[] = { 30,20,20 };
//三形态F技能攻击范围
int F_distances[] = { 80, 0, 0 };
//三形态F技能伤害值
int F_damages[] = { 200,50,50 };
//三形态F技能法力消耗
int F_costs[] = { 50,50,30 };

//四种敌人的基础生命值
int enemy_healths[] = { 80,100,80,1500 };
//四种敌人掉落的经验值
int enemy_exeperiences[] = { 40,100,70,0 };
//四种敌人掉落的法力值
int enemy_manas[] = { 15,15,15,0 };
//四种敌人的攻击伤害值
int enemy_damages[] = { 8, 5, 12, 12 };
//四种敌人的伤害抵抗系数
double enemy_defences[] = { 1,0.5,1.2,0.6 };
//四种敌人的移动速度
double enemy_speeds[] = { 4.0,2.0,0.0,4.0 };
//四种敌人位置最大左偏移
int enemy_left_offsets[] = { -120, -120, -120, -360 };
//四种敌人位置最大右偏移
int enemy_right_offsets[] = { 120,120,120,360 };
//四种敌人触发攻击的距离
int enemy_distance[] = { 60,60,240,80 };

#pragma endregion

#pragma region 结构体声明

// 场景结构体
struct Stage
{
	int stageID;		//场景编号
	HBITMAP bg;			//背景图片
	int timeCountDown;	//游戏时间倒计时
	bool timerOn;		//计时器是否运行（游戏是否被暂停）
	bool itemTimerOn;	//道具生成装置是否运行（仅限第三关）
};


// 按钮结构体
struct Button
{
	int buttonID;	//按钮编号
	bool visible;	//按钮是否可见
	HBITMAP img;	//图片
	int x;			//坐标x
	int y;			//坐标y
	int width;		//宽度
	int height;		//高度
};

//选项指针结构体
struct Indicator
{
	HBITMAP img;	//图片
	bool visible;	//指针是否可见
	int loc;		//指示位置
	int x;			//坐标x
	int y;			//坐标y
	int width;		//宽度
	int height;		//高度
};

// 主角结构体
struct Unit
{
	HBITMAP img;	//图片

	int frame_row;			//当前显示的是图像的第几行
	int frame_column;		//当前显示的是图像的第几列

	int* frame_sequence;		//当前的帧序列
	int frame_count;		//帧序列的长度
	int frame_id;			//当前显示的是帧序列的第几个

	int type;		//单位类型
	int state;		//单位状态
	int direction;	//单位方向

	int x;			//坐标x
	int y;			//坐标y
	double vx;		//速度x
	double vy;		//速度y

	int experience;	//经验值
	int level;		//等级
	int speed_mod;	//移动速度修正（对应不同形态）
	int health;		//生命值
	int mana;		//法力值
	double defence;	//伤害抵抗
	int jump_times;		//已跳跃次数
	int jump_height;	//已跳远高度
};

//敌人结构体
struct Enemy
{
	HBITMAP img;	//图片
	
	int frame_row;			//当前显示的是图像的第几行
	int frame_column;		//当前显示的是图像的第几列

	int* frame_sequence;		//当前的帧序列
	int frame_count;		//帧序列的长度
	int frame_id;			//当前显示的是帧序列的第几个

	int type;		//单位类型
	int state;		//单位状态
	int direction;	//单位方向

	int x;			//坐标x
	int y;			//坐标y
	double vx;		//速度x
	double vy;		//速度y
	int offset;	//相对初始位置的便宜

	int health;		//生命值
	double defence;	//伤害抵抗
	int exp;		//掉落经验
	int ma;			//掉落法力
};

//砖块结构体
struct Block
{
	HBITMAP img;	//图片

	int type;	//砖块类型
	int x;		//坐标x
	int y;		//坐标y
	bool visible;	//砖块是否可见
};

//消息结构体
struct Notice
{
	HBITMAP img;	//图片
	int type;		//消息类型
};

//场景道具结构体
struct Item
{
	HBITMAP img;	//图片
	int type;		//道具类型
	int x;
	int y;
};

//TODO: 添加游戏需要的更多种数据（地物、砖块等）


#pragma endregion


#pragma region 事件处理函数声明


// 初始化游戏窗体函数
void InitGame(HWND hWnd, WPARAM wParam, LPARAM lParam);

// 键盘按下事件处理函数
void KeyDown(HWND hWnd, WPARAM wParam, LPARAM lParam);

// 键盘松开事件处理函数
void KeyUp(HWND hWnd, WPARAM wParam, LPARAM lParam);

// 鼠标移动事件处理函数
void MouseMove(HWND hWnd, WPARAM wParam, LPARAM lParam);

// 鼠标左键按下事件处理函数
void LButtonDown(HWND hWnd, WPARAM wParam, LPARAM lParam);

// 鼠标左键松开事件处理函数
void LButtonUp(HWND hWnd, WPARAM wParam, LPARAM lParam);

// 定时器事件处理函数
void TimerUpdate(HWND hWnd, WPARAM wParam, LPARAM lParam);

#pragma endregion


#pragma region 其它游戏状态处理函数声明
//添加选项指针函数
Indicator* CreateIndicator(HBITMAP img, int width, int height, int x, int y);

// 添加按钮函数
Button* CreateButton(int buttonID, HBITMAP img, int width, int height, int x, int y);

// 添加单位函数
Unit* CreateUnit(int type, int x, int y, int health, int mana, int experience, int level);

//添加敌人函数
Enemy* CreateEnemy(int type, int x, int y);

//添加BOSS函数
Enemy* CreateBoss(int type, int x, int y);

//添加砖块函数
Block* CreateBlock(int type, int x, int y);

//添加场景道具函数
Item* CreateItem(int type, int x, int y);

//添加消息
Notice* CreateNotice(HWND hWnd, int type);

// 初始化场景函数
void InitStage(HWND hWnd, int stageID);

// 刷新单位状态函数
void UpdateUnits(HWND hWnd);

// 刷新敌人状态函数
void UpdateEnemies(HWND hWnd);

//加载敌人函数
void LoadEnemies(int stage);

//加载砖块函数
void LoadBlocks(int stage);

//加载场景道具函数
void LoadItems(int stage);

//清除所有单位函数
void ClearAll(HWND hWnd);

//重置全局变量值函数
void ResetGlobal();

#pragma endregion 


#pragma region 主角行为函数

//基础行为函数
void UnitBehaviour_Basic(HWND hWnd, Unit* unit);

//横向碰撞处理函数
void AvoidGoFarther(Unit* unit);

//攻击行为函数
void AttackBehaviour(HWND hWnd, bool is_end);

//S技能行为函数
void AbilityBehaviour_1(HWND hWnd, bool is_end);

//D技能行为函数
void AbilityBehaviour_2(HWND hWnd, bool is_end);

//F技能行为函数
void AbilityBehaviour_3(HWND hWnd, bool is_end);

//形态切换行为函数
void SwitchTypeBehaviour(HWND hWnd, int switch_direction);

//跳跃行为函数
void JumpBehaviour();

//下落行为函数
void FallBehaviour();
//落地行为函数
void LandBehaviour();

//拾取道具函数
void GetItemBehaviour(HWND hWnd, Unit* unit);

//升级行为函数
void UpgradeBehaviour(HWND hWnd, Unit* unit);

//死亡行为函数
void DieBehaviour(HWND hWnd);

//关卡传送函数
void ChangeStage(HWND hWnd);

//TODO: 添加游戏需要的更多函数

#pragma endregion


#pragma region 其他行为函数

//敌人行为函数
void EnemyBehaviour(HWND hWnd, Enemy* enemy);

//敌人攻击函数
void EnemyAttack(HWND hWnd, WPARAM wParam);

//敌人解除控制状态函数
void ReliefShocked(HWND hWnd);

//攻击伤害结算函数
void Injuring(HWND hWnd, Unit* unit, Enemy* enemy, int damage);

//被攻击伤害结算函数
void Injured(HWND hWnd, Unit* unit, int damage);

//补充最终关卡的MP道具函数
void MPSupply(HWND hWnd);

#pragma endregion


#pragma region 绘图函数声明

// 绘图函数
void Paint(HWND hWnd);

// 初始化单个背景图函数
HBITMAP InitSingleBackGround(HWND hWnd, HBITMAP bmp_src);

// 初始化随机生成背景函数
HBITMAP InitBackGround(HWND hWnd, HBITMAP bmp_src);

//发布掉落消息函数
void DropNotice(HWND hWnd, int exp, int hp,int ma);

//发布升级消息函数
void UpgradeNotice(HWND hWnd, int new_level);

//发布法力值不足消息函数
void ManaLimitAttention(HWND hWnd);

//结束消息显示函数
void EndNotice(HWND hWnd);

#pragma endregion

#pragma region 其他具体执行交互的函数

//方向处理函数
void HaddleDirection(bool value, int direction);

//选项指针切换函数
void SwitchOption(HWND hWnd, int direction);

//选项确定函数
void ConfirmOption(HWND hWnd);

//返回开始界面函数
void BackToStartMenu(HWND hWnd);

//退出“假帮助页面”函数
void LeaveFakeHelp(HWND hWnd);

//暂停游戏函数
void PauseGame(HWND hWnd);

#pragma endregion