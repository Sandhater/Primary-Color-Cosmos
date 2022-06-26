// ContraGame.cpp : 定义应用程序的入口点。
//

#include "ContraGame.h"

using namespace std;


#pragma region 全局变量

#define MAX_LOADSTRING			100		

// 全局变量: 
HINSTANCE hInst; // 当前窗体实例
WCHAR szTitle[MAX_LOADSTRING]; // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING]; // 主窗口类名

HBITMAP bmp_Indicator;	//选项指针图像资源
HBITMAP bmp_Introduce;	//故事背景介绍图像资源
HBITMAP bmp_StartBackground;	//开始界面背景图像资源
HBITMAP bmp_HelpBackground;		//帮助界面背景图像资源
HBITMAP bmp_PauseBackground;	//暂停界面背景图像资源
HBITMAP bmp_FailBackground;		//失败界面背景图像资源
HBITMAP bmp_VictoryBackground;	//胜利界面背景图像资源
HBITMAP bmp_Background1;		//关卡1背景图像资源
HBITMAP bmp_Background2;		//关卡2背景图像资源
HBITMAP bmp_Background3;		//关卡3背景图像资源
HBITMAP bmp_StartButton;	//开始按钮图像资源
HBITMAP bmp_HelpButton;		//帮助按钮图像资源
HBITMAP bmp_ExitButton;		//退出按钮图像资源
HBITMAP bmp_ContinueButton;	//继续游戏按钮图像资源
HBITMAP bmp_HomeButton;		//返回主页按钮图像资源
HBITMAP bmp_BackButton;		//返回开始页面按钮图像资源
HBITMAP bmp_Hero;			//主角图像资源
HBITMAP bmp_Enemy;			//敌人图像资源
HBITMAP bmp_Boss;			//BOSS图像资源
HBITMAP bmp_Block;			//砖块图像资源
HBITMAP bmp_Container;		//血量或法力槽图像资源
HBITMAP bmp_Health;			//血量图像资源
HBITMAP bmp_Mana;			//法力图像资源
HBITMAP bmp_Item;			//场景道具图像资源
HBITMAP bmp_Notice;			//消息图像资源

HBITMAP bmp_Background;		//生成的背景图像



Stage* currentStage = NULL; //当前场景状态
vector<Unit*> units;		//单位
vector<Button*> buttons;	//按钮
Indicator* indicator;	//选项指针
vector<Block*> blocks;		//砖块
vector<Enemy*> enemies;		//敌人
vector<Notice*> notices;	//消息
vector<Item*> items;		//场景道具

queue<Enemy*> injure_list;	//等待结算的伤害列表
stack<int> enemy_attack_timer_ids = stack<int>();	//用一个栈来为敌人攻击时所用的计时器id赋值

//全局变量
int mouseX = 0;
int mouseY = 0;
bool mouseDown = false;
bool keyUpDown = false;
bool keyDownDown = false;
bool keyLeftDown = false;
bool keyRightDown = false;
bool keyActionDown = false;		//攻击键和技能键按下后是否已经抬起
bool beingBusy = false;		//主角是否操作繁忙而无法立马进行其他操作
bool beingJump = false;		//主角是否正在跳跃
bool beingFall = true;		//主角是否正在下落
bool beingDash = false;		//主角是否正在冲刺（蓝色形态D技能）
bool isLastJumpOver = true;		//上一次跳跃是否已经结束
bool isNoticeOld = true;	//消息是否已经陈旧而等待清除
bool isPause = false;	//游戏是否暂停
bool isFakeHelp = false;	//是否处于虚假帮助页面（即从暂停页面进入帮助页面时，为改变stageID）
bool isFromLastStage = true;	//主角是从上一关卡进入此关卡，还是从下一关卡返回来的

int _health = 100;	//将主角数据变成全局变量
int _mana = 100;
int _level = 1;		//等级进度
int _experience = 0;	//经验进度
int _stage = STAGE_1;	//关卡进度
int _type = UNIT_TYPE_RED;	//形态

//帧
int FRAMES_HOLD[] = { 0 };
int FRAMES_HOLD_COUNT = 1;
int FRAMES_WALK[] = { 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2 };
int FRAMES_WALK_COUNT = 16;
int FRAMES_ATTACK[] = { 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 5, 5, 5, 5, 4, 4, 4, 4, 4, 3, 3, 3, 3, 3 };
int FRAMES_ATTACK_COUNT = 24;
int FRAMES_S[] = { 6, 6, 6, 6, 6, 6, 7, 7, 7, 7, 7, 7, 8, 8, 8, 8, 8, 8, 0, 0, 0, 0, 0, 0 };
int FRAMES_S_COUNT = 24;
int FRAMES_D[] = { 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9 };
int FRAMES_D_COUNT = 24;
int FRAMES_F[] = { 10, 10, 10, 10, 10, 10, 10, 10, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 10, 10, 10, 10, 10, 10 };
int FRAMES_F_COUNT = 24;

//当前选项指针位置
int INDICATOR_YINDEX = 0;

// TODO: 在此添加其它全局变量





double const PI = acos(double(-1));


#pragma endregion


#pragma region Win32程序框架



// 此代码模块中包含的函数的前向声明: 
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);




int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// 初始化全局字符串
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_CONTRAGAME, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 执行应用程序初始化: 
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CONTRAGAME));

	MSG msg;

	// 主消息循环: 
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_CONTRAGAME);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON1));

	return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释: 
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // 将实例句柄存储在全局变量中

	HWND hWnd = CreateWindow(szWindowClass, szTitle,
		~WS_HSCROLL & ~WS_VSCROLL & WS_POPUP & ~WS_BORDER, 
		// 上行参数设置窗口样式，不可改变大小，默认全屏，无滚动条
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		WINDOW_WIDTH,
		WINDOW_HEIGHT + WINDOW_TITLEBARHEIGHT,
		nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的:    处理主窗口的消息。
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
		// 初始化游戏窗体
		InitGame(hWnd, wParam, lParam);
		break;
	case WM_KEYDOWN:
		// 键盘按下事件
		KeyDown(hWnd, wParam, lParam);
		break;
	case WM_KEYUP:
		// 键盘松开事件
		KeyUp(hWnd, wParam, lParam);
		break;
	case WM_MOUSEMOVE:
		// 鼠标移动事件
		MouseMove(hWnd, wParam, lParam);
		break;
	case WM_LBUTTONDOWN:  
		// 鼠标左键按下事件
		LButtonDown(hWnd, wParam, lParam);
		break;
	case WM_LBUTTONUP:
		// 鼠标左键松开事件
		LButtonUp(hWnd, wParam, lParam);
		break;
	case WM_TIMER:
		// 定时器事件
		switch (wParam)
		{
		case TIMER_GAMETIMER:
			if (currentStage != NULL && currentStage->timerOn) TimerUpdate(hWnd, wParam, lParam);
			break;
		case TIMER_ATTACK:
			if (currentStage != NULL && currentStage->timerOn)
				AttackBehaviour(hWnd, true);
			break;
		case TIMER_S:
			if (currentStage != NULL && currentStage->timerOn)
				AbilityBehaviour_1(hWnd, true);
			break;
		case TIMER_D:
			if (currentStage != NULL && currentStage->timerOn)
				AbilityBehaviour_2(hWnd, true);
			break;
		case TIMER_F:
			if (currentStage != NULL && currentStage->timerOn)
				AbilityBehaviour_3(hWnd, true);
			break;
		case TIMER_SHOCKED:
			if (currentStage != NULL && currentStage->timerOn)
				ReliefShocked(hWnd);
			break;
		case TIMER_NOTICE:
			if (currentStage != NULL && currentStage->timerOn)
				EndNotice(hWnd);
			break;
		case TIMER_ITEM:
			if (currentStage != NULL && currentStage->timerOn && currentStage->itemTimerOn)
				MPSupply(hWnd);
			break;
		case TIMER_ENEMY_ATTACK:
			if (currentStage != NULL && currentStage->timerOn)
				EnemyAttack(hWnd, wParam);
			break;
		case TIMER_INTRODUCE:
			InitStage(hWnd, STAGE_STARTMENU);
			KillTimer(hWnd, TIMER_INTRODUCE);
			break;
		}
	case WM_PAINT:
		// 绘图
		Paint(hWnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}


#pragma endregion


#pragma region 事件处理函数

// 初始化游戏窗体函数
void InitGame(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	srand((unsigned int)time(NULL));

	//加载图像资源
	bmp_Introduce = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_INTRODUCE));
	bmp_StartBackground = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_START_BACKGROUND));
	bmp_HelpBackground= LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_HELP_BACKGROUND));
	bmp_PauseBackground = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_PAUSE_BACKGROUND));
	bmp_FailBackground = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_FAIL_BACKGROUND));
	bmp_VictoryBackground = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_VICTORY_BACKGROUND));
	bmp_Background1 = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_BACKGROUND1));
	bmp_Background2 = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_BACKGROUND2));
	bmp_Background3 = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_BACKGROUND3));

	bmp_Indicator = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_INDICATOR));
	bmp_StartButton = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_START));
	bmp_HelpButton = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_HELP));
	bmp_ExitButton = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_EXIT));
	bmp_ContinueButton = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_CONTINUE));
	bmp_HomeButton = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_HOME));
	bmp_BackButton = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_BACK));

	bmp_Hero = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_HERO));
	bmp_Enemy = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_ENEMY));
	bmp_Boss = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_BOSS));
	bmp_Block = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_BLOCK));

	bmp_Container = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_CONTAINER));
	bmp_Health = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_HEALTH));
	bmp_Mana = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_MANA));
	bmp_Item = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_ITEM));
	bmp_Notice = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_NOTICE));

	//加载音频资源
	mciSendString(_T("OPEN res/Walk.wav type MPEGvideo ALIAS Walk"), NULL, 0, 0);

	//添加按钮
	indicator = CreateIndicator(bmp_Indicator, INDICATOR_WIDTH, INDICATOR_HEIGHT, WINDOW_WIDTH / 5 - 1.5 * INDICATOR_WIDTH, INDICATOR_Y[INDICATOR_YINDEX]);
	Button* startButton = CreateButton(BUTTON_STARTGAME, bmp_StartButton, BUTTON_STARTGAME_WIDTH, BUTTON_STARTGAME_HEIGHT,
		WINDOW_WIDTH / 5, WINDOW_HEIGHT / 2.55);
	buttons.push_back(startButton);
	Button* helpButton = CreateButton(BUTTON_HELP, bmp_HelpButton, BUTTON_HELP_WIDTH, BUTTON_HELP_HEIGHT,
		WINDOW_WIDTH / 5, WINDOW_HEIGHT / 2.55 + 2 * BUTTON_HELP_HEIGHT);
	buttons.push_back(helpButton);
	Button* exitButton = CreateButton(BUTTON_EXITGAME, bmp_ExitButton, BUTTON_EXITGAME_WIDTH, BUTTON_EXITGAME_HEIGHT,
		WINDOW_WIDTH / 5, WINDOW_HEIGHT / 2.55 + 4 * BUTTON_EXITGAME_HEIGHT);
	buttons.push_back(exitButton);
	Button* continueButton = CreateButton(BUTTON_CONTINUE, bmp_ContinueButton, BUTTON_CONTINUE_WIDTH, BUTTON_CONTINUE_HEIGHT,
		WINDOW_WIDTH / 5, WINDOW_HEIGHT / 2.55);
	buttons.push_back(continueButton);
	Button* homeButton = CreateButton(BUTTON_HOME, bmp_HomeButton, BUTTON_HOME_WIDTH, BUTTON_HOME_HEIGHT,
		WINDOW_WIDTH / 5, WINDOW_HEIGHT / 2.55 + 4 * BUTTON_HOME_HEIGHT);
	buttons.push_back(homeButton);
	Button* backButton = CreateButton(BUTTON_BACK, bmp_BackButton, BUTTON_BACK_WIDTH, BUTTON_BACK_HEIGHT,
		WINDOW_WIDTH / 20, WINDOW_HEIGHT / 13.5);
	buttons.push_back(backButton);

	//初始化开始场景
	InitStage(hWnd, STAGE_INTRODUCE);

	//初始化主计时器
	SetTimer(hWnd, TIMER_GAMETIMER, TIMER_GAMETIMER_ELAPSE, NULL);
	//初始化道具补充计时器
	SetTimer(hWnd, TIMER_ITEM, TIMER_ITEM_ELAPSE, NULL);
}

