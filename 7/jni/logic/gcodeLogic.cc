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
#include <ObjectModel/PrinterStatus.hpp>
//#include "evc.h"

#define ARRAY_SIZE(arr) (sizeof(arr)/sizeof(arr[0]))

void Hardware_serial_transmission(const std::string& data) ;

/**
 * 当界面构造时触发
 */

#define DEBUG	(0) // 0: off, 1: MessageLog, 2: Uart
#include "Debug.hpp"

#define DEBUG2	(0) // 0: off, 1: DebugField
#if (DEBUG2)

#define STRINGIFY(x)	#x
#define TOSTRING(x)	STRINGIFY(x)
#define dbg2(fmt, args...)		debugField->SetValue(TOSTRING(__LINE__)); debugField->Refresh(true, 0, 0)

#else
#define dbg2(fmt, args...)		do {} while(0)

#endif

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

enum ReceivedDataEvent
{
	rcvUnknown = 0,

	// Keys for control command messages
	rcvControlCommand,

	// Keys for push messages
	rcvPushMessage,
	rcvPushResponse,
	rcvPushSeq,
	rcvPushBeepDuration,
	rcvPushBeepFrequency,

	// Keys for M20 response
	rcvM20Dir,
	rcvM20Err,
	rcvM20Files,

	// Keys for M36 respons
	rcvM36Filament,
	rcvM36Filename,
	rcvM36GeneratedBy,
	rcvM36Height,
	rcvM36LastModified,
	rcvM36LayerHeight,
	rcvM36PrintTime,
	rcvM36SimulatedTime,
	rcvM36Size,
	rcvM36ThumbnailsFormat,
	rcvM36ThumbnailsHeight,
	rcvM36ThumbnailsOffset,
	rcvM36ThumbnailsSize,
	rcvM36ThumbnailsWidth,

	rcvM361ThumbnailData,
	rcvM361ThumbnailErr,
	rcvM361ThumbnailFilename,
	rcvM361ThumbnailNext,
	rcvM361ThumbnailOffset,

	// Keys for M409 response
	rcvKey,
	rcvFlags,
	rcvResult,

	// Available keys
	rcvOMKeyBoards,
	rcvOMKeyDirectories,
	rcvOMKeyFans,
	rcvOMKeyHeat,
	rcvOMKeyInputs,
	rcvOMKeyJob,
	rcvOMKeyLimits,
	rcvOMKeyMove,
	rcvOMKeyNetwork,
	rcvOMKeyReply,
	rcvOMKeyScanner,
	rcvOMKeySensors,
	rcvOMKeySeqs,
	rcvOMKeySpindles,
	rcvOMKeyState,
	rcvOMKeyTools,
	rcvOMKeyVolumes,

	// Keys for boards response
	rcvBoardsFirmwareName,

	// Keys for fans response
	rcvFansRequestedValue,

	// Keys for heat response
	rcvHeatBedHeaters,
	rcvHeatChamberHeaters,
	rcvHeatHeatersActive,
	rcvHeatHeatersCurrent,
	rcvHeatHeatersStandby,
	rcvHeatHeatersState,

	// Keys for job response
	rcvJobDuration,
	rcvJobFileFilename,
	rcvJobFileSize,
	rcvJobFilePosition,
	rcvJobFileSimulatedTime,
	rcvJobLastFileName,
	rcvJobLastFileSimulated,
	rcvJobTimesLeftFilament,
	rcvJobTimesLeftFile,
	rcvJobTimesLeftSlicer,
	rcvJobWarmUpDuration,

	// Keys for move response
	rcvMoveAxesBabystep,
	rcvMoveAxesHomed,
	rcvMoveAxesLetter,
	rcvMoveAxesMachinePosition,
	rcvMoveAxesUserPosition,
	rcvMoveAxesVisible,
	rcvMoveAxesWorkplaceOffsets,
	rcvMoveExtrudersFactor,
	rcvMoveKinematicsName,
	rcvMoveSpeedFactor,
	rcvMoveWorkplaceNumber,

	// Keys for network response
	rcvNetworkName,
	rcvNetworkInterfacesActualIP,

	// Keys for sensors response
	rcvSensorsProbeValue,

	// Keys for seqs response
	rcvSeqsBoards,
	rcvSeqsDirectories,
	rcvSeqsFans,
	rcvSeqsHeat,
	rcvSeqsInputs,
	rcvSeqsJob,
	rcvSeqsMove,
	rcvSeqsNetwork,
	rcvSeqsReply,
	rcvSeqsScanner,
	rcvSeqsSensors,
	rcvSeqsSpindles,
	rcvSeqsState,
	rcvSeqsTools,
	rcvSeqsVolumes,

