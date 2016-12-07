#pragma once
#ifndef __LTSQRY_H__
#define __LTSQRY_H__

#include "ltsdefs.h"
#include "LTSGateway.h"

#include "utils/ckdef.h"
#include "utils/cktypes.h"

#include "SecurityFtdcQueryApi.h"

namespace cktrader {

//API�ļ̳�ʵ��
class LtsQry : public CSecurityFtdcQuerySpi
{
private:
	CSecurityFtdcQueryApi* api;			//API����

	EventEngine *m_event_service;
	LTSGateway *gateWay;
	std::string gateWayName;

	std::atomic<int> reqID = 0;//����������

	std::atomic<bool> connectionStatus = false;//      ����״̬
	std::atomic<bool> loginStatus = false;// ��¼״̬

	std::string userID;// �˺�
	std::string password;//����
	std::string brokerID;//�����̴���
	std::string address;// ��������ַ
	std::string productInfo;
	std::string authCode;
	std::string randCode;

	int frontID;//ǰ�û����
	int sessionID;// �Ự���

public:
	LtsQry(EventEngine* pEvent, LTSGateway* gateway);

	~LtsQry();

	//-------------------------------------------------------------------------------------
	//API�ص�����
	//-------------------------------------------------------------------------------------

	///���ͻ����뽻�׺�̨������ͨ������ʱ����δ��¼ǰ�����÷��������á�
	virtual void OnFrontConnected();

	///���ͻ����뽻�׺�̨ͨ�����ӶϿ�ʱ���÷��������á���������������API���Զ��������ӣ��ͻ��˿ɲ�������
	///@param nReason ����ԭ��
	///        0x1001 �����ʧ��
	///        0x1002 ����дʧ��
	///        0x2001 ����������ʱ
	///        0x2002 ��������ʧ��
	///        0x2003 �յ�������
	virtual void OnFrontDisconnected(int nReason);

	///������ʱ���档����ʱ��δ�յ�����ʱ���÷��������á�
	///@param nTimeLapse �����ϴν��ձ��ĵ�ʱ��
	virtual void OnHeartBeatWarning(int nTimeLapse);