// 键盘按下事件处理函数
void KeyDown(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case VK_UP:
		if (currentStage != NULL && currentStage->timerOn)
			JumpBehaviour();
		break;
	case VK_DOWN:
		break;
	case VK_LEFT:
		HaddleDirection(true, 0);
		break;
	case VK_RIGHT:
		HaddleDirection(true, 1);
		break;
	case VK_ESCAPE:
		switch (currentStage->stageID)
		{
		case STAGE_STARTMENU:
			break;
		case STAGE_HELP: case STAGE_FAIL: case STAGE_VICTORY:
			BackToStartMenu(hWnd);
			break;
		default:
			if(!isPause&&currentStage->timerOn)
				PauseGame(hWnd);
			else if(isFakeHelp)
			{
				LeaveFakeHelp(hWnd);
			}
			break;
		}
		break;
	case VK_A:
		if (currentStage != NULL && currentStage->timerOn)
			AttackBehaviour(hWnd, false);
		break;
	case VK_S:
		if (currentStage != NULL && currentStage->timerOn)
			AbilityBehaviour_1(hWnd, false);
		break;
	case VK_D:
		if (currentStage != NULL && currentStage->timerOn)
			AbilityBehaviour_2(hWnd, false);
		break;
	case VK_F:
		if (currentStage != NULL && currentStage->timerOn)
			AbilityBehaviour_3(hWnd, false);
		break;
	case VK_Q:
		if (currentStage != NULL && currentStage->timerOn)
			SwitchTypeBehaviour(hWnd, 2);
		break;
	case VK_R:
		if (currentStage != NULL && currentStage->timerOn)
			SwitchTypeBehaviour(hWnd, 1);
		break;
	default:
		break;
	}
}

// 键盘松开事件处理函数
void KeyUp(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	// TODO
	switch (wParam)
	{
	case VK_UP:
		if(currentStage->stageID>=STAGE_1)
			isLastJumpOver = true;
		SwitchOption(hWnd, 2);
		
		break;
	case VK_DOWN:
		SwitchOption(hWnd, 1);
		
		break;
	case VK_LEFT:
		HaddleDirection(false, 0);
		break;
	case VK_RIGHT:
		HaddleDirection(false, 1);
		break;
	case VK_RETURN:
		ConfirmOption(hWnd);
		break;
	case VK_A: case VK_S: case VK_D: case VK_F:
		if (currentStage != NULL && currentStage->timerOn)
			keyActionDown = false;
		break;
	case VK_SPACE:
		if (currentStage != NULL && currentStage->timerOn)
			ChangeStage(hWnd);
		break;
	default:
		break;
	}
}

// 鼠标移动事件处理函数
void MouseMove(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	mouseX = LOWORD(lParam);
	mouseY = HIWORD(lParam);
}

// 鼠标左键按下事件处理函数
void LButtonDown(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	mouseX = LOWORD(lParam);
	mouseY = HIWORD(lParam);
	mouseDown = true;

	for (int i = 0; i < buttons.size(); i++)
	{
		Button* button = buttons[i];
		if (button->visible)
		{
			if (button->x <= mouseX
				&& button->x + button->width >= mouseX
				&& button->y <= mouseY
				&& button->y + button->height >= mouseY)
			{
				switch (button->buttonID) {
				case BUTTON_STARTGAME:
					//TODO：判断进入哪个关卡
					InitStage(hWnd, _stage);
					break;
				case BUTTON_HELP:
					if (currentStage->stageID == STAGE_STARTMENU)
					{
						bmp_Background = InitSingleBackGround(hWnd, bmp_HelpBackground);
						InitStage(hWnd, STAGE_HELP);
					}
					else if (isPause && !currentStage->timerOn)
					{
						indicator->visible = false;
						for (int i = 0; i < buttons.size(); i++)
						{
							Button* button = buttons[i];
							if (button->buttonID == BUTTON_BACK)
								button->visible = true;
							else
								button->visible = false;
						}
						isFakeHelp = true;
						InvalidateRect(hWnd, NULL, FALSE);
						break;
					}
					break;
				case BUTTON_EXITGAME:
					PostQuitMessage(0);
					break;
				case BUTTON_CONTINUE:
					if (isPause && !currentStage->timerOn)
					{
						isPause = false;
						indicator->visible = false;
						currentStage->timerOn = true;
						currentStage->itemTimerOn = true;
						for (int i = 0; i < buttons.size(); i++)
							buttons[i]->visible = false;
						break;
					}
				case BUTTON_HOME:
					BackToStartMenu(hWnd);
					break;
				case BUTTON_BACK:
					if (currentStage->stageID == STAGE_HELP || currentStage->stageID == STAGE_FAIL|| currentStage->stageID == STAGE_VICTORY)
					{
						ClearAll(hWnd);
						InitStage(hWnd, STAGE_STARTMENU);
					}
					else if (isFakeHelp)
						LeaveFakeHelp(hWnd);
					break;
				}
			}
		}
	}

}

// 鼠标左键松开事件处理函数
void LButtonUp(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	mouseX = LOWORD(lParam);
	mouseY = HIWORD(lParam);
	mouseDown = false;
}

// 定时器事件处理函数
void TimerUpdate(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UpdateUnits(hWnd);
	UpdateEnemies(hWnd);
	//刷新显示
	InvalidateRect(hWnd, NULL, FALSE);
}


#pragma endregion


#pragma region 其它游戏状态处理函数



//TODO: 添加游戏需要的更多函数

//添加选项指针函数
Indicator* CreateIndicator(HBITMAP img, int width, int height, int x, int y)
{
	Indicator* indicator = new Indicator();
	indicator->img = img;
	indicator->x = x;
	indicator->y = y;
	indicator->height = height;
	indicator->width = width;

	indicator->visible = false;
	return indicator;
}

// 添加按钮函数
Button* CreateButton(int buttonID, HBITMAP img, int width, int height, int x, int y)
{
	Button* button = new Button();
	button->buttonID = buttonID;
	button->img = img;
	button->width = width;
	button->height = height;
	button->x = x;
	button->y = y;

	button->visible = false;
	return button;
}

// 添加主角函数
Unit* CreateUnit(int type, int x, int y, int health, int mana, int experience, int level)
{
	Unit* unit = new Unit();
	unit->img = bmp_Hero;
	unit->direction = UNIT_DIRECT_RIGHT;
	unit->type = type;
	unit->state = UNIT_STATE_HOLD;

	unit->frame_row = type;
	unit->frame_column = UNIT_LAST_FRAME * unit->direction;

	unit->frame_sequence = FRAMES_HOLD;
	unit->frame_count = FRAMES_HOLD_COUNT;
	unit->frame_id = 0;

	unit->x = x;
	unit->y = y;
	unit->vx = 0;
	unit->vy = 0;

	unit->experience = experience;
	unit->level = level;
	unit->health = health;
	unit->mana = mana;
	unit->defence = 1 + defence_mods[type];
	unit->jump_height = 0;
	unit->jump_times = 0;

	_health = unit->health;
	_mana = unit->mana;
	return unit;
}

//添加敌人函数
Enemy* CreateEnemy(int type, int x, int y)
{
	Enemy* enemy = new Enemy();
	enemy->img = bmp_Enemy;
	enemy->direction = ENEMY_DIRECT_RIGHT;
	enemy->type = type;
	enemy->state = ENEMY_STATE_WALK;

	enemy->frame_row = type;
	enemy->frame_column = ENEMY_LAST_FRAME * enemy->direction;

	enemy->frame_sequence = FRAMES_WALK;
	enemy->frame_count = FRAMES_WALK_COUNT;
	enemy->frame_id = 0;

	enemy->x = x;
	enemy->y = y;
	enemy->vx = 0;
	enemy->vy = 0;

	enemy->exp = enemy_exeperiences[type];
	enemy->health = enemy_healths[type];
	enemy->ma = enemy_manas[type];
	enemy->defence = enemy_defences[type];
	enemy->offset = 0;
	return enemy;
}

//添加BOSS函数
Enemy* CreateBoss(int type, int x, int y)
{
	Enemy* enemy = new Enemy();
	enemy->img = bmp_Boss;
	enemy->direction = ENEMY_DIRECT_RIGHT;
	enemy->type = type;
	enemy->state = ENEMY_STATE_WALK;

	enemy->frame_row = 0;
	enemy->frame_column = ENEMY_LAST_FRAME * enemy->direction;

	enemy->frame_sequence = FRAMES_WALK;
	enemy->frame_count = FRAMES_WALK_COUNT;
	enemy->frame_id = 0;

	enemy->x = x;
	enemy->y = y;
	enemy->vx = 0;
	enemy->vy = 0;

	enemy->exp = enemy_exeperiences[type];
	enemy->health = enemy_healths[type];
	enemy->ma = enemy_manas[type];
	enemy->defence = enemy_defences[type];
	enemy->offset = 0;
	return enemy;
}

//添加场景道具函数
Item* CreateItem(int type, int x, int y)
{
	Item* item = new Item();
	item->img = bmp_Item;
	item->type = type;
	item->x = x;
	item->y = y;
	return item;
}

//添加砖块函数
Block* CreateBlock(int type, int x, int y)
{
	Block* block = new Block();

	block->img = bmp_Block;
	block->type = type;
	block->x = x;
	block->y = y;
	block->visible = true;
	return block;
}

