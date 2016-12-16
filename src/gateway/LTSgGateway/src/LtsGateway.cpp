#include "LTSGateway.h"
#include "ltsmd.h"
#include "ltstd.h"
#include "ltsqry.h"
#include "eventservice/eventengine.h"
#include "utils/json11.hpp"

#include <fstream> 
#include <sstream>

namespace cktrader {

	LTSGateway::LTSGateway(EventEngine* pEvent,std::string gateWayName):IGateway(pEvent)
	{
		this->m_event_service = pEvent;

		mdConnected = false;
		tdConnected = false;
		qryConnected = false;

		this->gateWayName = gateWayName;

		md = new LtsMd(m_event_service, this);
		td = new LtsTd(m_event_service, this);
		qry = new LtsQry(m_event_service, this);

	}
	LTSGateway::~LTSGateway()
	{
		mdConnected = false;
		tdConnected = false;

		md->close();
		delete md;
		md = nullptr;

		td->close();
		delete td;
		td = nullptr;

		qry->close();
		delete qry;
		qry = nullptr;
	}

	std::string LTSGateway::getName()
	{
		return gateWayName;
	}

	EventEngine* LTSGateway::getEventEngine()
	{
		return m_event_service;
	}

	void LTSGateway::connect(std::string& userID,std::string& password)
	{
		std::string data = readFile(CKTRADER_LTS_FILE);

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
		std::string qryAddress = setting_json["qryAddress"].string_value();
		std::string productInfo = setting_json["productInfo"].string_value();
		std::string authCode = setting_json["authCode"].string_value();
		std::string mdPassword = setting_json["mdPassword"].string_value();

		md->connect(userID, mdPassword, brokerID, mdAddress);
		td->connect(userID, password, brokerID, tdAddress,productInfo,authCode);
		qry->connect(userID, password, brokerID, tdAddress, productInfo, authCode);

		m_event_service->registerHandler(EVENT_TIMER, std::bind(&LTSGateway::query, this, std::placeholders::_1), gateWayName);
	}

	void LTSGateway::subscribe(SubscribeReq& subReq)
	{
		md->subscribe(subReq);
	}

	std::string LTSGateway::sendOrder(OrderReq& orderReq)
	{
		return td->sendOrder(orderReq);
	}

	void LTSGateway::cancelOrder(CancelOrderReq& cancelOrderReq)
	{
		td->cancelOrder(cancelOrderReq);
	}

	void LTSGateway::qryAccount()
	{
		if (qryConnected)
		{
			qry->qryAccount();
		}
	}
	void LTSGateway::qryPosition()
	{
		if (qryConnected)
		{
			qry->qryPosition();
		}
	}
	void LTSGateway::close()
	{
		if (mdConnected)
		{
			md->close();
		}
		if (tdConnected)
		{
			td->close();
		}
		if (qryConnected)
		{
			qry->close();
		}
	}

	void LTSGateway::query(Datablk& notUse)
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

std::string LTSGateway:: readFile(std::string fileName)
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

}//cktrader