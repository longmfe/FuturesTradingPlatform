#include "stdafx.h"
#include "FullgoalStrategy.h"

extern AlgoManager* algomanager;

FullgoalStrategy::FullgoalStrategy(AlgoTSEng* ats){
	this->ats = ats;
	init();
}

FullgoalStrategy::~FullgoalStrategy(void){
}

void FullgoalStrategy::init(){
	strategyID = ats->espi->getCombName();
	maNum = ats->espi->getMaNum(); //->MA_num;
	N_1minBar = ats->espi->getNminite();//->N_1minBar;
	contract1=ats->espi->getContract1(); //contract1;
	contract2=ats->espi->getContract2();//contract2;
	MAparam_price=ats->espi->getMaPrice();//MAparam_price;
	MAparam_vol=ats->espi->getMaVolume();//MAparam_vol;
	MAparam_price_c=ats->espi->getMaPrice_c();//MAparam_price;
	MAparam_vol_c=ats->espi->getMaVolume_c();//MAparam_vol;
	stopLoss=ats->espi->getStoploss();
	hasOpen=false;
	hasClose = false;
	openTime = "000000.0";
	hasOpen2=false;
	closeTime = "000000.0";
	openDiff = 0.0;
	clearTime=ats->espi->getClearTime();

	isTradingReal = algomanager->IsConnectToMarket();
	stl = new SendToLocal(ats->pGlobalDm);
}