//添加消息
Notice* CreateNotice(HWND hWnd, int type)
{
	if (notices.size() > 9)		//如果消息太多，需要先清空
	{
		for (int i = 0; i < notices.size(); i++)
			notices[i] = NULL;
		notices.clear();
		KillTimer(hWnd, TIMER_NOTICE);
	}
	Notice* notice = new Notice();
	notice->img = bmp_Notice;
	notice->type = type;
	return notice;
}

// 初始化游戏场景函数
void InitStage(HWND hWnd, int stageID)
{
	// 初始化场景实例
	if (currentStage != NULL) delete currentStage;
	currentStage = new Stage();
	currentStage->stageID = stageID;

	if (stageID == STAGE_INTRODUCE)
	{
		bmp_Background = InitSingleBackGround(hWnd, bmp_Introduce);
		currentStage->bg = bmp_Background;
		currentStage->timeCountDown = 0;
		currentStage->timerOn = false;
		currentStage->itemTimerOn = true;
		SetTimer(hWnd, TIMER_INTRODUCE, TIMER_INTRODUCE_ELAPSE, NULL);

		mciSendString(_T("CLOSE Victory"), NULL, 0, NULL);
		mciSendString(_T("OPEN res/Background_Music.wav type MPEGVideo ALIAS Music0"), NULL, 0, 0);
		mciSendString(_T("PLAY Music0 repeat"), NULL, 0, NULL);
	}

	if (stageID == STAGE_STARTMENU) {
		units.clear();
		bmp_Background = InitSingleBackGround(hWnd, bmp_StartBackground);
		currentStage->bg = bmp_Background;
		currentStage->timeCountDown = 0;
		currentStage->timerOn = false;
		currentStage->itemTimerOn = true;
		ResetGlobal();

		//显示开始界面的按钮
		indicator->visible = true;
		for (int i = 0; i < buttons.size(); i++)
		{
			Button* button = buttons[i];
			if (button->buttonID >= BUTTON_STARTGAME && button->buttonID <= BUTTON_EXITGAME)
			{
				button->visible = true;
			}
			else
			{
				button->visible = false;
			}
		}
	}

	else if (stageID <= STAGE_HELP && stageID>=STAGE_VICTORY)
	{
		currentStage->bg = bmp_Background;
		currentStage->timeCountDown = 0;
		currentStage->timerOn = false;
		currentStage->itemTimerOn = false;

		//隐藏开始界面的按钮
		indicator->visible = false;
		for (int i = 0; i < buttons.size(); i++)
		{
			Button* button = buttons[i];
			if (button->buttonID == BUTTON_BACK)
			{
				button->visible = true;
			}
			else
			{
				button->visible = false;
			}
		}

		if (stageID == STAGE_VICTORY)
		{
			mciSendString(_T("STOP Muisc0"), NULL, 0, NULL);
			mciSendString(_T("CLOSE Muisc0"), NULL, 0, NULL);
			mciSendString(_T("OPEN res/Victory.wav ALIAS Victory"), NULL, 0, 0);
			mciSendString(_T("PLAY Victory"), NULL, 0, NULL);
		}
	}

	else if (stageID >= STAGE_1 && stageID <= STAGE_3) //TODO：添加多个游戏场景
	{
		switch (stageID)
		{
		case STAGE_1:
			bmp_Background = InitSingleBackGround(hWnd, bmp_Background1);
			break;
		case STAGE_2:
			bmp_Background = InitSingleBackGround(hWnd, bmp_Background2);
			break;
		case STAGE_3:
			bmp_Background = InitSingleBackGround(hWnd, bmp_Background3);
			break;
		}
		currentStage->bg = bmp_Background;
		currentStage->timeCountDown = 10000;
		currentStage->timerOn = true;
		if (stageID == STAGE_3)
			currentStage->itemTimerOn = true;
		else
			currentStage->itemTimerOn = false;
		for (int i = 0; i < enemy_attack_timer_ids.size(); i++)
			enemy_attack_timer_ids.pop();
		for (int i = 0; i < 15; i++)
			enemy_attack_timer_ids.push(i);

		//隐藏的按钮
		indicator->visible = false;
		for (int i = 0; i < buttons.size(); i++)
		{
			Button* button = buttons[i];
				button->visible = false;
		}

		// 按场景初始化单位
		int _x, _y;
		if (isFromLastStage)
		{
			_x = GATE1_LEFT;
			_y = GATE1_UP;
		}
		else
		{
			_x = GATE2_LEFT;
			_y = GATE2_UP;
		}
		units.insert(units.begin(), CreateUnit(_type, _x, _y, 100, 100, _experience, _level));
		LoadBlocks(stageID);
		LoadEnemies(stageID);
		LoadItems(stageID);
	}

	//刷新显示
	InvalidateRect(hWnd, NULL, FALSE);
}


// 刷新主角状态函数
void UpdateUnits(HWND hWnd)
{
	if (units.size() == 0)
		return;
	Unit* unit = units[0];
	//根据单位类型选择行为函数
	switch (unit->type) {
	default:
		UnitBehaviour_Basic(hWnd, unit);
		//unit->mana += UNIT_MANA_RECOVER;
		//_mana = unit->mana;
		break;
	}
}

// 刷新敌人状态函数
void UpdateEnemies(HWND hWnd)
{
	for (int i = 0; i < enemies.size(); i++)
	{
		Enemy* enemy = enemies[i];
		//根据敌人类型选择行为函数
		switch (enemy->type) {
		default:
			EnemyBehaviour(hWnd, enemy);
			break;
		}
	}
}

//加载敌人函数
void LoadEnemies(int stage)
{
	switch (stage)
	{
	case STAGE_1:
		enemies.push_back(CreateEnemy(ENEMY_TYPE_A, 400, 940));
		enemies.push_back(CreateEnemy(ENEMY_TYPE_B, 1300, 780));
		enemies.push_back(CreateEnemy(ENEMY_TYPE_C, 60, 460));
		enemies.push_back(CreateEnemy(ENEMY_TYPE_A, 1080, 940));
		enemies.push_back(CreateEnemy(ENEMY_TYPE_A, 720, 300));
		enemies.push_back(CreateEnemy(ENEMY_TYPE_B, 1200, 300));
		//Enemy* boss = CreateBoss(BOSS, 600, 900);
		//enemies.push_back(boss);
		break;
	case STAGE_2:
		enemies.push_back(CreateEnemy(ENEMY_TYPE_B, 180, 940));
		enemies.push_back(CreateEnemy(ENEMY_TYPE_C, 1840, 780));
		enemies.push_back(CreateEnemy(ENEMY_TYPE_A, 240, 540));
		enemies.push_back(CreateEnemy(ENEMY_TYPE_B, 400, 540));
		enemies.push_back(CreateEnemy(ENEMY_TYPE_A, 560, 540));
		enemies.push_back(CreateEnemy(ENEMY_TYPE_A, 800, 540));
		enemies.push_back(CreateEnemy(ENEMY_TYPE_A, 720, 300));
		enemies.push_back(CreateEnemy(ENEMY_TYPE_B, 1400, 300));
		enemies.push_back(CreateEnemy(ENEMY_TYPE_C, 1720, 300));
		break;
	case STAGE_3:
		enemies.push_back(CreateEnemy(ENEMY_TYPE_A, 360, 940));
		enemies.push_back(CreateEnemy(ENEMY_TYPE_A, 1360, 940));
		enemies.push_back(CreateEnemy(ENEMY_TYPE_B, 840, 940));
		enemies.push_back(CreateBoss(BOSS, 900, 900));
		break;
	}
}

//加载砖块函数
void LoadBlocks(int stage)
{
	switch (stage)
	{
	case STAGE_1:
		for (int i = 0; i < BLOCK_ROW_COUNT - 4; i++)
		{
			Block* block = CreateBlock(BLOCK_SOLID, BLOCK_WIDTH * i, BLOCK_HEIGHT * 25);
			blocks.push_back(block);
			block = CreateBlock(BLOCK_SOLID, BLOCK_WIDTH * i, BLOCK_HEIGHT * 26);
			blocks.push_back(block);
		}
		for (int i = BLOCK_ROW_COUNT - 7; i < BLOCK_ROW_COUNT; i++)
		{
			if (i > BLOCK_ROW_COUNT - 3)
			{
				Block* block = CreateBlock(BLOCK_SAND, BLOCK_WIDTH * i, BLOCK_HEIGHT * 21);
				blocks.push_back(block);
			}
			else
			{
				Block* block = CreateBlock(BLOCK_SOLID, BLOCK_WIDTH * i, BLOCK_HEIGHT * 21);
				blocks.push_back(block);
			}
		}
		for (int i = 3; i < 8; i++)
		{
			if (i == 5 || i == 6)
			{
				Block* block = CreateBlock(BLOCK_THORN, BLOCK_WIDTH * i, BLOCK_HEIGHT * 17);
				blocks.push_back(block);
			}
			else
			{
				Block* block = CreateBlock(BLOCK_SOLID, BLOCK_WIDTH * i, BLOCK_HEIGHT * 17);
				blocks.push_back(block);
			}
		}
		for (int i = 0; i < BLOCK_ROW_COUNT; i++)
		{
			if (i % 4 < 2)
			{
				Block* block = CreateBlock(BLOCK_SOLID, BLOCK_WIDTH * i, BLOCK_HEIGHT * 13);
				blocks.push_back(block);
			}
		}
		for (int i = 3; i < BLOCK_ROW_COUNT; i++)
		{
			if (i > 10 && i < 14)
			{
				Block* block = CreateBlock(BLOCK_THORN, BLOCK_WIDTH * i, BLOCK_HEIGHT * 9);
				blocks.push_back(block);
			}
			else
			{
				Block* block = CreateBlock(BLOCK_SOLID, BLOCK_WIDTH * i, BLOCK_HEIGHT * 9);
				blocks.push_back(block);
			}

		}
		for (int i = 3; i < BLOCK_ROW_COUNT; i += 3)
		{
			Block* block = CreateBlock(BLOCK_SOLID, BLOCK_WIDTH * i, BLOCK_HEIGHT * 5);
			blocks.push_back(block);
		}
		break;
	case STAGE_2:
		for (int i = 0; i < 4; i++)
		{
			blocks.push_back(CreateBlock(BLOCK_SOLID, BLOCK_WIDTH * i, BLOCK_HEIGHT * 25));
			blocks.push_back(CreateBlock(BLOCK_SOLID, BLOCK_WIDTH * i, BLOCK_HEIGHT * 26));
		}
		for (int i = 22; i < BLOCK_COL_COUNT; i++)
			blocks.push_back(CreateBlock(BLOCK_SOLID, BLOCK_WIDTH * 4, BLOCK_HEIGHT * i));
		for (int i = 5; i < BLOCK_ROW_COUNT; i++)
			blocks.push_back(CreateBlock(BLOCK_THORN, BLOCK_WIDTH * i, BLOCK_HEIGHT * 26));
		for (int i = 4; i < BLOCK_ROW_COUNT; i++)
		{
			if (i == 4 || i == BLOCK_ROW_COUNT - 1)
				blocks.push_back(CreateBlock(BLOCK_SOLID, BLOCK_WIDTH * i, BLOCK_HEIGHT * 21));
			else
				blocks.push_back(CreateBlock(BLOCK_SAND, BLOCK_WIDTH * i, BLOCK_HEIGHT * 21));
		}
		for(int i=0;i<BLOCK_ROW_COUNT-3;i++)
			blocks.push_back(CreateBlock(BLOCK_SOLID, BLOCK_WIDTH * i, BLOCK_HEIGHT * 15));
		for (int i = 2; i < BLOCK_ROW_COUNT - 4; i += 4)
		{
			blocks.push_back(CreateBlock(BLOCK_THORN, BLOCK_WIDTH * i, BLOCK_HEIGHT * 14));
			blocks.push_back(CreateBlock(BLOCK_THORN, BLOCK_WIDTH * (i + 1), BLOCK_HEIGHT * 14));
		}
		for (int i = 3; i < BLOCK_ROW_COUNT; i++)
		{
			if (i % 4 == 0 || i % 4 == 1)
				blocks.push_back(CreateBlock(BLOCK_SAND, BLOCK_WIDTH * i, BLOCK_HEIGHT * 9));
			else
				blocks.push_back(CreateBlock(BLOCK_SOLID, BLOCK_WIDTH * i, BLOCK_HEIGHT * 9));
		}
		for (int i = 0; i < 2; i++)
		{
			blocks.push_back(CreateBlock(BLOCK_SOLID, BLOCK_WIDTH * i, BLOCK_HEIGHT * 5));
			blocks.push_back(CreateBlock(BLOCK_SOLID, BLOCK_WIDTH * (BLOCK_ROW_COUNT - i - 1), BLOCK_HEIGHT * 5));
		}
		break;
	case STAGE_3:
		for (int i = 0; i < BLOCK_ROW_COUNT - 2; i++)
		{
			blocks.push_back(CreateBlock(BLOCK_SOLID, BLOCK_WIDTH * i, BLOCK_HEIGHT * 26));
			if(i%4==1)
				blocks.push_back(CreateBlock(BLOCK_THORN, BLOCK_WIDTH * i, BLOCK_HEIGHT * 25));
			else
				blocks.push_back(CreateBlock(BLOCK_SOLID, BLOCK_WIDTH* i, BLOCK_HEIGHT * 25));
		}
		for (int i = 0; i < 3; i++)
		{
			blocks.push_back(CreateBlock(BLOCK_SOLID, BLOCK_WIDTH* i, BLOCK_HEIGHT * 20));
			blocks.push_back(CreateBlock(BLOCK_SOLID, BLOCK_WIDTH* (BLOCK_ROW_COUNT - i), BLOCK_HEIGHT * 20));
		}
		for (int i = 7; i < 10; i++)
			blocks.push_back(CreateBlock(BLOCK_SOLID, BLOCK_WIDTH* i, BLOCK_HEIGHT * 14));
		break;
	}
}

