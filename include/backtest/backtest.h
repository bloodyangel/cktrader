#pragma once
#ifndef __BACKTEST_H__
#define __BACKTEST_H__

#include "utils/ckdef.h"
#include "utils/cktypes.h"
#include "utils/dllHelper.h"
#include "servicemgr_def.h"
#include "strategy/strategy_def.h"

namespace cktrader {

class CK_EXPORTS BackTest:public IServiceMgr
{
public:
	BackTest();
	BackTest(BackTest& b);
	~BackTest();

	virtual IGateway* getGateWay(std::string gateWayName = "backtest");//获取backtestgateway

	IStrategy* loadStrategy(std::string name, std::string path);//从dll动态加载strategy
	bool initStrategy(std::string strategyName);
	bool startStrategy(std::string strategyName);
	bool stopStrategy(std::string strategyName);

	void setSlippage(double turn);
	void setRate(double rate);
	void setSize(double s);

	void getExchageRecord();
	void saveExchangeRecord();

	void calculateBacktestingResult();
	void showBacktestingResult();

private:
	CDllHelper* dll_backtest_gate = nullptr;
	IGateway* backtest_gate = nullptr;
	IStrategy* backtest_strategy = nullptr;
};

}//cktrader

#endif
