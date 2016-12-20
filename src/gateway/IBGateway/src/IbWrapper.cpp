#include "IbWrapper.h"
#include "IBApi.h"

namespace cktrader {

void IBWrapper::tickPrice(TickerId tickerId, TickType field, double price, int canAutoExecute)
{
	this->api->tickPrice(tickerId, field, price, canAutoExecute);
}

void IBWrapper::tickSize(TickerId tickerId, TickType field, int size)
{
	this->api->tickSize(tickerId, field, size);
};

void IBWrapper::tickOptionComputation(TickerId tickerId, TickType tickType, double impliedVol, double delta, double optPrice, double pvDividend, double gamma, double vega, double theta, double undPrice)
{
	this->api->tickOptionComputation(tickerId, tickType, impliedVol, delta, optPrice, pvDividend, gamma, vega, theta, undPrice);
};

void IBWrapper::tickGeneric(TickerId tickerId, TickType tickType, double value)
{
	this->api->tickGeneric(tickerId, tickType, value);
};

void IBWrapper::tickString(TickerId tickerId, TickType tickType, const std::string& value)
{
	this->api->tickString(tickerId, tickType, value);
};

void IBWrapper::tickEFP(TickerId tickerId, TickType tickType, double basisPoints, const std::string& formattedBasisPoints, double totalDividends, int holdDays, const std::string& futureLastTradeDate, double dividendImpact, double dividendsToLastTradeDate)
{
	this->api->tickEFP(tickerId, tickType, basisPoints, formattedBasisPoints, totalDividends, holdDays, futureLastTradeDate, dividendImpact, dividendsToLastTradeDate);
};

void IBWrapper::orderStatus(OrderId orderId, const std::string& status, double filled, double remaining, double avgFillPrice, int permId, int parentId, double lastFillPrice, int clientId, const std::string& whyHeld)
{
	this->api->orderStatus(orderId, status, filled, remaining, avgFillPrice, permId, parentId, lastFillPrice, clientId, whyHeld);
};

void IBWrapper::openOrder(OrderId orderId, const Contract& contract, const Order& order, const OrderState& orderState)
{
	this->api->openOrder(orderId, contract, order, orderState);
};

void IBWrapper::openOrderEnd()
{
	this->api->openOrderEnd();
};

void IBWrapper::winError(const std::string& str, int lastError)
{
	this->api->winError(str, lastError);
};

void IBWrapper::connectionClosed()
{
	this->api->connectionClosed();
};

void IBWrapper::updateAccountValue(const std::string& key, const std::string& val, const std::string& currency, const std::string& accountName)
{
	this->api->updateAccountValue(key, val, currency, accountName);
};

void IBWrapper::updatePortfolio(const Contract& contract, double position, double marketPrice, double marketValue, double averageCost, double unrealizedPNL, double realizedPNL, const std::string& accountName)
{
	this->api->updatePortfolio(contract, position, marketPrice, marketValue, averageCost, unrealizedPNL, realizedPNL, accountName);
};

void IBWrapper::updateAccountTime(const std::string& timeStamp)
{
	this->api->updateAccountTime(timeStamp);
};

void IBWrapper::accountDownloadEnd(const std::string& accountName)
{
	this->api->accountDownloadEnd(accountName);
};

void IBWrapper::nextValidId(OrderId orderId)
{
	this->api->nextValidId(orderId);
};

void IBWrapper::contractDetails(int reqId, const ContractDetails& contractDetails)
{
	this->api->contractDetails(reqId, contractDetails);
};

void IBWrapper::bondContractDetails(int reqId, const ContractDetails& contractDetails)
{
	this->api->bondContractDetails(reqId, contractDetails);
};

void IBWrapper::contractDetailsEnd(int reqId)
{
	this->api->contractDetailsEnd(reqId);
};

void IBWrapper::execDetails(int reqId, const Contract& contract, const Execution& execution)
{
	this->api->execDetails(reqId, contract, execution);
};

void IBWrapper::execDetailsEnd(int reqId)
{
	this->api->execDetailsEnd(reqId);
};

void IBWrapper::error(const int id, const int errorCode, const std::string errorString)
{
	this->api->error(id, errorCode, errorString);
};

void IBWrapper::updateMktDepth(TickerId id, int position, int operation, int side, double price, int size)
{
	this->api->updateMktDepth(id, position, operation, side, price, size);
};

void IBWrapper::updateMktDepthL2(TickerId id, int position, std::string marketMaker, int operation, int side, double price, int size)
{
	this->api->updateMktDepthL2(id, position, marketMaker, operation, side, price, size);
};

void IBWrapper::updateNewsBulletin(int msgId, int msgType, const std::string& newsMessage, const std::string& originExch)
{
	this->api->updateNewsBulletin(msgId, msgType, newsMessage, originExch);
};

void IBWrapper::managedAccounts(const std::string& accountsList)
{
	this->api->managedAccounts(accountsList);
};

void IBWrapper::receiveFA(faDataType pFaDataType, const std::string& cxml)
{
	this->api->receiveFA(pFaDataType, cxml);
};

void IBWrapper::historicalData(TickerId reqId, const std::string& date, double open, double high, double low, double close, int volume, int barCount, double WAP, int hasGaps)
{
	this->api->historicalData(reqId, date, open, high, low, close, volume, barCount, WAP, hasGaps);
};

void IBWrapper::scannerParameters(const std::string& xml)
{
	this->api->scannerParameters(xml);
};

void IBWrapper::scannerData(int reqId, int rank, const ContractDetails& contractDetails, const std::string& distance, const std::string& benchmark, const std::string& projection, const std::string& legsStr)
{
	this->api->scannerData(reqId, rank, contractDetails, distance, benchmark, projection, legsStr);
};

void IBWrapper::scannerDataEnd(int reqId)
{
	this->api->scannerDataEnd(reqId);
};

void IBWrapper::realtimeBar(TickerId reqId, long time, double open, double high, double low, double close, long volume, double wap, int count)
{
	this->api->realtimeBar(reqId, time, open, high, low, close, volume, wap, count);
};

void IBWrapper::currentTime(long time)
{
	this->api->currentTime(time);
};

void IBWrapper::fundamentalData(TickerId reqId, const std::string& data)
{
	this->api->fundamentalData(reqId, data);
};

void IBWrapper::deltaNeutralValidation(int reqId, const UnderComp& underComp)
{
	this->api->deltaNeutralValidation(reqId, underComp);
};

void IBWrapper::tickSnapshotEnd(int reqId)
{
	this->api->tickSnapshotEnd(reqId);
};

void IBWrapper::marketDataType(TickerId reqId, int marketDataType)
{
	this->api->marketDataType(reqId, marketDataType);
};

void IBWrapper::commissionReport(const CommissionReport& commissionReport)
{
	this->api->commissionReport(commissionReport);
};

void IBWrapper::position(const std::string& account, const Contract& contract, double position, double avgCost)
{
	this->api->position(account, contract, position, avgCost);
};

void IBWrapper::positionEnd()
{
	this->api->positionEnd();
};

void IBWrapper::accountSummary(int reqId, const std::string& account, const std::string& tag, const std::string& value, const std::string& curency)
{
	this->api->accountSummary(reqId, account, tag, value, curency);
};

void IBWrapper::accountSummaryEnd(int reqId)
{
	this->api->accountSummaryEnd(reqId);
};

void IBWrapper::verifyMessageAPI(const std::string& apiData)
{
	this->api->verifyMessageAPI(apiData);
};

void IBWrapper::verifyCompleted(bool isSuccessful, const std::string& errorText)
{
	this->api->verifyCompleted(isSuccessful, errorText);
};

void IBWrapper::displayGroupList(int reqId, const std::string& groups)
{
	this->api->displayGroupList(reqId, groups);
};

void IBWrapper::displayGroupUpdated(int reqId, const std::string& contractInfo)
{
	this->api->displayGroupUpdated(reqId, contractInfo);
};

void IBWrapper::verifyAndAuthMessageAPI(const std::string& apiData, const std::string& xyzChallange)
{
	this->api->verifyAndAuthMessageAPI(apiData, xyzChallange);
};

void IBWrapper::verifyAndAuthCompleted(bool isSuccessful, const std::string& errorText)
{
	this->api->verifyAndAuthCompleted(isSuccessful, errorText);
};

void IBWrapper::connectAck()
{
	this->api->connectAck();
};

void IBWrapper::positionMulti(int reqId, const std::string& account, const std::string& modelCode, const Contract& contract, double pos, double avgCost)
{
	this->api->positionMulti(reqId, account, modelCode, contract, pos, avgCost);
};

void IBWrapper::positionMultiEnd(int reqId)
{
	this->api->positionMultiEnd(reqId);
}

void IBWrapper::accountUpdateMulti(int reqId, const std::string& account, const std::string& modelCode, const std::string& key, const std::string& value, const std::string& currency)
{
	this->api->accountUpdateMulti(reqId, account, modelCode, key, value, currency);
};

void IBWrapper::accountUpdateMultiEnd(int reqId)
{
	this->api->accountUpdateMultiEnd(reqId);
}

void IBWrapper::securityDefinitionOptionalParameter(int reqId, const std::string& exchange, int underlyingConId, const std::string& tradingClass, const std::string& multiplier, std::set<std::string> expirations, std::set<double> strikes)
{
	this->api->securityDefinitionOptionalParameter(reqId, exchange, underlyingConId, tradingClass, multiplier, expirations, strikes);
};

void IBWrapper::securityDefinitionOptionalParameterEnd(int reqId)
{
	this->api->securityDefinitionOptionalParameterEnd(reqId);
};

void IBWrapper::softDollarTiers(int reqId, const std::vector<SoftDollarTier> &tiers)
{
	this->api->softDollarTiers(reqId, tiers);
};

}//cktrader