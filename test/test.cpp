
#include "servicemgr_iml.h"
#include <iostream>
#include "utils/ckdef.h"
#include <functional>

using namespace cktrader;


void on_log(Datablk& log)
{
	LogData data_log = log.cast<LogData>();
	std::cout << data_log.gateWayName << " : " << data_log.logContent << std::endl;
}

int main()
{
	ServiceMgr mgr;

#ifdef _DEBUG
	IGateway* ctp_gate = mgr.loadGateWay("ctp", "CTPGateway-D.dll");
	IGateway* lts_gate = mgr.loadGateWay("lts", "LTSGateway-D.dll");
	IGateway* ib_gate = mgr.loadGateWay("ib", "IBGateway-D.dll");
#else
	IGateway* ctp_gate = mgr.loadGateWay("ctp", "CTPGateway.dll");
	IGateway* lts_gate = mgr.loadGateWay("lts", "LTSGateway.dll");
	IGateway* ib_gate = mgr.loadGateWay("ib", "IBGateway.dll");
#endif

	if (!ctp_gate||!lts_gate||!ib_gate)
	{
		std::cout << "loading gateway failed" << std::endl;
		return -1;
	}
	EventEngine *pEvent = mgr.getEventEngine();
	//pEvent->registerHandler(EVENT_LOG, std::bind(on_log, std::placeholders::_1));

	ctp_gate->connect(std::string("036789"),std::string("85399386"));
	lts_gate->connect(std::string("020000000334"), std::string("150601"));

	std::this_thread::sleep_for(std::chrono::seconds(5));

#ifdef _DEBUG
	IStrategy* rsk = mgr.loadStrategy("artris","strategyAtrRsi-D.dll");
#else
	IStrategy* rsk = mgr.loadStrategy("artris", "strategyAtrRsi.dll");
#endif
	if (!rsk)
	{
		std::cout << "loading strategy failed" << std::endl;
		return -2;
	}

	bool isInit = mgr.initStrategy("artris");
	if (!isInit)
	{
		std::cout << "strategy init failed" << std::endl;
		return -3;
	}

	bool isStarted = mgr.startStrategy("artris");
	if (!isStarted)
	{
		std::cout << "strategy start failed" << std::endl;
		return -4;
	}

	bool isStoped = mgr.stopStrategy("artris");
	
	getchar();
	return 0;
}