//加载场景道具函数
void LoadItems(int stage)
{
	switch (stage)
	{
	case STAGE_1:
		items.push_back(CreateItem(ITEM_HP, 1800, 640));
		items.push_back(CreateItem(ITEM_HP, 200, 720));
		items.push_back(CreateItem(ITEM_HP, 1600, 60));
		items.push_back(CreateItem(ITEM_MP, 160, 120));
		items.push_back(CreateItem(ITEM_EXP, 1300, 440));
		break;
	case STAGE_2:
		items.push_back(CreateItem(ITEM_EXP, 300, 760));
		items.push_back(CreateItem(ITEM_MP, 1080, 660));
		items.push_back(CreateItem(ITEM_EXP, 1760, 440));
		items.push_back(CreateItem(ITEM_HP, 20, 120));
		items.push_back(CreateItem(ITEM_MP, 900, 60));
		items.push_back(CreateItem(ITEM_EXP, 1820, 300));
	case STAGE_3:
		items.push_back(CreateItem(ITEM_MP, 960, 380));
		break;
	}
}

//清除所有单位函数
void ClearAll(HWND hWnd)
{
	for (int i = 0; i < units.size(); i++)
		units[i] = NULL;
	units.clear();
	for (int i = 0; i < enemies.size(); i++)
		enemies[i] = NULL;
	enemies.clear();
	for (int i = 0; i < blocks.size(); i++)
		blocks[i] = NULL;
	blocks.clear();
	for (int i = 0; i < items.size(); i++)
		items[i] = NULL;
	items.clear();
	for (int i = 0; i < notices.size(); i++)
		notices[i] = NULL;
	notices.clear();
}

//重置全局变量值函数
void ResetGlobal()
{
	mouseX = 0;
	mouseY = 0;
	mouseDown = false;
	keyUpDown = false;
	keyDownDown = false;
	keyLeftDown = false;
	keyRightDown = false;
	keyActionDown = false;
	beingBusy = false;
	beingJump = false;
	beingFall = true;
	beingDash = false;
	isLastJumpOver = true;
	isNoticeOld = true;
	isPause = false;
	isFakeHelp = false;
	isFromLastStage = true;
}

#pragma endregion


#pragma region 主角行为函数

//基础行为函数
void UnitBehaviour_Basic(HWND hWnd, Unit* unit) {

	if (keyRightDown && !keyLeftDown)
		unit->direction = UNIT_DIRECT_RIGHT;
	else if (!keyRightDown && keyLeftDown)
		unit->direction = UNIT_DIRECT_LEFT;

	//判断当前状态, 以及判断是否需要状态变化
	int next_state = unit->state;
	switch (unit->state) {
	case UNIT_STATE_HOLD:
		unit->frame_sequence = FRAMES_HOLD;
		unit->frame_count = FRAMES_HOLD_COUNT;
		unit->vx = 0;
		if ((keyRightDown || keyLeftDown) && !beingDash) {
			next_state = UNIT_STATE_WALK;
			mciSendString(_T("OPEN res/Walk.wav type MPEGvideo ALIAS Walk"), NULL, 0, 0);
			mciSendString(_T("PLAY Walk"), NULL, 0, NULL);
		}
		break;
	case UNIT_STATE_WALK:
		unit->frame_sequence = FRAMES_WALK;
		unit->frame_count = FRAMES_WALK_COUNT;
		if (!keyRightDown && !keyLeftDown) {
			next_state = UNIT_STATE_HOLD;
			mciSendString(_T("CLOSE Walk"), NULL, 0, NULL);
		}
		else {
			if (unit->direction == UNIT_DIRECT_LEFT)
				unit->vx = -(UNIT_SPEED + unit->speed_mod);
			else
				unit->vx = UNIT_SPEED + unit->speed_mod;
		}
		break;
	case UNIT_STATE_ATTACK:
		unit->frame_sequence = FRAMES_ATTACK;
		unit->frame_count = FRAMES_ATTACK_COUNT;
		unit->vx = 0;
		break;
	case UNIT_STATE_S:
		unit->frame_sequence = FRAMES_S;
		unit->frame_count = FRAMES_S_COUNT;
		unit->vx = 0;
		break;
	case UNIT_STATE_D:
		unit->frame_sequence = FRAMES_D;
		unit->frame_count = FRAMES_D_COUNT;
		if (unit->type != UNIT_TYPE_BLUE)
			unit->vx = 0;
		break;
	case UNIT_STATE_F:
		unit->frame_sequence = FRAMES_F;
		unit->frame_count = FRAMES_F_COUNT;
		unit->vx = 0;
		break;
	};

	if (next_state != unit->state)
	{
		//状态变化
		unit->state = next_state;
		unit->frame_id = -1;
	}


	//角色位置改变
	unit->x += unit->vx;
	unit->y += unit->vy;
	unit->jump_height -= unit->vy;

	//判断是否获得道具
	GetItemBehaviour(hWnd, unit);
	
	//判断是否落地或走至空洞
	if (beingFall)
	{
		bool will_fall = true;
		for (int i = 0; i < blocks.size(); i++)
		{
			Block* block = blocks[i];
			if (unit->y + UNIT_SIZE_Y > block->y && unit->y < block->y &&
				((unit->x + UNIT_SIZE_X > block->x + TOLERANCE && unit->x + UNIT_SIZE_X < block->x + BLOCK_WIDTH - TOLERANCE) || (unit->x > block->x + TOLERANCE && unit->x < block->x + BLOCK_WIDTH - TOLERANCE)))
			{
				unit->y = block->y - UNIT_SIZE_Y;
				LandBehaviour();
				//触碰到沙子砖块后将其销毁
				if (block->type == BLOCK_SAND)
				{
					for (auto it = blocks.begin(); it != blocks.end(); it++) {
						if (*it == block) {
							it = blocks.erase(it);
							if (it == blocks.end())
								break;
						}
					}
					mciSendString(_T("CLOSE Sand"), NULL, 0, NULL);
					mciSendString(_T("OPEN res/Sand.wav ALIAS Sand"), NULL, 0, 0);
					mciSendString(_T("PLAY Sand"), NULL, 0, NULL);
				}
				//触碰到尖刺砖块后持续受到伤害
				if (block->type == BLOCK_THORN)
				{
					Injured(hWnd, unit, 1);
				}
				will_fall = false;
				break;
			}
		}
		if(will_fall)
			FallBehaviour();
	}


	//横向碰撞处理
	AvoidGoFarther(unit);

	//判断是否达到最大跳跃高度或触顶
	if (beingJump)
	{
		if (unit->jump_height >= UNIT_JUMP_HEIGHT_LIMIT)
			FallBehaviour();
		for (int i = 0; i < blocks.size(); i++)
		{
			Block* block = blocks[i];
			if (unit->y < block->y + BLOCK_HEIGHT && unit->y + UNIT_SIZE_Y>block->y + BLOCK_HEIGHT &&
				unit->x<block->x + BLOCK_WIDTH && unit->x + UNIT_SIZE_X>block->x)
			{
				unit->y = block->y + BLOCK_HEIGHT;
				FallBehaviour();
				break;
			}
		}
	}

	//判断是否掉入深渊死亡
	if (unit->y > WINDOW_HEIGHT * 1.2)
		DieBehaviour(hWnd);

	//动画运行到下一帧
	unit->frame_id++;
	unit->frame_id = unit->frame_id % unit->frame_count;

	int column = unit->frame_sequence[unit->frame_id];
	unit->frame_column = column + unit->direction * (UNIT_LAST_FRAME - 2 * column);


}