void FullgoalStrategy::run(){

	ss=*(ats->pGlobalDm->getMdMana()->getTickData());
	int tick_min=tc.timeToHourmin(ss.getTime());

	Position* pos1 = ats->pGlobalDm->getPositionMana()->getPositionBysec(ats->espi->getContract1());
	int posVolConfig1=ats->espi->getVolume1();
	int posVol1=0;
	if (pos1->getBuyopen()>0)
	{
		posVol1=pos1->getBuyopen();
	}else{
		posVol1=pos1->getSellopen();
	}

	Position* pos2 = ats->pGlobalDm->getPositionMana()->getPositionBysec(ats->espi->getContract2());
	int posVolConfig2=ats->espi->getVolume2();
	int posVol2=0;
	if (pos2->getBuyopen()>0)
	{
		posVol2=pos2->getBuyopen();
	}else{
		posVol2=pos2->getSellopen();
	}
	
	bool hasPosition1=false;
	if (posVol1 == posVolConfig1) 
	{
		hasPosition1=true;
	}

	bool hasPosition2=false;
	if (posVol2 == posVolConfig2)
	{
		hasPosition2=true;
	}

	if (hasPosition1 && hasPosition2)
	{
		hasOpen=false;
		hasOpen2=false;
	}
	if (posVol1==0 && posVol2==0)
	{
		hasClose=false;
	}

	int combDirection=-1;   //��ϳֲ�Ϊ��������
	if ((pos1->getSellopen()>0 && pos1->getBuyopen()==0) || (pos2->getBuyopen()>0 && pos2->getSellopen()==0))
	{
		combDirection=0;   //��ϳֲ�Ϊ���뿪��
	}
	if ((pos1->getSellopen()==0 && pos1->getBuyopen()>0) || (pos2->getBuyopen()==0 && pos2->getSellopen()>0))
	{
		combDirection=1;   //��ϳֲ�Ϊ���뿪��
	}

	if(hasOpen && !hasPosition1 && !hasPosition2  && tc.timeMinus(ss.getTime(), openTime) > ats->espi->getTimeSpan()){
		esCancleAllOrder(ats->espi->getContract1());
		hasOpen = false;
	}else if(hasOpen && hasPosition1 && !hasPosition2){
		if(hasOpen2){			
			if (combDirection==0 && tc.timeMinus(ss.getTime(), openTime) > ats->espi->getTimeSpan()){
				esCancleAllOrder(ats->espi->getContract2());
				esOpenCombOrder(0, true, 0, ats->espi->getVolume2());
			}else if(combDirection==1 && tc.timeMinus(ss.getTime(), openTime) > ats->espi->getTimeSpan()){
				esCancleAllOrder(ats->espi->getContract2());
				esOpenCombOrder(1, true, 0, ats->espi->getVolume2());
			}
		}else{
			if (combDirection==0){
				esOpenCombOrder(0, true, 0, ats->espi->getVolume2());
			}else if(combDirection==1){
				esOpenCombOrder(1, true, 0, ats->espi->getVolume2());
			}
			hasOpen2 = true;
		}
		
	}

	if(hasClose && hasPosition2 && tc.timeMinus(ss.getTime(), closeTime) > ats->espi->getTimeSpan()){
		esCancleAllOrder(ats->espi->getContract2());
		if (combDirection==0)
		{
			esCloseCombOrder(1, true, 0, ats->espi->getVolume2());
		}else if(combDirection==1){
			esCloseCombOrder(0, true, 0, ats->espi->getVolume2());
		}
	}
	if(hasClose && hasPosition1 && tc.timeMinus(ss.getTime(), closeTime) > ats->espi->getTimeSpan()){
		esCancleAllOrder(ats->espi->getContract1());
		if (combDirection==0)
		{
			esCloseCombOrder(1, true, ats->espi->getVolume1(), 0);
		}else if (combDirection==1)
		{
			esCloseCombOrder(0, true, ats->espi->getVolume1(), 0);
		}
	}

	//��Ͽ���ִ�д���
	//CombOrder 
	/*if(hasOpen && tick_min - tc.timeToHourmin(openTime) > ats->espi->getTimeSpan())
	{
		if(ats->espi->getOpenType()) //limit order
		{
			if(tick_min - tc.timeToHourmin(openTime) < 2 * ats->espi->getTimeSpan())
			{
				if(hasPosition1){
					esOpenCombOrder(true, 0, ats->espi->getVolume2());//send B with limit order
				}else
				{
					esCancleAllOrder(ats->espi->getContract1());
				}
			}else{
				if(!hasPosition2){
					esCancleAllOrder(ats->espi->getContract2());
					esOpenCombOrder(true, 0, ats->espi->getVolume2());//send B with limit order
				}
			}
		}		
	}

	if(hasClose && tick_min - tc.timeToHourmin(closeTime) > ats->espi->getTimeSpan())
	{
		if(ats->espi->getCloseType()) //limit order
		{
			if(hasPosition1){
				esCancleAllOrder(ats->espi->getContract1());
				esCloseCombOrder(true, ats->espi->getVolume1(), 0);
			}

			if(hasPosition2){
				esCancleAllOrder(ats->espi->getContract2());
				esCloseCombOrder(true, 0, ats->espi->getVolume2());
			}
		}		
	}*/

	//����Ϊ���������жϵ��߼�
	
	cb=(CombBar*)ats->pGlobalDm->getIndexMana()->getIndexUnit(contract1+"_"+contract2);	
	
	int minit = tick_min/100;
	if (posVol1==0 && posVol2==0 && !hasOpen && minit>1037 && minit<1450) //
	{
		bool buyopen=true;
		bool sellopen=true;
		//for(int i=1;i<=N_1minBar;i++){
		//	int idx=cb->getDifferVsize()-1-i;
		//	double tmpDiffer = cb->getDifferByIndex(idx);

		//	int idx_ma=cb->maDifferV.size()-1-i;
		//	double tmpDifferMA = cb->maDifferV[idx_ma];
		//	if (tmpDiffer<=tmpDifferMA+MAparam_price)
		//	{
		//		sellopen=false;
		//	}
		//	if (tmpDiffer>=tmpDifferMA-MAparam_price_c)
		//	{
		//		buyopen=false;
		//	}

		//	/*double tmpVolumeSum = cb->sumVector[idx];
		//	double tmpVolumeSumMA = cb->maSumV[idx_ma];
		//	if (tmpVolumeSum<=tmpVolumeSumMA+MAparam_vol)
		//	{
		//	buyopen=false;
		//	break;
		//	}*/
		//}

			double curDiffer=cb->differVector.back();
			double curDifferMA=cb->maDifferV.back();

			double curVolumeSum = cb->sumVector.back();
			double curVolumeSumMA = cb->maSumV.back();

			if(curDifferMA==0.0){
				return;
			}

			if ( (curDiffer>curDifferMA+MAparam_price)/* && (curVolumeSum>curVolumeSumMA+MAparam_vol)*/)  //��������
			{
				ats->pGlobalDm->keepRecord(HisOrder, "\r\n ��������:");
				//esOpenCombOrder(0, true, ats->espi->getVolume1(), ats->espi->getVolume2());
				//ats->pGlobalDm->keepRecord(HisOrder, "\r\n ��"+contract1+""+posVolConfig1+"�֣���"+contract2+""+posVolConfig2+"��");
				esOpenCombOrder(0, true, ats->espi->getVolume1(), 0);
				openDiff=curDiffer;
				ats->pGlobalDm->keepRecord(HisOrder, "����ʱ�۲�Ϊ��"+tc.doubleToString(openDiff)+" �۲��ֵΪ��"+tc.doubleToString(curDifferMA));
				hasOpen = true;
				return;
			}

			if ( (curDiffer<curDifferMA-MAparam_price_c)/* && (curVolumeSum<curVolumeSumMA+MAparam_vol)*/)
			{
				ats->pGlobalDm->keepRecord(HisOrder, "\r\n ���뿪��:");
				//esOpenCombOrder(1, true, ats->espi->getVolume1(), ats->espi->getVolume2());
				esOpenCombOrder(1, true, ats->espi->getVolume1(), 0);
				openDiff=curDiffer;
				ats->pGlobalDm->keepRecord(HisOrder, "����ʱ�۲�Ϊ��"+tc.doubleToString(openDiff)+" �۲��ֵΪ��"+tc.doubleToString(curDifferMA));
				hasOpen = true;
				return;
			}
	}


	

	//����Ϊƽ�������жϵ��߼�
	if (hasPosition1 && hasPosition2 && !hasClose && minit>1037 && minit<1450)
	{
		double curDiffer=cb->differVector.back();
		double curDifferMA=cb->maDifferV.back();

		/*double curVolumeSum = cb->sumVector.back();
		double curVolumeSumMA = cb->maSumV.back();*/

		if ( curDiffer<=curDifferMA && combDirection==0 /*&& (curVolumeSum<curVolumeSumMA+MAparam_vol_c)*/)  //����ƽ��
		{
			ats->pGlobalDm->keepRecord(HisOrder, "\r\n �ֲ�Ϊ����������ƽ��:");
			esCloseCombOrder(1, true, ats->espi->getVolume1(), ats->espi->getVolume2());   //�˴��Ƿ�Ҫ��ǲ�λ��־��
			ats->pGlobalDm->keepRecord(HisOrder, "ƽ��ʱ�۲�Ϊ��" +tc.doubleToString(curDiffer)+" �۲��ֵΪ��"+tc.doubleToString(curDifferMA));
			hasClose = true;
			return;
		}else if ( curDiffer>=curDifferMA && combDirection==1 /*&& (curVolumeSum<curVolumeSumMA+MAparam_vol_c)*/)  //����ƽ��
		{
			ats->pGlobalDm->keepRecord(HisOrder, "\r\n �ֲ�Ϊ�򿪣�����ƽ��:");
			esCloseCombOrder(0, true, ats->espi->getVolume1(), ats->espi->getVolume2());   //�˴��Ƿ�Ҫ��ǲ�λ��־��
			ats->pGlobalDm->keepRecord(HisOrder, "ƽ��ʱ�۲�Ϊ��" +tc.doubleToString(curDiffer)+" �۲��ֵΪ��"+tc.doubleToString(curDifferMA));
			hasClose = true;
			return;
		}
	}


	//����Ϊֹ�������жϵ��߼�
	if (hasPosition1 && hasPosition2 && !hasClose  && minit>1037 && minit<1450){
		double curDiffer=cb->differVector.back();
		if (curDiffer >= (openDiff+stopLoss) && combDirection==0)
		{
			ats->pGlobalDm->keepRecord(HisOrder, "\r\n �ֲ�Ϊ��������ǰ�۲�Ϊ��" +tc.doubleToString(curDiffer)+" ����ʱ�۲�Ϊ��"+tc.doubleToString(openDiff)+"  ��ǰ�۲� >= ����ʱ�۲�+ֹ���λ��������ƽ��ֹ��:");
			esCloseCombOrder(1, true, ats->espi->getVolume1(), ats->espi->getVolume2());   //�˴��Ƿ�Ҫ��ǲ�λ��־��
			hasClose = true;
			return;
		}else if (curDiffer <= (openDiff-stopLoss) && combDirection==1)
		{
			ats->pGlobalDm->keepRecord(HisOrder, "\r\n �ֲ�Ϊ�򿪣���ǰ�۲�Ϊ��" +tc.doubleToString(curDiffer)+" ����ʱ�۲�Ϊ��"+tc.doubleToString(openDiff)+"  ��ǰ�۲� <= ��ʱ�۲�-ֹ���λ��������ƽ��ֹ��:");
			esCloseCombOrder(0, true, ats->espi->getVolume1(), ats->espi->getVolume2());   //�˴��Ƿ�Ҫ��ǲ�λ��־��
			hasClose = true;
			return;
		}
	}


	//����Ϊ��������жϵ��߼�
	if (hasPosition1 && hasPosition2 && !hasClose && minit>=clearTime)
	{
		ats->pGlobalDm->keepRecord(HisOrder, "\r\n����ǰ��֣�");
		if (combDirection==0)
		{
			esCloseCombOrder(1, true, ats->espi->getVolume1(), ats->espi->getVolume2());   //�˴��Ƿ�Ҫ��ǲ�λ��־��
		}else if (combDirection==1)
		{
			esCloseCombOrder(0, true, ats->espi->getVolume1(), ats->espi->getVolume2());   //�˴��Ƿ�Ҫ��ǲ�λ��־��
		}
		
		hasClose = true;
	}
}