	///����Ӧ��
	virtual void OnRspError(CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///��¼������Ӧ
	virtual void OnRspUserLogin(CSecurityFtdcRspUserLoginField *pRspUserLogin, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�ǳ�������Ӧ
	virtual void OnRspUserLogout(CSecurityFtdcUserLogoutField *pUserLogout, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///��ȡ��֤�����������Ӧ
	virtual void OnRspFetchAuthRandCode(CSecurityFtdcAuthRandCodeField *pAuthRandCode, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�����ѯ��������Ӧ
	virtual void OnRspQryExchange(CSecurityFtdcExchangeField *pExchange, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�����ѯ��Լ��Ӧ
	virtual void OnRspQryInstrument(CSecurityFtdcInstrumentField *pInstrument, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�����ѯͶ������Ӧ
	virtual void OnRspQryInvestor(CSecurityFtdcInvestorField *pInvestor, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�����ѯ���ױ�����Ӧ
	virtual void OnRspQryTradingCode(CSecurityFtdcTradingCodeField *pTradingCode, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�����ѯ�ʽ��˻���Ӧ
	virtual void OnRspQryTradingAccount(CSecurityFtdcTradingAccountField *pTradingAccount, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�����ѯծȯ��Ϣ��Ӧ
	virtual void OnRspQryBondInterest(CSecurityFtdcBondInterestField *pBondInterest, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�����ѯ��ֵ������Ϣ��Ӧ
	virtual void OnRspQryMarketRationInfo(CSecurityFtdcMarketRationInfoField *pMarketRationInfo, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�����ѯ��Լ����������Ӧ
	virtual void OnRspQryInstrumentCommissionRate(CSecurityFtdcInstrumentCommissionRateField *pInstrumentCommissionRate, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�����ѯETF��Լ��Ӧ
	virtual void OnRspQryETFInstrument(CSecurityFtdcETFInstrumentField *pETFInstrument, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�����ѯETF��Ʊ����Ӧ
	virtual void OnRspQryETFBasket(CSecurityFtdcETFBasketField *pETFBasket, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�����ѯOF��Լ��Ӧ
	virtual void OnRspQryOFInstrument(CSecurityFtdcOFInstrumentField *pOFInstrument, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�����ѯSF��Լ��Ӧ
	virtual void OnRspQrySFInstrument(CSecurityFtdcSFInstrumentField *pSFInstrument, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�����ѯ��Լ���ֱ�֤����Ӧ
	virtual void OnRspQryInstrumentUnitMargin(CSecurityFtdcInstrumentUnitMarginField *pInstrumentUnitMargin, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�����ѯԤ������Ϣ��Ӧ
	virtual void OnRspQryPreDelivInfo(CSecurityFtdcPreDelivInfoField *pPreDelivInfo, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�����ѯ����ȯ������Ϣ��Ӧ
	virtual void OnRspQryCreditStockAssignInfo(CSecurityFtdcCreditStockAssignInfoField *pCreditStockAssignInfo, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�����ѯ�����ʷ�����Ϣ��Ӧ
	virtual void OnRspQryCreditCashAssignInfo(CSecurityFtdcCreditCashAssignInfoField *pCreditCashAssignInfo, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�����ѯ֤ȯ��������Ӧ
	virtual void OnRspQryConversionRate(CSecurityFtdcConversionRateField *pConversionRate, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�����ѯ��ʷ���ø�ծ��Ϣ��Ӧ
	virtual void OnRspQryHisCreditDebtInfo(CSecurityFtdcHisCreditDebtInfoField *pHisCreditDebtInfo, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�����ѯ���龲̬��Ϣ��Ӧ
	virtual void OnRspQryMarketDataStaticInfo(CSecurityFtdcMarketDataStaticInfoField *pMarketDataStaticInfo, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�����ѯ���ڻع���Ϣ��Ӧ
	virtual void OnRspQryExpireRepurchInfo(CSecurityFtdcExpireRepurchInfoField *pExpireRepurchInfo, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�����ѯծȯ��ѺΪ��׼ȯ������Ӧ
	virtual void OnRspQryBondPledgeRate(CSecurityFtdcBondPledgeRateField *pBondPledgeRate, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�����ѯծȯ��Ѻ������չ�ϵ��Ӧ
	virtual void OnRspQryPledgeBond(CSecurityFtdcPledgeBondField *pPledgeBond, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�����ѯ������Ӧ
	virtual void OnRspQryOrder(CSecurityFtdcOrderField *pOrder, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�����ѯ�ɽ���Ӧ
	virtual void OnRspQryTrade(CSecurityFtdcTradeField *pTrade, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�����ѯͶ���ֲ߳���Ӧ
	virtual void OnRspQryInvestorPosition(CSecurityFtdcInvestorPositionField *pInvestorPosition, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�ʽ�ת�˲�ѯӦ��
	virtual void OnRspQryFundTransferSerial(CSecurityFtdcFundTransferField *pFundTransfer, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�ʽ���ת��ˮ��ѯӦ��
	virtual void OnRspQryFundInterTransferSerial(CSecurityFtdcFundInterTransferSerialField *pFundInterTransferSerial, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	//-------------------------------------------------------------------------------------
	//task������
	//-------------------------------------------------------------------------------------

	void processFrontConnected(Datablk& data);

	void processFrontDisconnected(Datablk& data);

	void processHeartBeatWarning(Datablk& data);

	void processRspError(Datablk& data);

	void processRspUserLogin(Datablk& data);

	void processRspUserLogout(Datablk& data);

	void processRspFetchAuthRandCode(Datablk& data);

	void processRspQryExchange(Datablk& data);

	void processRspQryInstrument(Datablk& data);

	void processRspQryInvestor(Datablk& data);

	void processRspQryTradingCode(Datablk& data);

	void processRspQryTradingAccount(Datablk& data);

	void processRspQryBondInterest(Datablk& data);

	void processRspQryMarketRationInfo(Datablk& data);

	void processRspQryInstrumentCommissionRate(Datablk& data);

	void processRspQryETFInstrument(Datablk& data);

	void processRspQryETFBasket(Datablk& data);

	void processRspQryOFInstrument(Datablk& data);

	void processRspQrySFInstrument(Datablk& data);

	void processRspQryInstrumentUnitMargin(Datablk& data);

	void processRspQryPreDelivInfo(Datablk& data);

	void processRspQryCreditStockAssignInfo(Datablk& data);

	void processRspQryCreditCashAssignInfo(Datablk& data);

	void processRspQryConversionRate(Datablk& data);

	void processRspQryHisCreditDebtInfo(Datablk& data);

	void processRspQryMarketDataStaticInfo(Datablk& data);

	void processRspQryExpireRepurchInfo(Datablk& data);

	void processRspQryBondPledgeRate(Datablk& data);

	void processRspQryPledgeBond(Datablk& data);

	void processRspQryOrder(Datablk& data);

	void processRspQryTrade(Datablk& data);

	void processRspQryInvestorPosition(Datablk& data);

	void processRspQryFundTransferSerial(Datablk& data);

	void processRspQryFundInterTransferSerial(Datablk& data);

	
	//-------------------------------------------------------------------------------------
	//req:��������
	//-------------------------------------------------------------------------------------
	void connect(std::string userID, 
				std::string password, 
				std::string brokerID, 
				std::string address,
				std::string productInfo, 
				std::string authCode);

	void login();

	void qryAccount();

	void qryPosition();

	void close();
};


}//cktrader

#endif