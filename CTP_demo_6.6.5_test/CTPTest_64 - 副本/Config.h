﻿#pragma once
#pragma warning(disable:4996)
#pragma comment(lib,"thosttraderapi_se.lib")
#pragma comment(lib,"thostmduserapi_se.lib")
#include<vector>
#include <cstring>
#include<string.h>
#include <stdio.h>
#include <stdlib.h>
#include<iostream>
using namespace std;
std::string MARKET_ADDRESS="";
std::string TRADER_ADDRESS="";
std::string BROKERID = "";
std::string USERID = "";
std::string INVESTORID = "";
std::string APPID = "";
std::string AUTHCODE = "";
std::string PASSWORD = "";
std::string INSTRUMENTID = "";
//程序会自动订阅行情下单，也可以通过在控制台输入一下命令下单
/*
order SZSE / c 90005271 fak buy open / v 1 / p 1.8209 / q
order SZSE / c 90005272 buy open / v 1 / p 1.8209 / q
*/

void order(std::string cmd);