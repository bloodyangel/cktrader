#pragma once
#ifndef __STRATEGYARTRSI_H__
#define __STRATEGYARTRSI_H__

#include "strategy/strategy_def.h"
#include "servicemgr_def.h"
#include "gateway/tgateway_def.h"

#include <string>

#include "spdlog/spdlog.h"

using namespace spdlog;

using namespace cktrader;

CAPI_CKTRADER
{
	CK_EXPORTS IStrategy* CreateStrategy(IServiceMgr*, const char*);
	CK_EXPORTS int ReleaseStrategy(IStrategy*p);
}

class CK_EXPORTS StrategyAtrRsi :public IStrategy
{
public:
	StrategyAtrRsi(IServiceMgr* x, const char* name);
	~StrategyAtrRsi();
	bool onInit();
	bool onStart();
	bool onStop();

	void onTick(Datablk& tick);
	void onOrder(Datablk&  order);
	void onTrade(Datablk&  trade);
	void onLog(Datablk&  log);
	void timer(Datablk& tick);

private:
	IGateway * pctp_Gateway = nullptr;
	IGateway * plts_Gateway = nullptr;
	EventEngine* pEvent = nullptr;
	std::shared_ptr<spdlog::logger> console;
	std::shared_ptr<spdlog::logger> rotating_log;
};

#endif