//横向碰撞处理函数
void AvoidGoFarther(Unit* unit)
{
	if (unit->x < 0)
		unit->x = 0;
	if (unit->x + UNIT_SIZE_X > WINDOW_WIDTH)
		unit->x = WINDOW_WIDTH - UNIT_SIZE_X;
	//检测砖块
	for (int i = 0; i < blocks.size(); i++)
	{
		Block* block = blocks[i];
		if (unit->y < block->y + BLOCK_HEIGHT && unit->y + UNIT_SIZE_Y > block->y)
		{
			if (unit->x + UNIT_SIZE_X > block->x && unit->x +UNIT_SIZE_X < block->x + TOLERANCE && unit->direction == UNIT_DIRECT_RIGHT)
			{
				unit->x = block->x - UNIT_SIZE_X;
				return;
			}
			if (unit->x > block->x + BLOCK_WIDTH - TOLERANCE && unit->x < block->x + BLOCK_WIDTH && unit->direction == UNIT_DIRECT_LEFT)
			{
				unit->x = block->x + BLOCK_WIDTH;
				return;
			}
		}
	}
	//检测敌人
	for (int i = 0; i < enemies.size(); i++)
	{
		if (beingDash)
			break;
		Enemy* enemy = enemies[i];
		if (unit->y < enemy->y + ENEMY_SIZE_Y && unit->y + UNIT_SIZE_Y > enemy->y)
		{
			if (unit->x + UNIT_SIZE_X > enemy->x && unit->x < enemy->x + ENEMY_SIZE_X && unit->direction == UNIT_DIRECT_RIGHT)
			{
				unit->x = enemy->x - UNIT_SIZE_X;
				return;
			}
			if (unit->x + UNIT_SIZE_X > enemy->x + ENEMY_SIZE_X && unit->x < enemy->x + ENEMY_SIZE_X && unit->direction == UNIT_DIRECT_LEFT)
			{
				unit->x = enemy->x + ENEMY_SIZE_X;
				return;
			}
		}
	}
}

//攻击行为函数
void AttackBehaviour(HWND hWnd, bool is_end)
{
	if (units.size() == 0)
		return;
	Unit* unit = units[0];
	if (is_end)
	{
		for (int i = 0; i < enemies.size(); i++)
		{
			Enemy* enemy = enemies[i];
			int distance = enemy->x - unit->x;
			if (distance >= -UNIT_ATTACK_DISTANCE && distance <= UNIT_ATTACK_DISTANCE
				&& unit->y<enemy->y + ENEMY_SIZE_Y && unit->y + UNIT_SIZE_Y>enemy->y)
			{
				if (distance < 0)
					unit->direction = UNIT_DIRECT_LEFT;
				else
					unit->direction = UNIT_DIRECT_RIGHT;
				Injuring(hWnd,unit,enemy,attack_damages[unit->type]);
			}
		}
		
		KillTimer(hWnd, TIMER_ATTACK);
		unit->state = UNIT_STATE_HOLD;
		beingBusy = false;
		return;
	}
	if (!beingBusy && !keyActionDown)
	{
		SetTimer(hWnd, TIMER_ATTACK, TIMER_ATTACK_ELAPSE, NULL);
		unit->state = UNIT_STATE_ATTACK;
		beingBusy = true;
		keyActionDown = true;
		mciSendString(_T("CLOSE Attack"), NULL, 0, NULL);
		mciSendString(_T("OPEN res/Attack.wav ALIAS Attack"), NULL, 0, 0);
		mciSendString(_T("PLAY Attack"), NULL, 0, NULL);
	}
}

//S技能行为函数
void AbilityBehaviour_1(HWND hWnd, bool is_end)
{
	if (units.size() == 0)
		return;
	Unit* unit = units[0];
	if (unit->level < 2)
		return;		//Level 2才能使用S技能
	if (is_end)
	{
		for (int i = 0; i < enemies.size(); i++)
		{
			Enemy* enemy = enemies[i];
			int distance = enemy->x - unit->x;
			if (distance >= -S_distances[unit->type] && distance <= S_distances[unit->type]
				&& unit->y<enemy->y + ENEMY_SIZE_Y && unit->y + UNIT_SIZE_Y>enemy->y)
			{
				if (distance < 0)
					unit->direction = UNIT_DIRECT_LEFT;
				else
					unit->direction = UNIT_DIRECT_RIGHT;
				Injuring(hWnd, unit, enemy, S_damages[unit->type]);
			}
		}

		KillTimer(hWnd, TIMER_S);
		unit->state = UNIT_STATE_HOLD;
		beingBusy = false;
		return;
	}
	if (!beingBusy && !keyActionDown)
	{
		if (unit->mana < S_costs[unit->type])
		{
			mciSendString(_T("CLOSE Shortage"), NULL, 0, NULL);
			mciSendString(_T("OPEN res/Shortage.wav ALIAS Shortage"), NULL, 0, 0);
			mciSendString(_T("PLAY Shortage"), NULL, 0, NULL);
			ManaLimitAttention(hWnd);
			return;
		}
		unit->mana -= S_costs[unit->type];
		_mana = unit->mana;
		switch (unit->type)
		{
		case UNIT_TYPE_RED:
			mciSendString(_T("CLOSE RedS"), NULL, 0, NULL);
			mciSendString(_T("OPEN res/Red_S.wav ALIAS RedS"), NULL, 0, 0);
			mciSendString(_T("PLAY RedS"), NULL, 0, NULL);
			break;
		case UNIT_TYPE_YELLOW:
			mciSendString(_T("CLOSE YellowS"), NULL, 0, NULL);
			mciSendString(_T("OPEN res/Yellow_S.wav ALIAS YellowS"), NULL, 0, 0);
			mciSendString(_T("PLAY YellowS"), NULL, 0, NULL);
			break;
		case UNIT_TYPE_BLUE:
			mciSendString(_T("CLOSE BlueS"), NULL, 0, NULL);
			mciSendString(_T("OPEN res/Blue_S.wav ALIAS BlueS"), NULL, 0, 0);
			mciSendString(_T("PLAY BlueS"), NULL, 0, NULL);
			break;
		}
		SetTimer(hWnd, TIMER_S, TIMER_S_ELAPSE, NULL);
		unit->state = UNIT_STATE_S;
		beingBusy = true;
		keyActionDown = true;
	}
}

//D技能行为函数
void AbilityBehaviour_2(HWND hWnd, bool is_end)
{
	if (units.size() == 0)
		return;
	Unit* unit = units[0];
	if (unit->level < 4)
		return;		//Level 4才能使用D技能
	if (is_end)
	{
		switch (unit->type)
		{
		case UNIT_TYPE_RED:
			SetTimer(hWnd, TIMER_SHOCKED, TIMER_SHOCKED_ELAPSE, NULL);
			for (int i = 0; i < enemies.size(); i++)
			{
				Enemy* enemy = enemies[i];
				enemy->state = ENEMY_STATE_HOLD;
			}
			break;
		case UNIT_TYPE_YELLOW:
			unit->health += 50;
			if (unit->health > 100)
				unit->health = 100;
			_health = unit->health;
			DropNotice(hWnd, 0, 50, 0);
			break;
		case UNIT_TYPE_BLUE:
			beingDash = false;
			break;
		}

		KillTimer(hWnd, TIMER_D);
		unit->state = UNIT_STATE_HOLD;
		beingBusy = false;
		return;
	}
	if (!beingBusy && !keyActionDown)
	{
		if (unit->mana < D_costs[unit->type])
		{
			mciSendString(_T("CLOSE Shortage"), NULL, 0, NULL);
			mciSendString(_T("OPEN res/Shortage.wav ALIAS Shortage"), NULL, 0, 0);
			mciSendString(_T("PLAY Shortage"), NULL, 0, NULL);
			ManaLimitAttention(hWnd);
			return;
		}
		unit->mana -= D_costs[unit->type];
		_mana = unit->mana;

		switch (unit->type)
		{
		case UNIT_TYPE_RED:
			mciSendString(_T("CLOSE RedD"), NULL, 0, NULL);
			mciSendString(_T("OPEN res/Red_D.wav ALIAS RedD"), NULL, 0, 0);
			mciSendString(_T("PLAY RedD"), NULL, 0, NULL);
			break;
		case UNIT_TYPE_YELLOW:
			mciSendString(_T("CLOSE YellowD"), NULL, 0, NULL);
			mciSendString(_T("OPEN res/Yellow_D.wav ALIAS YellowD"), NULL, 0, 0);
			mciSendString(_T("PLAY YellowD"), NULL, 0, NULL);
			break;
		case UNIT_TYPE_BLUE:
			if (unit->direction == UNIT_DIRECT_LEFT)
				unit->vx = -3 * (UNIT_SPEED + unit->speed_mod);
			else
				unit->vx = 3 * (UNIT_SPEED + unit->speed_mod);
			beingDash = true;
			mciSendString(_T("CLOSE BlueD"), NULL, 0, NULL);
			mciSendString(_T("OPEN res/Blue_D.wav ALIAS BlueD"), NULL, 0, 0);
			mciSendString(_T("PLAY BlueD"), NULL, 0, NULL);
			break;
		}

		SetTimer(hWnd, TIMER_D, TIMER_D_ELAPSE, NULL);
		unit->state = UNIT_STATE_D;
		beingBusy = true;
		keyActionDown = true;
	}
}

