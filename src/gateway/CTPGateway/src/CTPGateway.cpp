#include "CTPGateway.h"
#include "ctpmd.h"
#include "ctptd.h"
#include "eventservice/eventengine.h"
#include "utils/json11.hpp"

#include <fstream> 
#include <sstream>

namespace cktrader {

CTPGateWay::CTPGateWay(EventEngine* pEvent,std::string gateWayName):IGateway(pEvent)
{
	this->m_event_service = pEvent;

	mdConnected = false;
	tdConnected = false;

	this->gateWayName = gateWayName;

	md = new CtpMd(m_event_service, this);
	td = new CtpTd(m_event_service, this);
}
CTPGateWay::~CTPGateWay()
{
	md->close();
	delete md;
	md = nullptr;

	td->close();
	delete td;
	td = nullptr;
}

std::string CTPGateWay::getName()
{
	return gateWayName;
}

EventEngine* CTPGateWay::getEventEngine()
{
	return m_event_service;
}

void CTPGateWay::connect(std::string& userID,std::string& password)
{
	std::string data = readFile(CKTRADER_CTP_FILE);

	if (data.empty())
	{
		return;
	}

	std::string err;
	auto setting_json = json11::Json::parse(data, err);
	if (!err.empty())
	{
		return;
	}

	std::string brokerID = setting_json["brokerID"].string_value();
	std::string mdAddress = setting_json["mdAddress"].string_value();
	std::string tdAddress = setting_json["tdAddress"].string_value();
	md->connect(userID, password, brokerID, mdAddress);
	td->connect(userID, password, brokerID, tdAddress);

	m_event_service->registerHandler(EVENT_TIMER,std::bind(&CTPGateWay::query, this, std::placeholders::_1),gateWayName);
}

void CTPGateWay::subscribe(SubscribeReq& subReq)
{
	md->subscribe(subReq);
}

std::string CTPGateWay::sendOrder(OrderReq& orderReq)
{
	return td->sendOrder(orderReq);
}

void CTPGateWay::cancelOrder(CancelOrderReq& cancelOrderReq)
{
	td->cancelOrder(cancelOrderReq);
}

void CTPGateWay::qryAccount()
{
	if (tdConnected)
	{
		td->qryAccount();
	}
}
void CTPGateWay::qryPosition()
{
	if (tdConnected)
	{
		td->qryPosition();
	}
}
void CTPGateWay::close()
{
	if (mdConnected)
	{
		md->close();
	}
	if (tdConnected)
	{
		td->close();
	}
}

void CTPGateWay::query(Datablk& notUse)
{
	qryCount++;
	if (qryCount == 1)
	{
		qryAccount();
	}
	if (qryCount == 2)
	{
		qryPosition();
	}

	if (qryCount>qryTrigger)
	{
		qryCount = 0;
	}
}

std::string CTPGateWay:: readFile(std::string fileName)
{
	std::ifstream is(fileName, std::ifstream::binary);
	std::string ret("");

	if (is)
	{
		// get length of file:
		is.seekg(0, is.end);
		int length = is.tellg();
		is.seekg(0, is.beg);

		char * buffer = new char[length+1];
		memset(buffer, 0, length + 1);

		// read data as a block:
		is.read(buffer, length);

		if (is)
		{
			ret = std::string(buffer);
		}

		is.close();

		// ...buffer contains the entire file...
		delete[] buffer;
	}
	return ret;
}

void CTPGateWay::writeLog(std::string logInfo)
{
	LogData data;
	data.gateWayName = gateWayName;
	data.logContent = logInfo;
	onLog(data);
}

}//cktrader