void FullgoalStrategy::esOpenCombOrder(int direction, bool type, int volume1, int volume2) //0-market 1-limit
{
	Stype direction1, direction2;
	double price1 = 0.0, price2 = 0.0;
	if(direction==0) //�����������
	{
		direction1 = Sellshort;
		price1 = cb->getBidPrice1();
		direction2 = Buy;
		price2 = cb->getAskPrice2();
	}
	else if(direction==1) //������뿪��
	{
		direction1 = Buy;
		price1 = cb->getAskPrice1();
		direction2 = Sellshort;
		price2 = cb->getBidPrice2();
	}

	//open position: 0-market order, 1-limit order
	if(type) //limit order
	{
		//if(price1 != 0 && price2 !=0){			
			esSendCombOrder(direction1, price1,  volume1, direction2, price2,  volume2);
			hasOpen = true;
		//}
	}else
	{
		esSendCombOrder(direction1, 0,  volume1, direction2, 0,  volume2);
		hasOpen = true;
	}

	openTime = ss.getTime();
}

void FullgoalStrategy::esCloseCombOrder(int direction, bool type, int volume1, int volume2)
{
	Stype direction1, direction2;
	double price1 = 0.0, price2 = 0.0;
	if(direction==1) //�������ƽ��
	{
		direction1 = Buytocover;
		price1 = cb->getAskPrice1();
		direction2 = Sell;
		price2 = cb->getBidPrice2();
	}
	else if(direction==0) //�������ƽ��
	{
		direction1 = Sell;
		price1 = cb->getBidPrice1();
		direction2 = Buytocover;
		price2 = cb->getAskPrice2();
	}

	//close position: 0-market order, 1-limit order
	if(type) //limit order
	{
		esSendCombOrder(direction1, price1, volume1, direction2, price2, volume2);
		hasClose = true;
	}else
	{
		esSendCombOrder(direction1, 0, volume1, direction2, 0, volume2);
		//hasClose = true;
	}

	closeTime = ss.getTime();
}

