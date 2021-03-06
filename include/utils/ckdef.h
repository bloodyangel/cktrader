#pragma once
#ifndef __CKDEF_H__
#define __CKDEF_H__

#if (defined WIN32 || defined _WIN32 || defined WINCE || defined __CYGWIN__)&& defined CKAPI_EXPORTS
#  define CK_EXPORTS __declspec(dllexport)
#elif defined __GNUC__ && __GNUC__ >= 4
#  define CK_EXPORTS __attribute__ ((visibility ("default")))
#else
#  define CK_EXPORTS
#endif

#define CK_ERROR -1
#define CK_TRUE 0

#define _FTA(x) x
#define _FTW(x) L#x

#define CAPI_CKTRADER extern "C"

#define CK_CONTRACT_SYMBOL_DELM ","

/*event type*/
#define EVENT_INFO "eInfo."
#define EVENT_STOCK_HQ "eStockHq."
#define EVENT_TIMER "eTimer."
#define EVENT_TICK  "eTick."
#define EVENT_SUB "eSubcribe."
#define EVENT_READY_SUB "eReadySubcribe."
#define EVENT_ERROR "eError."
#define EVENT_LOG "eLog."
#define EVENT_CONTRACT "eContract."
#define EVENT_ACCOUNT "eAccount."
#define EVENT_POSITION "ePosition."
#define EVENT_ORDER "eOrder."
#define EVENT_TRADE "eTrade."
#define EVENT_CANCEL_ORDER "eCancelOrder."
#define EVENT_SEND_ORDER "eSendOrder."
#define EVENT_CONNECT_CTP "eConnectCTP."

#define EVENT_CAL_BACKTEST "eCalBacktest."

#define PRODUCT_EQUITY "股票"
#define PRODUCT_FUTURES "期货"
#define PRODUCT_OPTION "期权"
#define PRODUCT_INDEX "指数"
#define PRODUCT_COMBINATION "组合"
#define PRODUCT_FOREX "外汇"
#define PRODUCT_UNKNOWN "未知"
#define PRODUCT_SPOT "现货"
#define PRODUCT_DEFER "延期"
#define PRODUCT_NONE = ""

#define OPTION_CALL "看涨期权"
#define OPTION_PUT "看跌期权"

#define DIRECTION_NONE "无方向"
#define DIRECTION_LONG "多"
#define DIRECTION_SHORT "空"
#define DIRECTION_UNKNOWN "未知"
#define DIRECTION_NET "净"
#define DIRECTION_SELL "卖出"

#define OFFSET_NONE "无开平"
#define OFFSET_OPEN "开仓"
#define OFFSET_CLOSE "平仓"
#define OFFSET_CLOSETODAY "平今"
#define OFFSET_CLOSEYESTERDAY "平昨"
#define OFFSET_UNKNOWN "未知"

#define STATUS_NOTTRADED "未成交"
#define STATUS_PARTTRADED "部分成交"
#define STATUS_ALLTRADED "全部成交"
#define STATUS_CANCELLED "已撤销"
#define STATUS_UNKNOWN "未知"

#define PRICETYPE_LIMITPRICE "限价"
#define PRICETYPE_MARKETPRICE "市价"
#define PRICETYPE_FAK "FAK"
#define PRICETYPE_FOK "FOK"

#define CKTRADER_SETTING_FILE "cktrader_setting.json"

#define CTP_PATH_COM "\\ctp_temp\\"
#define LTS_PATH_COM "\\lts_temp\\"

//db
#define DB_PATH "./data"


//strategy
#define ST_ORDER_BUY "买开"
#define ST_ORDER_SELL "卖平"
#define ST_ORDER_SHORT "卖开"
#define ST_ORDER_COVER "买平"

#define STOPORDER_WAITING "等待中"
#define STOPORDER_CANCELLED "已撤销"
#define STOPORDER_TRIGGERED "已触发"

#define STOPORDERPREFIX "StopOrder."

//交易所类型
#define EXCHANGE_SSE "SSE"       //上交所
#define EXCHANGE_SZSE "SZSE"     // 深交所
#define EXCHANGE_CFFEX "CFFEX"   // 中金所
#define EXCHANGE_SHFE "SHFE"   //上期所
#define EXCHANGE_CZCE "CZCE"     //郑商所
#define EXCHANGE_DCE "DCE"      //大商所
#define EXCHANGE_SGE "SGE"     //上金所
#define EXCHANGE_UNKNOWN "UNKNOWN" // 未知交易所
#define EXCHANGE_NONE ""         // 空交易所
#define EXCHANGE_HKEX "HKEX"   //港交所

#define EXCHANGE_SMART "SMART"	// IB智能路由（股票、期权）
#define EXCHANGE_NYMEX "NYMEX" //       # IB 期货
#define EXCHANGE_GLOBEX "GLOBEX" //     # CME电子交易平台
#define EXCHANGE_IDEALPRO "IDEALPRO"  // # IB外汇ECN

// 货币类型
#define CURRENCY_USD "USD"            //美元
#define CURRENCY_CNY "CNY"            // 人民币
#define CURRENCY_UNKNOWN "UNKNOWN"    // 未知货币
#define CURRENCY_NONE ""              // 空货币

#endif