#include "StdAfx.h"
#include "SendToLocal.h"
#include "GlobalDataManage.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

using namespace std;
extern AlgoManager* algomanager;

SendToLocal::SendToLocal(GlobalDataManage* pGlobalDm){
	m_pGlobalDm = pGlobalDm;
}
SendToLocal::~SendToLocal(){
}

bool SendToLocal::sendOrder(Order* or){

	if(algomanager->getAccountMana()->addFroneCommision(or->getFroneCommision())){
		if(algomanager->getAccountMana()->addFroneMargin(or->getFroneMargin())){
                
        }
        else{
            algomanager->getAccountMana()->addFroneCommision(0 - or->getFroneCommision());
            cout << "Fail#############Not Enough Money!  FroneMargin" << endl;
            return false;
        }
	}
	else{
		cout << "Fail#############Not Enough Money!  FroneCommision" << endl;
        return false;
	}

	m_pGlobalDm->getOrdersMana()->SendedOrder(or);
	m_pGlobalDm->keepRecord(HisOrder, or->getString());
    cout << "sendOrder :"  <<  or->getString()<< endl;
    return true;

}

