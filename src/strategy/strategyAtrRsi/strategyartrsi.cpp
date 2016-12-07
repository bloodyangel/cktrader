#include "strategyartrsi.h"

#include <assert.h>
#include <iostream>
#include <functional>
#include <thread>


StrategyAtrRsi::StrategyAtrRsi(IServiceMgr* x, const char* name)
{
	pctp_Gateway = x->getGateWay("ctp");
	plts_Gateway = x->getGateWay("lts");
	pEvent = x->getEventEngine();

	console = stdout_color_mt("console");
	rotating_log = rotating_logger_mt("artrsi", "mylogfile", 1048576 * 2, 3);
}

StrategyAtrRsi::~StrategyAtrRsi()
{

}

bool StrategyAtrRsi::onInit()
{
	pEvent->registerHandler(EVENT_TICK, std::bind(&StrategyAtrRsi::onTick, this, std::placeholders::_1));
	pEvent->registerHandler(EVENT_ORDER, std::bind(&StrategyAtrRsi::onOrder, this, std::placeholders::_1));
	pEvent->registerHandler(EVENT_TRADE, std::bind(&StrategyAtrRsi::onTrade, this, std::placeholders::_1));
	pEvent->registerHandler(EVENT_LOG, std::bind(&StrategyAtrRsi::onLog, this, std::placeholders::_1));
	pEvent->registerHandler(EVENT_TIMER, std::bind(&StrategyAtrRsi::timer, this, std::placeholders::_1));

	std::this_thread::sleep_for(std::chrono::seconds(20));
	SubscribeReq sub1;
	sub1.symbol = "rb1701,IF1703,cu1702,al1702,zn1701,ni1703,c1703,SR703,CF701,CF703,CF707,FG612,FG703,JR701,LR701,MA701,OI701,PM707";
	pctp_Gateway->subscribe(sub1);

	SubscribeReq sub2;
	sub2.symbol = "000002,000001";
	sub2.exchange = "SZE";
	plts_Gateway->subscribe(sub2);
	console->info("rsi:: start");
	rotating_log->info("rsi:: start");
	return true;
}

bool StrategyAtrRsi::onStart()
{
	console->info("rsi:: start");
	rotating_log->info("rsi:: start");
	return true;
}

bool StrategyAtrRsi::onStop()
{
	console->info("rsi:: stop");
	rotating_log->info("rsi:: stop");
	return true;
}

void StrategyAtrRsi::onTick(Datablk& tick)
{
	TickData tick_data = tick.cast<TickData>();
	console->info("date{}:rsi:tick:{}:{}",tick_data.date, tick_data.symbol, tick_data.askPrice1);
	rotating_log->info("date{}:rsi:tick:{}:{}", tick_data.date, tick_data.symbol, tick_data.askPrice1);
}

void StrategyAtrRsi::onOrder(Datablk&  order)
{
	console->info("rsi:: order");
}

void StrategyAtrRsi::onTrade(Datablk&  trade)
{
	console->info("rsi:: trade");
}

void StrategyAtrRsi::onLog(Datablk&  log)
{
	LogData log_data = log.cast<LogData>();

	console->info("log{}:{}", log_data.gateWayName, log_data.logContent);
}

void StrategyAtrRsi::timer(Datablk& tick)
{
	console->info("timer");
}