	// Keys for spindles respons
	rcvSpindlesActive,
	rcvSpindlesCurrent,
	rcvSpindlesMax,
	rcvSpindlesMin,
	rcvSpindlesState,
	rcvSpindlesTool,

	// Keys from state response
	rcvStateCurrentTool,
	rcvStateMessageBox,
	rcvStateMessageBoxAxisControls,
	rcvStateMessageBoxMessage,
	rcvStateMessageBoxMode,
	rcvStateMessageBoxSeq,
	rcvStateMessageBoxTimeout,
	rcvStateMessageBoxTitle,
	rcvStateStatus,
	rcvStateUptime,

	// Keys from tools response
	rcvToolsActive,
	rcvToolsExtruders,
	rcvToolsFans,
	rcvToolsHeaters,
	rcvToolsOffsets,
	rcvToolsNumber,
	rcvToolsSpindle,
	rcvToolsSpindleRpm,
	rcvToolsStandby,
	rcvToolsState,
};

struct FieldTableEntry
{
	const ReceivedDataEvent val;
	const char* key;
};

// The following tables will be sorted once on startup so entries can be better grouped for code maintenance
// A '^' character indicates the position of an _ecv_array index, and a ':' character indicates the start of a sub-field name
static FieldTableEntry fieldTable[] =
{
	// M409 common fields
	{ rcvKey, 							"key" },

	// M409 K"boards" response
	{ rcvBoardsFirmwareName, 			"boards^:firmwareName" },

	// M409 K"fans" response
	{ rcvFansRequestedValue,			"fans^:requestedValue" },

	// M409 K"heat" response
	{ rcvHeatBedHeaters,				"heat:bedHeaters^" },
	{ rcvHeatChamberHeaters,			"heat:chamberHeaters^" },
	{ rcvHeatHeatersActive,				"heat:heaters^:active" },
	{ rcvHeatHeatersCurrent,			"heat:heaters^:current" },
	{ rcvHeatHeatersStandby,			"heat:heaters^:standby" },
	{ rcvHeatHeatersState,				"heat:heaters^:state" },

	// M409 K"job" response
	{ rcvJobFileFilename, 				"job:file:fileName" },
	{ rcvJobFileSize, 					"job:file:size" },
	{ rcvJobFileSimulatedTime, 			"job:file:simulatedTime" },
	{ rcvJobFilePosition,				"job:filePosition" },
	{ rcvJobLastFileName,				"job:lastFileName" },
	{ rcvJobDuration,				"job:duration" },
	{ rcvJobTimesLeftFilament,			"job:timesLeft:filament" },
	{ rcvJobTimesLeftFile,				"job:timesLeft:file" },
	{ rcvJobTimesLeftSlicer,			"job:timesLeft:slicer" },
	{ rcvJobWarmUpDuration,				"job:warmUpDuration" },

	// M409 K"move" response
	{ rcvMoveAxesBabystep, 				"move:axes^:babystep" },
	{ rcvMoveAxesHomed,					"move:axes^:homed" },
	{ rcvMoveAxesLetter,	 			"move:axes^:letter" },
	{ rcvMoveAxesMachinePosition,		"move:axes^:machinePosition" },
	{ rcvMoveAxesUserPosition,			"move:axes^:userPosition" },
	{ rcvMoveAxesVisible, 				"move:axes^:visible" },
	{ rcvMoveAxesWorkplaceOffsets, 		"move:axes^:workplaceOffsets^" },
	{ rcvMoveExtrudersFactor, 			"move:extruders^:factor" },
	{ rcvMoveKinematicsName, 			"move:kinematics:name" },
	{ rcvMoveSpeedFactor, 				"move:speedFactor" },
	{ rcvMoveWorkplaceNumber, 			"move:workplaceNumber" },

	// M409 K"network" response
	{ rcvNetworkName, 					"network:name" },
	{ rcvNetworkInterfacesActualIP,		"network:interfaces^:actualIP" },

	// M409 K"sensors" response
	{ rcvSensorsProbeValue,				"sensors:probes^:value^" },

	// M409 K"seqs" response
	{ rcvSeqsBoards,					"seqs:boards" },
	{ rcvSeqsDirectories,				"seqs:directories" },
	{ rcvSeqsFans,						"seqs:fans" },
	{ rcvSeqsHeat,						"seqs:heat" },
	{ rcvSeqsInputs,					"seqs:inputs" },
	{ rcvSeqsJob,						"seqs:job" },
	{ rcvSeqsMove,						"seqs:move" },
	{ rcvSeqsNetwork,					"seqs:network" },
	{ rcvSeqsReply,						"seqs:reply" },
	{ rcvSeqsScanner,					"seqs:scanner" },
	{ rcvSeqsSensors,					"seqs:sensors" },
	{ rcvSeqsSpindles,					"seqs:spindles" },
	{ rcvSeqsState,						"seqs:state" },
	{ rcvSeqsTools,						"seqs:tools" },
	{ rcvSeqsVolumes,					"seqs:volumes" },

	// M409 K"spindles" response
	{ rcvSpindlesActive, 				"spindles^:active" },
	{ rcvSpindlesCurrent,				"spindles^:current" },
	{ rcvSpindlesMax, 					"spindles^:max" },
	{ rcvSpindlesMin, 					"spindles^:min" },
	{ rcvSpindlesState, 				"spindles^:state" },
	{ rcvSpindlesTool,	 				"spindles^:tool" },

	// M409 K"state" response
	{ rcvStateCurrentTool,				"state:currentTool" },
	{ rcvStateMessageBox,				"state:messageBox" },
	{ rcvStateMessageBoxAxisControls,	"state:messageBox:axisControls" },
	{ rcvStateMessageBoxMessage,		"state:messageBox:message" },
	{ rcvStateMessageBoxMode,			"state:messageBox:mode" },
	{ rcvStateMessageBoxSeq,			"state:messageBox:seq" },
	{ rcvStateMessageBoxTimeout,		"state:messageBox:timeout" },
	{ rcvStateMessageBoxTitle,			"state:messageBox:title" },
	{ rcvStateStatus,					"state:status" },
	{ rcvStateUptime,					"state:upTime" },

	// M409 K"tools" response
	{ rcvToolsActive, 					"tools^:active^" },
	{ rcvToolsExtruders,				"tools^:extruders^" },
	{ rcvToolsFans,						"tools^:fans^" },
	{ rcvToolsHeaters,					"tools^:heaters^" },
	{ rcvToolsNumber, 					"tools^:number" },
	{ rcvToolsOffsets, 					"tools^:offsets^" },
	{ rcvToolsSpindle, 					"tools^:spindle" },
	{ rcvToolsSpindleRpm,				"tools^:spindleRpm" },
	{ rcvToolsStandby, 					"tools^:standby^" },
	{ rcvToolsState, 					"tools^:state" },

	// M20 response
	{ rcvM20Dir,						"dir" },
	{ rcvM20Err,						"err" },
	{ rcvM20Files,						"files^" },

	// M36 response
	{ rcvM36Filament,					"filament^" },
	{ rcvM36Filename,					"fileName" },
	{ rcvM36GeneratedBy,				"generatedBy" },
	{ rcvM36Height,						"height" },
	{ rcvM36LastModified,				"lastModified" },
	{ rcvM36LayerHeight,				"layerHeight" },
	{ rcvM36PrintTime,					"printTime" },
	{ rcvM36SimulatedTime,				"simulatedTime" },
	{ rcvM36Size,						"size" },
	{ rcvM36ThumbnailsFormat,			"thumbnails^:format" },
	{ rcvM36ThumbnailsHeight,			"thumbnails^:height" },
	{ rcvM36ThumbnailsOffset,			"thumbnails^:offset" },
	{ rcvM36ThumbnailsSize,				"thumbnails^:size" },
	{ rcvM36ThumbnailsWidth,			"thumbnails^:width" },

	{ rcvM361ThumbnailData,				"thumbnail:data" },
	{ rcvM361ThumbnailErr,				"thumbnail:err" },
	{ rcvM361ThumbnailFilename,			"thumbnail:fileName" },
	{ rcvM361ThumbnailNext,				"thumbnail:next" },
	{ rcvM361ThumbnailOffset,			"thumbnail:offset" },

	// Push messages
	{ rcvPushMessage,					"message" },
	{ rcvPushResponse,					"resp" },
	{ rcvPushSeq,						"seq" },
	{ rcvPushBeepDuration,				"beep_length" },
	{ rcvPushBeepFrequency,				"beep_freq" },

	// Control Command message
	{ rcvControlCommand,				"controlCommand" },
};


