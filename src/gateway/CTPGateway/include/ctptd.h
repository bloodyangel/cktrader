#pragma once
#ifndef __CTPTD_H__
#define __CTPTD_H__

#include "ThostFtdcUserApiDataType.h"
#include "ThostFtdcUserApiStruct.h"
#include "ThostFtdcTraderApi.h"
#include "utils/ckdef.h"
#include "utils/cktypes.h"
#include "ctpdefs.h"

#include <map>
#include <mutex>
#include <string>
#include <atomic>

namespace cktrader {

	class EventEngine;
	class CTPGateWay;

	class CtpTd :public CThostFtdcTraderSpi
	{
	public:
		CtpTd(EventEngine *event_service, CTPGateWay *gateWay);
		~CtpTd();

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

		///��¼������Ӧ
		virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		///�ǳ�������Ӧ
		virtual void OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		///����¼��������Ӧ
		virtual void OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		///��������������Ӧ
		virtual void OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		///Ͷ���߽�����ȷ����Ӧ
		virtual void OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		///�����ѯͶ���ֲ߳���Ӧ
		virtual void OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		///�����ѯ�ʽ��˻���Ӧ
		virtual void OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		///�����ѯ��Լ��Ӧ
		virtual void OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		///����Ӧ��
		virtual void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		///����֪ͨ
		virtual void OnRtnOrder(CThostFtdcOrderField *pOrder);

		///�ɽ�֪ͨ
		virtual void OnRtnTrade(CThostFtdcTradeField *pTrade);

		///����¼�����ر�
		virtual void OnErrRtnOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo);

		///������������ر�
		virtual void OnErrRtnOrderAction(CThostFtdcOrderActionField *pOrderAction, CThostFtdcRspInfoField *pRspInfo);


		//-------------------------------------------------------------------------------------
		//task������
		//-------------------------------------------------------------------------------------
		void processFrontConnected(Datablk& data);

		void processFrontDisconnected(Datablk& data);


		void processRspUserLogin(Datablk& data);

		void processRspUserLogout(Datablk& data);

		void processRspOrderInsert(Datablk& data);

		void processRspOrderAction(Datablk& data);

		void processRspSettlementInfoConfirm(Datablk& data);

		void processRspQryInvestorPosition(Datablk& data);

		void processRspQryTradingAccount(Datablk& data);

		void processRspQryInstrument(Datablk& data);

		void processRspError(Datablk& data);

		void processRtnOrder(Datablk& data);

		void processRtnTrade(Datablk& data);

		void processErrRtnOrderInsert(Datablk& data);

		void processErrRtnOrderAction(Datablk& data);

		void connect(std::string userID, std::string password, std::string brokerID, std::string address);

		void login();
		void qryAccount();
		void qryPosition();
		std::string sendOrder(OrderReq& req);
		void cancelOrder(CancelOrderReq & req);
		void close();

	private:
		EventEngine *m_event_service;
		CTPGateWay *gateWay;
		std::string gateWayName;

		CThostFtdcTraderApi* api;

		std::atomic<int> reqID = 0;//����������
		std::atomic<int> orderRef = 0;// �������

		std::atomic<bool> connectionStatus = false;//      ����״̬
		std::atomic<bool> loginStatus = false;// ��¼״̬

		std::string userID;// �˺�
		std::string password;//����
		std::string brokerID;//�����̴���
		std::string address;// ��������ַ

		int frontID;//ǰ�û����
		int sessionID;// �Ự���

		std::map<std::string, PositionData> *posBufferMap = nullptr;// ����ֲ����ݵ��ֵ�

		mutable std::recursive_mutex the_mutex;
	};

}//cktrader

#endif // CTPTD_H