//F技能行为函数
void AbilityBehaviour_3(HWND hWnd, bool is_end)
{
	if (units.size() == 0)
		return;
	Unit* unit = units[0];
	if (unit->level < 5)
		return;		//Level 5才能使用F技能
	if (is_end)
	{
		switch (unit->type)
		{
		case UNIT_TYPE_RED:
			for (int i = 0; i < enemies.size(); i++)
			{
				Enemy* enemy = enemies[i];
				int distance = enemy->x - unit->x;
				if (distance >= -S_distances[unit->type] && distance <= S_distances[unit->type]
					&& unit->y<enemy->y + ENEMY_SIZE_Y && unit->y + UNIT_SIZE_Y>enemy->y)
				{
					if (distance < 0)
						unit->direction = UNIT_DIRECT_LEFT;
					else
						unit->direction = UNIT_DIRECT_RIGHT;
					Injuring(hWnd, unit, enemy, F_damages[unit->type]);
				}
			}
			break;
		case UNIT_TYPE_YELLOW:
			for (int i = 0; i < enemies.size(); i++)
			{
				Enemy* enemy = enemies[i];
				Injuring(hWnd, unit, enemy, F_damages[UNIT_TYPE_YELLOW]);
			}
			break;
		case UNIT_TYPE_BLUE:
			for (int i = 0; i < enemies.size(); i++)
			{
				Enemy* enemy = enemies[i];
				if (unit->y<enemy->y + ENEMY_SIZE_Y && unit->y + UNIT_SIZE_Y>enemy->y)
				{
					if ((unit->direction == UNIT_DIRECT_RIGHT && unit->x < enemy->x)
						|| (unit->direction == UNIT_DIRECT_LEFT && unit->x > enemy->x))
						Injuring(hWnd, unit, enemy, F_damages[UNIT_TYPE_BLUE]);
				}
			}
			break;
		}

		KillTimer(hWnd, TIMER_F);
		unit->state = UNIT_STATE_HOLD;
		beingBusy = false;
		return;
	}
	if (!beingBusy && !keyActionDown)
	{
		if (unit->mana < F_costs[unit->type])
		{
			mciSendString(_T("CLOSE Shortage"), NULL, 0, NULL);
			mciSendString(_T("OPEN res/Shortage.wav ALIAS Shortage"), NULL, 0, 0);
			mciSendString(_T("PLAY Shortage"), NULL, 0, NULL);
			ManaLimitAttention(hWnd);
			return;
		}
		switch (unit->type)
		{
		case UNIT_TYPE_RED:
			mciSendString(_T("CLOSE RedF"), NULL, 0, NULL);
			mciSendString(_T("OPEN res/Red_F.wav ALIAS RedF"), NULL, 0, 0);
			mciSendString(_T("PLAY RedF"), NULL, 0, NULL);
			break;
		case UNIT_TYPE_YELLOW:
			mciSendString(_T("CLOSE YellowF"), NULL, 0, NULL);
			mciSendString(_T("OPEN res/Yellow_F.wav ALIAS YellowF"), NULL, 0, 0);
			mciSendString(_T("PLAY YellowF"), NULL, 0, NULL);
			break;
		case UNIT_TYPE_BLUE:
			mciSendString(_T("CLOSE BlueF"), NULL, 0, NULL);
			mciSendString(_T("OPEN res/Blue_F.wav ALIAS BlueF"), NULL, 0, 0);
			mciSendString(_T("PLAY BlueF"), NULL, 0, NULL);
			break;
		}
		unit->mana -= F_costs[unit->type];
		_mana = unit->mana;
		SetTimer(hWnd, TIMER_F, TIMER_F_ELAPSE, NULL);
		unit->state = UNIT_STATE_F;
		beingBusy = true;
		keyActionDown = true;
	}
}

//形态切换行为函数
void SwitchTypeBehaviour(HWND hWnd, int switch_direction)
{
	if (units.size() == 0)
		return;
	if (beingBusy)
		return;
	Unit* unit = units[0];
	unit->type = (unit->type + switch_direction) % 3;
	unit->frame_row = unit->type;
	unit->speed_mod = speed_mods[unit->type];
	unit->defence = 1 + defence_mods[unit->type];
}

//跳跃行为函数
void JumpBehaviour()
{
	if (currentStage->stageID < STAGE_1)
		return;
	if (units.size() == 0)
		return;
	if (!isLastJumpOver)
		return;
	Unit* unit = units[0];
	int jump_limit = 1;	//可跳跃次数
	if (unit->level > 2)	//level为3时解锁二段跳
		jump_limit = 2;
	if (unit->jump_times < jump_limit)
	{
		unit->vy = UNIT_JUMP_SPEED;
		unit->jump_times++;
		unit->jump_height = 0;
		beingJump = true;
		isLastJumpOver = false;
		beingFall = false;
	}
}

//下落行为函数
void FallBehaviour()
{
	if (units.size() == 0)
		return;
	Unit* unit = units[0];
	unit->vy = UNIT_FALL_SPEED;
	beingFall = true;
	beingJump = false;
}

//落地行为函数
void LandBehaviour()
{
	if (units.size() == 0)
		return;
	Unit* unit = units[0];
	unit->vy = 0;
	unit->jump_times = 0;
	beingJump = false;
}

//拾取道具函数
void GetItemBehaviour(HWND hWnd, Unit* unit)
{
	for (int i = 0; i < items.size(); i++)
	{
		Item* item = items[i];
		if (item->x <= unit->x + UNIT_SIZE_X && item->x + ITEM_SIZE_X >= unit->x
			&& item->y <= unit->y + UNIT_SIZE_Y && item->y + ITEM_SIZE_Y >= unit->y)
		{
			switch (item->type)
			{
			case ITEM_HP:
				unit->health += ITEM_HP_VALUE;
				if (unit->health > 100)
					unit->health = 100;
				_health = unit->health;
				DropNotice(hWnd, 0, ITEM_HP_VALUE, 0);
				break;
			case ITEM_MP:
				unit->mana += ITEM_MP_VALUE;
				if (unit->mana > 100)
					unit->mana = 100;
				_mana = unit->mana;
				DropNotice(hWnd, 0, 0, ITEM_MP_VALUE);
				break;
			case ITEM_EXP:
				unit->experience += ITEM_EXP_VALUE;
				DropNotice(hWnd, ITEM_EXP_VALUE, 0, 0);
				if (unit->experience >= UNIT_UPGRADE_NEED)
					UpgradeBehaviour(hWnd, unit);
				break;
			}
			//删除道具
			for (auto it = items.begin(); it != items.end(); it++) {
				if (*it == item) {
					it = items.erase(it);
					if (it == items.end())
						break;
				}
			}
			mciSendString(_T("CLOSE Get"), NULL, 0, NULL);
			mciSendString(_T("OPEN res/Get.wav ALIAS Get"), NULL, 0, 0);
			mciSendString(_T("PLAY Get"), NULL, 0, NULL);
			break;
		}
	}
}

//升级行为函数
void UpgradeBehaviour(HWND hWnd, Unit* unit)
{
	if (unit->level == 5)
		return;
	unit->level++;
	unit->experience -= UNIT_UPGRADE_NEED;
	unit->mana + 25;
	if (unit->mana > 100)
		unit->mana = 100;
	_mana = unit->mana;
	unit->health + 25;
	if (unit->health > 100)
		unit->health = 100;
	_health = unit->health;
	DropNotice(hWnd, 0, 25, 25);
	UpgradeNotice(hWnd, unit->level);
}

//死亡行为函数
void DieBehaviour(HWND hWnd)
{
	mciSendString(_T("CLOSE Die"), NULL, 0, NULL);
	mciSendString(_T("OPEN res/Die.wav ALIAS Die"), NULL, 0, 0);
	mciSendString(_T("PLAY Die wait"), NULL, 0, NULL);
	ClearAll(hWnd);
	bmp_Background = InitSingleBackGround(hWnd, bmp_FailBackground);
	InitStage(hWnd, STAGE_FAIL);
}

//关卡传送函数
void ChangeStage(HWND hWnd)
{
	if (units.size() == 0)
		return;
	if (beingBusy)
		return;
	Unit* unit = units[0];
	if (unit->x <= GATE1_LEFT + GATE_SIZE_X && unit->x + UNIT_SIZE_X >= GATE1_LEFT
		&& unit->y <= GATE1_UP + GATE_SIZE_Y && unit->y + UNIT_SIZE_Y >= GATE1_UP)
	{
		if (currentStage->stageID == STAGE_1)
			return;
		else
		{
			_stage = (currentStage->stageID - 1);
			_experience = unit->experience;
			_level = unit->level;
			_type = unit->type;
			isFromLastStage = false;
			mciSendString(_T("CLOSE Trans"), NULL, 0, NULL);
			mciSendString(_T("OPEN res/Transport.wav ALIAS Trans"), NULL, 0, 0);
			mciSendString(_T("PLAY Trans wait"), NULL, 0, NULL);
			ClearAll(hWnd);
			InitStage(hWnd, _stage);
			return;
		}
	}
	else if (unit->x <= GATE2_LEFT + GATE_SIZE_X && unit->x + UNIT_SIZE_X >= GATE2_LEFT
		&& unit->y <= GATE2_UP + GATE_SIZE_Y && unit->y + UNIT_SIZE_Y >= GATE2_UP)
	{
		if (currentStage->stageID == STAGE_3)
			return;
		else
		{
  			_stage = (currentStage->stageID + 1) % 4;
			_experience = unit->experience;
			_level = unit->level;
			_type = unit->type;
			isFromLastStage = true;
			mciSendString(_T("CLOSE Trans"), NULL, 0, NULL);
			mciSendString(_T("OPEN res/Transport.wav ALIAS Trans"), NULL, 0, 0);
			mciSendString(_T("PLAY Trans wait"), NULL, 0, NULL);
			ClearAll(hWnd);
			InitStage(hWnd, _stage);
			return;
		}
	}
}

#pragma endregion


#pragma region 其他行为函数

//敌人行为函数
void EnemyBehaviour(HWND hWnd, Enemy* enemy)
{
	if (units.size() == 0)
		return;
	Unit* unit = units[0];
	int distance = (unit->x + 0.5 * UNIT_SIZE_X) - (enemy->x + 0.5 * ENEMY_SIZE_X);
	//判断当前状态, 以及判断是否需要状态变化
	int next_state = enemy->state;
	switch (enemy->state) {
		case ENEMY_STATE_HOLD:
			enemy->frame_sequence = FRAMES_HOLD;
			enemy->frame_count = FRAMES_HOLD_COUNT;
			enemy->vx = 0;
			enemy->vy = 0;
			break;
		case ENEMY_STATE_WALK:
			enemy->frame_sequence = FRAMES_WALK;
			enemy->frame_count = FRAMES_WALK_COUNT;
			if (enemy->direction == ENEMY_DIRECT_LEFT)
			{
				enemy->vx = -enemy_speeds[enemy->type];
				if (enemy->offset < enemy_left_offsets[enemy->type])
				{
					enemy->x += enemy_left_offsets[enemy->type] - enemy->offset;
					enemy->offset = enemy_left_offsets[enemy->type];
					enemy->direction = ENEMY_DIRECT_RIGHT;
				}
			}
			else
			{
				enemy->vx = enemy_speeds[enemy->type];
				if (enemy->offset >= enemy_right_offsets[enemy->type])
				{
					enemy->x -= enemy->offset - enemy_right_offsets[enemy->type];
					enemy->offset = enemy_right_offsets[enemy->type];
					enemy->direction = ENEMY_DIRECT_LEFT;
				}
			}
			if (distance <= enemy_distance[enemy->type] && distance >= - enemy_distance[enemy->type]
				&& unit->y<enemy->y + ENEMY_SIZE_Y && unit->y + UNIT_SIZE_Y>enemy->y)
			{
				if (distance < 0)
					enemy->direction = ENEMY_DIRECT_LEFT;
				else
					enemy->direction = ENEMY_DIRECT_RIGHT;
				next_state = ENEMY_STATE_ATTACK;
				SetTimer(hWnd, TIMER_ENEMY_ATTACK, TIMER_ENEMY_ATTACK_ELAPSE, NULL);
			}
			break;
		case ENEMY_STATE_ATTACK:
			enemy->frame_sequence = FRAMES_ATTACK;
			enemy->frame_count = FRAMES_ATTACK_COUNT;
			enemy->vx = 0;
			if ((distance > enemy_distance[enemy->type] && distance < -enemy_distance[enemy->type])
				|| unit->y + UNIT_SIZE_Y<enemy->y || unit->y>enemy->y + ENEMY_SIZE_Y)
				next_state = ENEMY_STATE_WALK;
			break;
	}

	if (next_state != enemy->state) {
		//状态变化
		enemy->state = next_state;
		enemy->frame_id = -1;
	}

	//角色位置改变
	enemy->x += enemy->vx;
	enemy->y += enemy->vy;
	enemy->offset += enemy->vx;

	//动画运行到下一帧
	enemy->frame_id++;
	enemy->frame_id = enemy->frame_id % enemy->frame_count;

	int column = enemy->frame_sequence[enemy->frame_id];
	enemy->frame_column = column + enemy->direction * (ENEMY_LAST_FRAME - 2 * column);
		
}