void FullgoalStrategy::esSendCombOrder(Stype direction1, double price1,int volume1, Stype direction2, double price2, int volume2)
{
	
	if(0 != volume1){
		Order*  or1 = new Order(strategyID, ats->pGlobalDm->getOrdersMana()->getLastOrderID(), ats->espi->getContract1(), 
			price1, direction1, 0, volume1, ss.getDate(), ss.getTime(), ats->espi->getExchage(),algomanager->getAccountMana());
		ats->pGlobalDm->keepRecord(HisOrder, "������ " + or1->getString());
		esSendOrder(or1);
	}

	if(0 != volume2){
		Order*  or2 = new Order(strategyID, ats->pGlobalDm->getOrdersMana()->getLastOrderID(), ats->espi->getContract2(), 
			price2, direction2, 0, volume2, ss.getDate(), ss.getTime(), ats->espi->getExchage(),algomanager->getAccountMana());
		ats->pGlobalDm->keepRecord(HisOrder, "������ " + or2->getString());
		esSendOrder(or2);	
	}

}

void FullgoalStrategy::esSendOrder(Order* order)
{
	if(isTradingReal)
	{
		algomanager->sendorder(order,ats);
	}
	else 
	{
		stl->sendOrder(order);
	}
}

bool FullgoalStrategy::esCancleAllOrder(string contract){
	bool isOrderCancel = false;
	cout<<"in esCancelAllOrder"<<endl;
	if(isTradingReal)
	{
		bool res = false;        
		list<Order*> untradeList=ats->pGlobalDm->getOrdersMana()->getUnTradeOrder();
		if (untradeList.size()==0)
		{
			return false;
		}else{
			list<Order*>::iterator it = ats->pGlobalDm->getOrdersMana()->getUnTradeOrder().begin();
			int i = 0;
			for(;it!=ats->pGlobalDm->getOrdersMana()->getUnTradeOrder().end();it++)
			{
				if((*it)->getSec().compare(contract) == 0)
				{
					algomanager->cancelOrder(*it,ats);
					ats->pGlobalDm->keepRecord(CancelOrders, "����: " + contract + "," + tc.IntToString((*it)->getOrderID()));
					res = true;
					isOrderCancel = true;
					i++;
				}
			}
		}
		//cout<< contract << "cancel All, totally: "<<i<<endl;
	}
	else
	{
		isOrderCancel = ats->pGlobalDm->getOrdersMana()->CancelAll(contract);
		if(isOrderCancel)
			ats->pGlobalDm->keepRecord(CancelOrders, "����: " + contract + "," + ss.getTime());
	}
	return isOrderCancel;
}
