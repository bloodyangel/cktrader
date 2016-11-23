#include "backtest/backtest.h"
#include "backtest/backtestgate.h"
#include "backtest/dataloader_iml_csv.h"

namespace cktrader{

BackTest::BackTest()
{

}

BackTest::BackTest(BackTest& b)
{
	this->backtest_gate = b.backtest_gate;
	this->backtest_strategy = b.backtest_strategy;
	this->dll_backtest_strategy = b.dll_backtest_strategy;

	this->datasource = b.datasource;

	this->slippage = b.slippage;
	this->chargeRate = b.chargeRate;
	this->contractSize = b.contractSize;
}
BackTest::~BackTest()
{
	if (backtest_gate)
	{
		delete backtest_gate;
		backtest_gate = nullptr;
	}

	if (backtest_strategy)
	{
		delete backtest_strategy;
		backtest_strategy = nullptr;
	}

	if (dll_backtest_strategy)
	{
		delete dll_backtest_strategy;
		dll_backtest_strategy = nullptr;
	}

	if (datasource)
	{
		delete datasource;
		datasource = nullptr;
	}
}

bool BackTest::initGateway()
{
	//生成gateway
	if (!backtest_gate)
	{
		backtest_gate = new BacktestGate();
		EventEngine* pEvent = backtest_gate->getEventEngine();
		pEvent->registerHandler(EVENT_CAL_BACKTEST, std::bind(&BackTest::calculateBacktestingResult, this, std::placeholders::_1));
		return true;
	}

	return false;
}

bool BackTest::initDataSource(std::string begin, std::string end, std::string path, std::string type)
{
	if (!datasource)
	{
		if (type == "csv")
		{
			datasource = new DataLoaderCsv(backtest_gate);
			datasource->setStartDate(begin);
			datasource->setEndDate(end);
			return true;
		}
	}
	
	return false;
}

bool BackTest::initStrategy(std::string name, std::string path)
{
	IStrategy* s = loadStrategy(name, path);
	if (!s)
	{
		return false;
	}

	backtest_strategy->onInit();
	backtest_strategy->onStart();

	return true;
}

IGateway* BackTest::getGateWay(std::string gateWayName)//获取backtestgateway
{
	return backtest_gate;
}

IStrategy* BackTest::loadStrategy(std::string name, std::string path)//从dll动态加载strategy
{
	dll_backtest_strategy = new CDllHelper(_FTA(path.c_str()));

	CreateStrategy pfunc = dll_backtest_strategy->GetProcedure<CreateStrategy>("CreateStrategy");
	if (pfunc)
	{
		backtest_strategy = pfunc(this, name.c_str());

		return backtest_strategy;
	}

	return nullptr;
}

void BackTest::setSlippage(double turn)
{
	this->slippage = turn;
}

void BackTest::setChargeRate(double rate)
{
	this->chargeRate = rate;
}

void BackTest::setContractSize(int s)
{
	this->contractSize = s;
}

bool BackTest::runBacktesting(std::string begin, std::string end, std::string datapath, std::string type, std::string name, std::string strategypath)
{
	bool initgate = initGateway();

	if (!initgate)
	{
		return false;
	}

	bool initdata = initDataSource(begin,end,datapath,type);
	bool initstrategy = initStrategy(name,strategypath);

	if (!initdata || !initstrategy)
	{
		return false;
	}

	datasource->startLoadData();
}

void BackTest::calculateBacktestingResult(Datablk& data)
{

}

void BackTest::getBacktestingResult()
{

}

}//cktrader