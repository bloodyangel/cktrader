#pragma once
#ifndef __LTSGATEWAY_H__
#define __LTSGATEWAY_H__

#include <string>
#include <mutex>

#include "utils/ckdef.h"
#include "utils/cktypes.h"
#include "gateway/tgateway_def.h"

#include <atomic>

namespace cktrader {

#define CKTRADER_LTS_FILE "lts_setting.json"

	CAPI_CKTRADER
	{
		CK_EXPORTS  IGateway* CreateGateway(EventEngine* pEvent,const char* x);
		CK_EXPORTS int ReleaseGateway(IGateway*p);
	}

	class EventEngine;
	class LtsMd;
	class LtsTd;
	class LtsQry;

	class LTSGateway :public IGateway
	{
	public:
		LTSGateway(EventEngine* pEvent,std::string gateWayName = "lts");
		~LTSGateway();

		std::string getName();
		EventEngine* getEventEngine();

		void connect(std::string& userID, std::string& password);
		void subscribe(SubscribeReq& subReq);
		std::string sendOrder(OrderReq& orderReq);
		void cancelOrder(CancelOrderReq& cancelOrderReq);
		void qryAccount();
		void qryPosition();
		void close();

		std::string readFile(std::string fileName);

		void query(Datablk& notUse);

		std::atomic<bool> mdConnected = false;
		std::atomic<bool> tdConnected = false;
		std::atomic<bool> qryConnected = false;

	private:
		EventEngine *m_event_service;
		std::string gateWayName;
		LtsMd *md;
		LtsTd *td;
		LtsQry *qry;

		int qryCount = 0;          // 查询触发倒计时
		int qryTrigger = 2;        //查询触发点

		mutable std::recursive_mutex the_mutex;
	};

}//cktrader
#endif