enum SeqState {
	SeqStateInit,
	SeqStateOk,
	SeqStateUpdate,
	SeqStateError,
	SeqStateDisabled
};

static struct Seq {
	const ReceivedDataEvent event;
	const ReceivedDataEvent seqid;

	uint16_t lastSeq;
	enum SeqState state;

	const char * const key;
	const char * const flags;
} seqs[] = {
#if FETCH_NETWORK
	{ .event = rcvOMKeyNetwork, .seqid = rcvSeqsNetwork, .lastSeq = 0, .state = SeqStateInit, .key = "network", .flags = "v" },
#endif
#if FETCH_BOARDS
	{ .event = rcvOMKeyBoards, .seqid = rcvSeqsBoards, .lastSeq = 0, .state = SeqStateInit, .key = "boards", .flags = "v" },
#endif
#if FETCH_MOVE
	{ .event = rcvOMKeyMove, .seqid = rcvSeqsMove, .lastSeq = 0, .state = SeqStateInit, .key = "move", .flags = "v" },
#endif
#if FETCH_HEAT
	{ .event = rcvOMKeyHeat, .seqid = rcvSeqsHeat, .lastSeq = 0, .state = SeqStateInit, .key = "heat", .flags = "v" },
#endif
#if FETCH_TOOLS
	{ .event = rcvOMKeyTools, .seqid = rcvSeqsTools, .lastSeq = 0, .state = SeqStateInit, .key = "tools", .flags = "v" },
#endif
#if FETCH_SPINDLES
	{ .event = rcvOMKeySpindles, .seqid = rcvSeqsSpindles, .lastSeq = 0, .state = SeqStateInit, .key = "spindles", .flags = "v" },
#endif
#if FETCH_DIRECTORIES
	{ .event = rcvOMKeyDirectories, .seqid = rcvSeqsDirectories, .lastSeq = 0, .state = SeqStateInit, .key = "directories", .flags = "v" },
#endif
#if FETCH_FANS
	{ .event = rcvOMKeyFans, .seqid = rcvSeqsFans, .lastSeq = 0, .state = SeqStateInit, .key = "fans", .flags = "v" },
#endif
#if FETCH_INPUTS
	{ .event = rcvOMKeyInputs, .seqid = rcvSeqsInputs, .lastSeq = 0, .state = SeqStateInit, .key = "inputs", .flags = "v" },
#endif
#if FETCH_JOB
	{ .event = rcvOMKeyJob, .seqid = rcvSeqsJob, .lastSeq = 0, .state = SeqStateInit, .key = "job", .flags = "v" },
#endif
#if FETCH_SCANNER
	{ .event = rcvOMKeyScanner, .seqid = rcvSeqsScanner, .lastSeq = 0, .state = SeqStateInit, .key = "scanner", .flags = "v" },
#endif
#if FETCH_SENSORS
	{ .event = rcvOMKeySensors, .seqid = rcvSeqsSensors, .lastSeq = 0, .state = SeqStateInit, .key = "sensors", .flags = "v" },
#endif
#if FETCH_STATE
	{ .event = rcvOMKeyState, .seqid = rcvSeqsState, .lastSeq = 0, .state = SeqStateInit, .key = "state", .flags = "vn" },
#endif
#if FETCH_VOLUMES
	{ .event = rcvOMKeyVolumes, .seqid = rcvSeqsVolumes, .lastSeq = 0, .state = SeqStateInit, .key = "volumes", .flags = "v" },
#endif
};

