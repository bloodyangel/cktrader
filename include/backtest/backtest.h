#pragma once
#ifndef __BACKTEST_H__
#define __BACKTEST_H__

#include "utils/ckdef.h"
#include "utils/cktypes.h"
#include "utils/dllHelper.h"
#include "servicemgr_def.h"
#include "strategy/strategy_def.h"
#include "dataloader_def.h"

namespace cktrader {

typedef IStrategy* (*CreateStrategy)(IServiceMgr*, const char*);
typedef int(*ReleaseStrategy)(IStrategy*p);

class CK_EXPORTS BackTest:public IServiceMgr
{
public:
	BackTest();
	BackTest(BackTest& b);
	~BackTest();

	IGateway* getGateWay(std::string gateWayName);//获取backtestgateway	

	void setSlippage(double turn);//设置跳数
	void setChargeRate(double rate);//设置交易成本
	void setContractSize(int s);//设置合约大小

	bool runBacktesting(std::string begin, std::string end, std::string datapath, std::string type, std::string name, std::string strategypath);

	void getBacktestingResult();

protected:
	bool initGateway();
	bool initDataSource(std::string begin, std::string end, std::string path, std::string type);
	bool initStrategy(std::string name, std::string path);
	IStrategy* loadStrategy(std::string name, std::string path);//从dll动态加载strategy

	void calculateBacktestingResult(Datablk&  data);

private:
	IGateway* backtest_gate = nullptr;

	CDllHelper* dll_backtest_strategy = nullptr;
	IStrategy* backtest_strategy = nullptr;

	DataLoader* datasource = nullptr;

	double slippage = 0.2;
	double  chargeRate = 0.0003;
	int contractSize = 300;
};

}//cktrader

#endif