//敌人攻击函数
void EnemyAttack(HWND hWnd, WPARAM wParam)
{
	Unit* unit = units[0];
	for (int i = 0; i < enemies.size(); i++)
	{
		Enemy* enemy = enemies[i];
		if (enemy->state == ENEMY_STATE_ATTACK)
		{
			int distance;
			if (enemy->direction == ENEMY_DIRECT_LEFT)
				distance = unit->x + 0.5 * UNIT_SIZE_X - enemy->x - 0.5 * ENEMY_SIZE_X + 2;
			else
				distance = unit->x + 0.5 * UNIT_SIZE_X - enemy->x - 0.5 * ENEMY_SIZE_X - 2;
			if (distance < enemy_distance[enemy->type] && distance>-enemy_distance[enemy->type] && unit->y + UNIT_SIZE_Y > enemy->y && unit->y < enemy->y + ENEMY_SIZE_Y)
				Injured(hWnd, unit, enemy_damages[enemy->type]);
			enemy->state = ENEMY_STATE_WALK;
		}
	}

	KillTimer(hWnd, TIMER_ENEMY_ATTACK);
}

//敌人解除控制状态函数
void ReliefShocked(HWND hWnd)
{
	for (int i = 0; i < enemies.size(); i++)
	{
		Enemy* enemy = enemies[i];
		enemy->state = ENEMY_STATE_WALK;
	}
}

//攻击伤害结算函数
void Injuring(HWND hWnd, Unit* unit, Enemy* enemy, int damage)
{
	enemy->health -= damage * enemy->defence;
	if (enemy->health <= 0)
	{
		if (enemy->type == BOSS)
		{
			bmp_Background = bmp_VictoryBackground;
			InitStage(hWnd, STAGE_VICTORY);
			return;
		}

		//处理升级与掉落
		unit->experience += enemy->exp;
		if (unit->experience >= UNIT_UPGRADE_NEED)
			UpgradeBehaviour(hWnd, unit);
		unit->mana += enemy->ma;
		if (unit->mana > 100)
			unit->mana = 100;
		_mana = unit->mana;
		DropNotice(hWnd, enemy->exp, 0, enemy->ma);

		//删除敌人
		for (auto it = enemies.begin(); it != enemies.end(); it++) {
			if (*it == enemy) {
				it = enemies.erase(it);
				if (it == enemies.end())
					break;
			}
		}
	}
}

//被攻击伤害结算函数
void Injured(HWND hWnd, Unit* unit, int damage)
{
	unit->health -= damage * unit->defence;
	if (unit->health <= 0)
		DieBehaviour(hWnd);
	else
	{
		_health = unit->health;
		InvalidateRect(hWnd, NULL, FALSE);
	}
}

//补充最终关卡的MP道具函数
void MPSupply(HWND hWnd)
{
	if (currentStage->stageID == STAGE_3 && items.size() == 0)
		items.push_back(CreateItem(ITEM_MP, 960, 380));
}

#pragma endregion

#pragma region 绘图函数
// 绘图函数
void Paint(HWND hWnd)
{

	PAINTSTRUCT ps;
	HDC hdc_window = BeginPaint(hWnd, &ps);

	HDC hdc_memBuffer = CreateCompatibleDC(hdc_window);
	HDC hdc_loadBmp = CreateCompatibleDC(hdc_window);

	//初始化缓存
	HBITMAP	blankBmp = CreateCompatibleBitmap(hdc_window, WINDOW_WIDTH, WINDOW_HEIGHT);
	SelectObject(hdc_memBuffer, blankBmp);

	// 绘制背景到缓存
	SelectObject(hdc_loadBmp, bmp_Background);
	BitBlt(hdc_memBuffer, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, hdc_loadBmp, 0, 0, SRCCOPY);

	// 按场景分类绘制内容到缓存
	if (currentStage->stageID == STAGE_STARTMENU) {

	}
	else if (currentStage->stageID >= STAGE_1 && currentStage->stageID <= STAGE_3) //TODO：添加多个游戏场景
	{
		//绘制砖块到缓存
		for (int i = 0; i < blocks.size(); i++)
		{
			Block* block = blocks[i];
			SelectObject(hdc_loadBmp, block->img);
			TransparentBlt(
				hdc_memBuffer, block->x, block->y,
				BLOCK_WIDTH, BLOCK_HEIGHT,
				hdc_loadBmp, 0, BLOCK_HEIGHT * block->type, BLOCK_WIDTH, BLOCK_HEIGHT,
				RGB(220, 220, 0)
			);
		}

		// 绘制敌人到缓存
		for (int i = 0; i < enemies.size(); i++)
		{
			Enemy* enemy = enemies[i];
			SelectObject(hdc_loadBmp, enemy->img);
			if (enemy->type == BOSS)
			{
				TransparentBlt(hdc_memBuffer, enemy->x, enemy->y, BOSS_SIZE_X, BOSS_SIZE_Y,
					hdc_loadBmp, BOSS_SIZE_X * enemy->frame_column, BOSS_SIZE_Y * enemy->frame_row, BOSS_SIZE_X, BOSS_SIZE_Y,
					RGB(0, 245, 0)
				);
			}
			else
			{
				TransparentBlt(hdc_memBuffer, enemy->x, enemy->y, ENEMY_SIZE_X, ENEMY_SIZE_Y,
					hdc_loadBmp, ENEMY_SIZE_X * enemy->frame_column, ENEMY_SIZE_Y * enemy->frame_row, ENEMY_SIZE_X, ENEMY_SIZE_Y,
					RGB(0, 0, 80)
				);
			}
		}

		//绘制场景道具到缓存
		for (int i = 0; i < items.size(); i++)
		{
			Item* item = items[i];
			SelectObject(hdc_loadBmp, item->img);
			TransparentBlt(
				hdc_memBuffer, item->x, item->y,
				ITEM_SIZE_X, ITEM_SIZE_Y,
				hdc_loadBmp, 0, ITEM_SIZE_Y * item->type, ENEMY_SIZE_X, ENEMY_SIZE_Y,
				RGB(220, 220, 220)
			);
		}

		// 绘制主角到缓存
		for (int i = 0; i < units.size(); i++) {
			Unit* unit = units[i];
			SelectObject(hdc_loadBmp, unit->img);
			TransparentBlt(
				hdc_memBuffer, unit->x, unit->y,
				UNIT_SIZE_X, UNIT_SIZE_Y,
				hdc_loadBmp, UNIT_SIZE_X * unit->frame_column, UNIT_SIZE_Y * unit->frame_row, UNIT_SIZE_X, UNIT_SIZE_Y,
				RGB(220, 220, 220)
			);
		}
		
		//绘制血量与法力条到缓存
		SelectObject(hdc_loadBmp, bmp_Container);
		TransparentBlt(hdc_memBuffer, BAR_X, HEALTH_BAR_Y, CONTAINER_LENGTH, CONTAINER_WIDTH,
			hdc_loadBmp, 0, 0, CONTAINER_LENGTH, CONTAINER_WIDTH, RGB(0, 0, 0));
		SelectObject(hdc_loadBmp, bmp_Container);
		TransparentBlt(hdc_memBuffer, BAR_X, MANA_BAR_Y, CONTAINER_LENGTH, CONTAINER_WIDTH,
			hdc_loadBmp, 0, 0, CONTAINER_LENGTH, CONTAINER_WIDTH, RGB(0, 0, 0));
		SelectObject(hdc_loadBmp, bmp_Health);
		TransparentBlt(hdc_memBuffer, BAR_X, HEALTH_BAR_Y, _health * CONTAINER_LENGTH / 100, CONTAINER_WIDTH,
			hdc_loadBmp, 0, 0, _health * CONTAINER_LENGTH / 100, CONTAINER_WIDTH, RGB(0, 0, 0));
		SelectObject(hdc_loadBmp, bmp_Mana);
		TransparentBlt(hdc_memBuffer, BAR_X, MANA_BAR_Y, _mana * CONTAINER_LENGTH / 100, CONTAINER_WIDTH,
			hdc_loadBmp, 0, 0, _mana * CONTAINER_LENGTH / 100, CONTAINER_WIDTH, RGB(0, 0, 0));

		//绘制消息到缓存
		for (int i = 0; i < notices.size(); i++) {
			Notice* notice = notices[i];
			SelectObject(hdc_loadBmp, notice->img);
			TransparentBlt(hdc_memBuffer, WINDOW_WIDTH-NOTICE_SIZE_X, i*NOTICE_SIZE_Y, NOTICE_SIZE_X, NOTICE_SIZE_Y,
				hdc_loadBmp, 0, NOTICE_SIZE_Y * notice->type, NOTICE_SIZE_X, NOTICE_SIZE_Y, RGB(220, 220, 220));
		}
	}

	//绘制暂停页面到缓存
	if (isPause)
	{
		SelectObject(hdc_loadBmp, bmp_PauseBackground);
		BitBlt(hdc_memBuffer, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, hdc_loadBmp, 0, 0, SRCCOPY);
	}

	//绘制假帮助页面到缓存
	if (isFakeHelp)
	{
		SelectObject(hdc_loadBmp, bmp_HelpBackground);
		BitBlt(hdc_memBuffer, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, hdc_loadBmp, 0, 0, SRCCOPY);
	}

	//绘制选项指针到缓存
	if (indicator->visible)
	{
		SelectObject(hdc_loadBmp, indicator->img);
		TransparentBlt(
			hdc_memBuffer, indicator->x, indicator->y,
			indicator->width, indicator->height,
			hdc_loadBmp, 0, 0, indicator->width, indicator->height,
			RGB(120, 120, 120)
		);
	}

	// 绘制按钮到缓存
	for (int i = 0; i < buttons.size(); i++)
	{
		Button* button = buttons[i];
		if (button->visible)
		{
			SelectObject(hdc_loadBmp, button->img);
			TransparentBlt(
				hdc_memBuffer, button->x, button->y,
				button->width, button->height,
				hdc_loadBmp, 0, 0, button->width, button->height,
				RGB(120, 120, 120)
			);
		}
	}


	// 最后将所有的信息绘制到屏幕上
	BitBlt(hdc_window, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, hdc_memBuffer, 0, 0, SRCCOPY);

	// 回收资源所占的内存（非常重要）
	DeleteObject(blankBmp);
	DeleteDC(hdc_memBuffer);
	DeleteDC(hdc_loadBmp);

	// 结束绘制
	EndPaint(hWnd, &ps);
}