static struct Seq *currentReqSeq = NULL;
static struct Seq *currentRespSeq = NULL;

static struct Seq* GetNextSeq(struct Seq *current)
{
	if (current == NULL)
	{
		current = seqs;
	}


	for (size_t i = current - seqs; i < ARRAY_SIZE(seqs); ++i)
	{
		current = &seqs[i];
		if (current->state == SeqStateError)
		{
			// skip and re-init if last request had an error
			current->state = SeqStateInit;
			continue;
		}
		if (current->state == SeqStateInit || current->state == SeqStateUpdate)
		{
			return current;
		}
	}


	return NULL;
}

static struct Seq *FindSeqByKey(const char *key)
{
	dbg("key %s\n", key);

	for (size_t i = 0; i < ARRAY_SIZE(seqs); ++i)
	{
		if (strcasecmp(seqs[i].key, key) == 0)
		{
			return &seqs[i];
		}

	}

	return NULL;
}

static void UpdateSeq(const ReceivedDataEvent seqid, int32_t val)
{
	for (size_t i = 0; i < ARRAY_SIZE(seqs); ++i)
	{
		if (seqs[i].seqid == seqid)
		{
			if (seqs[i].lastSeq != val)
			{
				dbg("%s %d -> %d\n", seqs[i].key, seqs[i].lastSeq, val);
				seqs[i].lastSeq = val;
				seqs[i].state = SeqStateUpdate;
			}
		}
	}
}

static void ResetSeqs()
{
	for (size_t i = 0; i < ARRAY_SIZE(seqs); ++i)
	{
		seqs[i].lastSeq = 0;
		seqs[i].state = SeqStateInit;
	}
}

struct FileList
{
	int listNumber;
	size_t scrollOffset;
	String<100> path;
};

void Delay(uint32_t milliSeconds)
{
	const uint32_t now = SystemTick::GetTickCount();
	while (SystemTick::GetTickCount() - now < milliSeconds) { }
}

bool IsPrintingStatus(OM::PrinterStatus status)
{
	return status == OM::PrinterStatus::printing
			|| status == OM::PrinterStatus::paused
			|| status == OM::PrinterStatus::pausing
			|| status == OM::PrinterStatus::resuming
			|| status == OM::PrinterStatus::simulating;
}

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


