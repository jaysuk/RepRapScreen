#pragma once
#include "uart/ProtocolSender.h"
#include "storage/StoragePreferences.h"
#include "uart/UartContext.h"
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include "utils/BrightnessHelper.h"
#include "entry/EasyUIContext.h"
#include "manager/ConfigManager.h"
#include "os/MountMonitor.h"
#include <algorithm>
#include "json/json.h"

void Hardware_serial_transmission(const std::string& data) ;

/**
 * 当界面构造时触发
 */

// Controlling constants
const uint32_t defaultPrinterPollInterval = 500;	// poll interval in milliseconds
const uint32_t printerResponseTimeout = 2000;	// shortest time after a response that we send another poll (gives printer time to catch up)
const uint32_t slowPrinterPollInterval = 4000;		// poll interval in milliseconds when screensaver active
const uint32_t touchBeepLength = 20;				// beep length in ms
const uint32_t touchBeepFrequency = 4500;			// beep frequency in Hz. Resonant frequency of the piezo sounder is 4.5kHz.
const uint32_t errorBeepLength = 100;
const uint32_t errorBeepFrequency = 2250;
const uint32_t normalTouchDelay = 250;				// how long we ignore new touches for after pressing Set
const uint32_t repeatTouchDelay = 100;				// how long we ignore new touches while pressing up/down, to get a reasonable repeat rate

const int parserMinErrors = 2;

static uint32_t lastActionTime = 0;

// These defines control which detailed M409 requests will be sent
// If one of the fields in the disabled ones need to be fetched the
// corresponding define has to be set to (1)
#define FETCH_BOARDS		(1)
#define FETCH_DIRECTORIES	(0)
#define FETCH_FANS			(0)
#define FETCH_HEAT			(1)
#define FETCH_INPUTS		(0)
#define FETCH_JOB			(1)
#define FETCH_MOVE			(1)
#define FETCH_NETWORK		(1)
#define FETCH_SCANNER		(0)
#define FETCH_SENSORS		(0)
#define FETCH_SPINDLES		(1)
#define FETCH_STATE			(1)
#define FETCH_TOOLS			(1)
#define FETCH_VOLUMES		(1)

static uint32_t lastTouchTime;

static uint32_t lastPollTime = 0;
static uint32_t lastResponseTime = 0;
static uint32_t lastOutOfBufferResponse = 0;

static uint8_t oobCounter = 0;
static bool outOfBuffers = false;

//static FirmwareFeatureMap firmwareFeatures;

static bool screensaverActive = false;						// true if screensaver is active
static bool isDelta = false;

//static size_t numAxes = MIN_AXES;
static int32_t beepFrequency = 0;
static int32_t beepLength = 0;

static uint32_t messageSeq = 0;
static uint32_t newMessageSeq = 0;

static uint32_t fileSize = 0;
static uint8_t visibleAxesCounted = 0;

static int8_t lastBed = -1;
static int8_t lastChamber = -1;
static int8_t lastSpindle = -1;
static int8_t lastTool = -1;
static uint32_t remoteUpTime = 0;
static bool initialized = false;
static float pollIntervalMultiplier = 1.0;
static uint32_t printerPollInterval = defaultPrinterPollInterval;



static void onUI_init(){
    //Tips :添加 UI初始化的显示代码到这里,如:mText1Ptr->setText("123");

}

/**
 * 当切换到该界面时触发
 */
static void onUI_intent(const Intent *intentPtr) {
    if (intentPtr != NULL) {
        //TODO
    }
}

/*
 * 当界面显示时触发
 */
static void onUI_show() {

}

/*
 * 当界面隐藏时触发
 */
static void onUI_hide() {

}

/*
 * 当界面完全退出时触发
 */
static void onUI_quit() {

}

/**
 * 串口数据回调接口
 */
static void onProtocolDataUpdate(const SProtocolData &data) {

}

/**
 * 定时器触发函数
 * 不建议在此函数中写耗时操作，否则将影响UI刷新
 * 参数： id
 *         当前所触发定时器的id，与注册时的id相同
 * 返回值: true
 *             继续运行当前定时器
 *         false
 *             停止运行当前定时器
 */

/**
 * 有新的触摸事件时触发
 * 参数：ev
 *         新的触摸事件
 * 返回值：true
 *            表示该触摸事件在此被拦截，系统不再将此触摸事件传递到控件上
 *         false
 *            触摸事件将继续传递到控件上
 */
static bool onmainActivityTouchEvent(const MotionEvent &ev) {
    switch (ev.mActionStatus) {
		case MotionEvent::E_ACTION_DOWN://触摸按下
			//LOGD("时刻 = %ld 坐标  x = %d, y = %d", ev.mEventTime, ev.mX, ev.mY);
			break;
		case MotionEvent::E_ACTION_MOVE://触摸滑动
			break;
		case MotionEvent::E_ACTION_UP:  //触摸抬起
			break;
		default:
			break;
	}
	return false;
}


static int timea = 0;
 static bool ongcodeActivityTouchEvent(const MotionEvent &ev) {

}

static S_ACTIVITY_TIMEER REGISTER_ACTIVITY_TIMER_TAB[] = {  // Timers used for updating tasks
 	{0,  1000},//定时器id=0, // 1000ms so every 1 second
 	{1,  600}, // 600ms
 	{2,  50}, // 50ms
 	{3,  3000}, // 3000ms
};

int can_in=1;

static bool onUI_Timer(int id){
	if(id == 0){
		Hardware_serial_transmission("M409\r\n");
	}
}