//初始化单个背景图函数
HBITMAP InitSingleBackGround(HWND hWnd, HBITMAP bmp_src) {

	srand(time(NULL));
	PAINTSTRUCT ps;
	HDC hdc_window = BeginPaint(hWnd, &ps);

	HDC hdc_memBuffer = CreateCompatibleDC(hdc_window);
	HDC hdc_loadBmp = CreateCompatibleDC(hdc_window);

	//初始化缓存
	HBITMAP	bmp_output = CreateCompatibleBitmap(hdc_window, WINDOW_WIDTH, WINDOW_HEIGHT);
	SelectObject(hdc_memBuffer, bmp_output);

	//加载资源
	SelectObject(hdc_loadBmp, bmp_src);
	TransparentBlt(
		hdc_memBuffer, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT,
		hdc_loadBmp, 0, 0, 1920, 1080,
		RGB(0, 0, 0));

	// 最后将所有的信息绘制到屏幕上
	BitBlt(hdc_window, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, hdc_memBuffer, 0, 0, SRCCOPY);

	// 回收资源所占的内存（非常重要）
	DeleteDC(hdc_memBuffer);
	DeleteDC(hdc_loadBmp);

	// 结束绘制
	EndPaint(hWnd, &ps);

	return bmp_output;
}



// 初始化随机生成背景函数
HBITMAP InitBackGround(HWND hWnd, HBITMAP bmp_src) {

	srand(time(NULL));
	PAINTSTRUCT ps;
	HDC hdc_window = BeginPaint(hWnd, &ps);

	HDC hdc_memBuffer = CreateCompatibleDC(hdc_window);
	HDC hdc_loadBmp = CreateCompatibleDC(hdc_window);

	//初始化缓存
	HBITMAP	bmp_output = CreateCompatibleBitmap(hdc_window, WINDOW_WIDTH, WINDOW_HEIGHT);
	SelectObject(hdc_memBuffer, bmp_output);

	//加载资源
	SelectObject(hdc_loadBmp, bmp_src);
	for (int i = 0; i < BG_ROWS; i++) {

		int src_row = 1;
		if (i >= BG_ROWS_SKY)
			src_row = 0;

		for (int j = 0; j < BG_COLUMNS; j++) {
			int src_column = rand() % BG_SRC_COUNT;

			TransparentBlt(
				hdc_memBuffer, j*1920, i*1080,
				1920, 1080,
				hdc_loadBmp, src_column * 1920, src_row * 1080,
				1920, 1080,
				RGB(0, 0, 0)
			);

		}
	}

	// 最后将所有的信息绘制到屏幕上
	BitBlt(hdc_window, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, hdc_memBuffer, 0, 0, SRCCOPY);

	// 回收资源所占的内存（非常重要）
	DeleteDC(hdc_memBuffer);
	DeleteDC(hdc_loadBmp);

	// 结束绘制
	EndPaint(hWnd, &ps);

	return bmp_output;
}

//发布掉落消息函数
void DropNotice(HWND hWnd, int exp, int hp, int ma)
{
	if (exp > 0)
	{
		notices.push_back(CreateNotice(hWnd, NOTICE_EXP_ADD));
		isNoticeOld = false;
		if (notices.size() == 1)
			SetTimer(hWnd, TIMER_NOTICE, TIMER_NOTICE_ELAPSE, NULL);
	}
	if (hp > 0)
	{
		notices.push_back(CreateNotice(hWnd, NOTICE_HP_RECOVER));
		isNoticeOld = false;
		if (notices.size() == 1)
			SetTimer(hWnd, TIMER_NOTICE, TIMER_NOTICE_ELAPSE, NULL);
	}
	if (ma > 0)
	{
		notices.push_back(CreateNotice(hWnd, NOTICE_MP_RECOVER));
		isNoticeOld = false;
		if (notices.size() == 1)
			SetTimer(hWnd, TIMER_NOTICE, TIMER_NOTICE_ELAPSE, NULL);
	}
}

//发布升级消息函数
void UpgradeNotice(HWND hWnd, int new_level)
{
	switch (new_level)
	{
	case 2:
		notices.push_back(CreateNotice(hWnd, NOTICE_LEVELUP_2));
		break;
	case 3:
		notices.push_back(CreateNotice(hWnd, NOTICE_LEVELUP_3));
		break;
	case 4:
		notices.push_back(CreateNotice(hWnd, NOTICE_LEVELUP_4));
		break;
	case 5:
		notices.push_back(CreateNotice(hWnd, NOTICE_LEVELUP_5));
		break;
	}
	isNoticeOld = false;
	if (notices.size() == 1)
		SetTimer(hWnd, TIMER_NOTICE, TIMER_NOTICE_ELAPSE, NULL);
}

//发布法力值不足消息函数
void ManaLimitAttention(HWND hWnd)
{
	notices.push_back(CreateNotice(hWnd, NOTICE_MANA_LIMIT));
	isNoticeOld = false;
	if (notices.size() == 1)
		SetTimer(hWnd, TIMER_NOTICE, TIMER_NOTICE_ELAPSE, NULL);
}

//结束消息显示函数
void EndNotice(HWND hWnd)
{
	if (isNoticeOld)
	{
		for (int i = 0; i < notices.size(); i++)
			notices[i] = NULL;
		notices.clear();
		KillTimer(hWnd, TIMER_NOTICE);
	}
	else
	{
		isNoticeOld = true;
	}
}

#pragma endregion


#pragma region 具体执行交互的函数

//方向处理函数
void HaddleDirection(bool value, int direction)
{
	if (currentStage->stageID >= STAGE_1)
	{
		switch (direction)
		{
		case 0:
			keyLeftDown = value;
			break;
		case 1:
			keyRightDown = value;
			break;
		}
	}
}

//选项指针切换函数
void SwitchOption(HWND hWnd, int direction)
{
	if (currentStage->stageID == STAGE_INTRODUCE)
		return;
	if (currentStage->stageID == STAGE_STARTMENU)
	{
		INDICATOR_YINDEX = (INDICATOR_YINDEX + direction) % 3;
		indicator->y = INDICATOR_Y[INDICATOR_YINDEX];
		InvalidateRect(hWnd, NULL, FALSE);
		mciSendString(_T("CLOSE Switch"), NULL, 0, NULL);
		mciSendString(_T("OPEN res/Switch.wav ALIAS Switch"), NULL, 0, 0);
		mciSendString(_T("PLAY Switch"), NULL, 0, NULL);
	}
	else if (isPause && !currentStage->timerOn && !isFakeHelp)
	{
		INDICATOR_YINDEX = (INDICATOR_YINDEX + direction) % 3;
		indicator->y = INDICATOR_Y[INDICATOR_YINDEX];
		InvalidateRect(hWnd, NULL, FALSE);
		mciSendString(_T("CLOSE Switch"), NULL, 0, NULL);
		mciSendString(_T("OPEN res/Switch.wav ALIAS Switch"), NULL, 0, 0);
		mciSendString(_T("PLAY Switch"), NULL, 0, NULL);
	}
}

//选项确定函数
void ConfirmOption(HWND hWnd)
{
	if (currentStage->stageID == STAGE_INTRODUCE)
		return;
	if (currentStage->stageID == STAGE_STARTMENU)
	{
		switch (INDICATOR_YINDEX)
		{
		case 0:
			InitStage(hWnd, _stage);
			break;
		case 1:
			bmp_Background = InitSingleBackGround(hWnd, bmp_HelpBackground);
			InitStage(hWnd, STAGE_HELP);
			break;
		case 2:
			PostQuitMessage(0);
			break;
		default:
			break;
		}
	}
	else if (isPause && !currentStage->timerOn && !isFakeHelp)
	{
		switch (INDICATOR_YINDEX)
		{
		case 0:
			isPause = false;
			indicator->visible = false;
			currentStage->timerOn = true;
			for (int i = 0; i < buttons.size(); i++)
				buttons[i]->visible = false;
			break;
		case 1:
			indicator->visible = false;
			for (int i = 0; i < buttons.size(); i++)
			{
				Button* button = buttons[i];
				if (button->buttonID == BUTTON_BACK)
					button->visible = true;
				else
					button->visible = false;
			}
			isFakeHelp = true;
			InvalidateRect(hWnd, NULL, FALSE);
			break;
		case 2:
			BackToStartMenu(hWnd);
			break;
		default:
			break;
		}
	}
	mciSendString(_T("CLOSE Conf"), NULL, 0, NULL);
	mciSendString(_T("OPEN res/Confirm.wav ALIAS Conf"), NULL, 0, 0);
	mciSendString(_T("PLAY Conf"), NULL, 0, NULL);
}

//返回开始界面函数
void BackToStartMenu(HWND hWnd)
{
	mciSendString(_T("CLOSE Conf"), NULL, 0, NULL);
	mciSendString(_T("OPEN res/Confirm.wav ALIAS Conf"), NULL, 0, 0);
	mciSendString(_T("PLAY Conf"), NULL, 0, NULL);
	ClearAll(hWnd);
	InitStage(hWnd, STAGE_STARTMENU);
}

//退出“假帮助页面”函数
void LeaveFakeHelp(HWND hWnd)
{
	isFakeHelp = false;
	indicator->visible = true;
	for (int i = 0; i < buttons.size(); i++)
	{
		Button* button = buttons[i];
		if (button->buttonID == BUTTON_CONTINUE || button->buttonID == BUTTON_HELP || button->buttonID == BUTTON_HOME)
			button->visible = true;
		else
			button->visible = false;
	}
	mciSendString(_T("CLOSE Conf"), NULL, 0, NULL);
	mciSendString(_T("OPEN res/Confirm.wav ALIAS Conf"), NULL, 0, 0);
	mciSendString(_T("PLAY Conf"), NULL, 0, NULL);
	InvalidateRect(hWnd, NULL, FALSE);
}

//暂停游戏函数
void PauseGame(HWND hWnd)
{
	if (currentStage->stageID >= STAGE_1)
	{
		isPause = true;
		indicator->visible = true;
		currentStage->timerOn = false;
		currentStage->itemTimerOn = false;
		for (int i = 0; i < buttons.size(); i++)
		{
			Button* button = buttons[i];
			if (button->buttonID == BUTTON_CONTINUE || button->buttonID == BUTTON_HELP || button->buttonID == BUTTON_HOME)
				button->visible = true;
			else
				button->visible = false;
		}
		mciSendString(_T("CLOSE Switch"), NULL, 0, NULL);
		mciSendString(_T("OPEN res/Switch.wav ALIAS Switch"), NULL, 0, 0);
		mciSendString(_T("PLAY Switch"), NULL, 0, NULL);
		InvalidateRect(hWnd, NULL, FALSE);
	}
}

#pragma endregion