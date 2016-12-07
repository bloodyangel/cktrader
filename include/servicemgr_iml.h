#pragma once
#ifndef __SERVICEMGR_H__
#define __SERVICEMGR_H__

#include "utils/ckdef.h"
#include "utils/cktypes.h"
#include "gateway/tgateway_def.h"
#include "utils/dllHelper.h"
#include "strategy/strategy_def.h"
#include "servicemgr_def.h"

namespace cktrader {

typedef IGateway* (*CreateGateway)(EventEngine* pEvent,const char*);
typedef int(*ReleaseGateway)(IGateway*p);

typedef IStrategy* (*CreateStrategy)(IServiceMgr*, const char*);
typedef int(*ReleaseStrategy)(IStrategy*p);

class CK_EXPORTS ServiceMgr:public IServiceMgr
{
public:
	ServiceMgr();
	ServiceMgr(ServiceMgr& mgr);
	~ServiceMgr();

	IGateway* loadGateWay(std::string name, std::string path);//��dll��̬����gateway

	IStrategy* loadStrategy(std::string name, std::string path);//��dll��̬����strategy

	virtual IGateway* getGateWay(std::string gateWayName);//�ӱ����ֵ��ڻ�ȡgateway

	IStrategy* getStrategy(std::string strategyName);//�ӱ����ֵ��ȡstrategy
	bool initStrategy(std::string strategyName);
	bool startStrategy(std::string strategyName);
	bool stopStrategy(std::string strategyName);

	virtual EventEngine* getEventEngine();

	std::map<std::string, IGateway*>* getGatewayMap();
	std::map<std::string, IStrategy*>* getStrategyMap();

protected:
	bool readFile(std::string fileName, std::stringstream& stringText);
	bool writeFile(std::string fileName, std::stringstream& stringText);

private:
	std::map<std::string, CDllHelper*> *m_DLLMap = nullptr;//װ�ض�̬���ص�dll

	std::map<std::string, IGateway*> *m_GateWayMap = nullptr;//װ��gateway
	
	std::map<std::string, IStrategy*> *m_StrategyMap = nullptr;//װ��strategy

	EventEngine* m_eventEngine = nullptr;

	mutable std::recursive_mutex the_mutex;
};

}//cktrader

#endif