#include "stdafx.h"
#include "TaoBaoSenderInterface.h"
#include "./Misc/Misc.h"
#include "../../../include/DataManager/BaseDataType/CommonData/CommonData.h"

#if (CUR_EDITION == HG_EDITION)

#include <stdio.h>
#include <stdlib.h>
#include "unistd.h"
#include "topsdk.h"

char* url = "http://gw.api.taobao.com/router/rest"/*"http://api.daily.taobao.net/router/rest?"*/ ;
char* appkey = "23751539"/*"4272"*/;
char* appsecret = "3c9cfdeb7b004090d88f30dbe8dbbe69"/*"0ebbcccfee18d7ad1aebc5b135ffa906"*/;

char* httpsUrl = "http://gw.api.taobao.com/router/rest" /*"https://eco.taobao.com/router/rest?"*/ ;
char* appkey_ol = "23751539"/*"12497914"*/;
char* appsecret_ol = "3c9cfdeb7b004090d88f30dbe8dbbe69"/*"4b0f28396e072d67fae169684613bcd1"*/;

//void testUserGet(){
//	int i;
//	for(i = 0 ; i < 5; i++){
//		pTopRequest pRequest = alloc_top_request();
//		pTopResponse pResponse = NULL;
//		pTaobaoClient pClient = alloc_taobao_client(url, appkey, appsecret);
//		set_api_name(pRequest,"taobao.user.get");
//		add_param(pRequest, "fields", "nick,type,sex,location");
//		add_param(pRequest, "nick", "sandbox_c_1");
//
//		pResponse = top_execute(pClient,pRequest,NULL);
//		printf("ret code:%d\n",pResponse->code);
//		if(pResponse->code == 0){
//			pTopResponseIterator ite = init_response_iterator(pResponse);
//			pResultItem pResultItem = alloc_result_item();
//			while(parseNext(ite, pResultItem) == 0){
//				printf("%s:%s\n",pResultItem->key,pResultItem->value);
//			}
//			destroy_response_iterator(ite);
//			destroy_result_item(pResultItem);
//		}
//		destroy_top_request(pRequest);
//		destroy_top_response(pResponse);
//		destroy_taobao_client(pClient);
//	}
//}
//
//void testByteArray(){
//	int i;
//	for(i = 0 ; i < 5; i++){
//		pTopRequest pRequest = alloc_top_request();
//		pTopResponse pResponse = NULL;
//		pTaobaoClient pClient = alloc_taobao_client(url, appkey, appsecret);
//		set_api_name(pRequest,"taobao.chengxian.bytetest");
//
//		pResponse = top_execute(pClient,pRequest,NULL);
//		printf("ret code:%d\n",pResponse->code);
//		if(pResponse->code == 0){
//			printf("%s\n",pResponse->bytes);
//		}
//		destroy_top_request(pRequest);
//		destroy_top_response(pResponse);
//		destroy_taobao_client(pClient);
//	}
//}
//
//void testHttps(){
//
//	pTopRequest pRequest = alloc_top_request();
//	pTopResponse pResponse = NULL;
//	pTaobaoClient pClient = alloc_taobao_client(httpsUrl, appkey_ol, appsecret_ol);
//
//	set_retry_times(3);
//	set_retry_sleep_times(100);
//
//	set_api_name(pRequest,"taobao.time.get");
//	pResponse = top_execute(pClient,pRequest,NULL);
//
//	printf("ret code:%d\n",pResponse->code);
//	if(pResponse->code == 0){
//		pTopResponseIterator ite = init_response_iterator(pResponse);
//		pResultItem pResultItem = alloc_result_item();
//		while(parseNext(ite, pResultItem) == 0){
//			printf("%s:%s\n",pResultItem->key,pResultItem->value);
//		}
//		destroy_response_iterator(ite);
//		destroy_result_item(pResultItem);
//	}else{
//		printf("Error Msg:%s\n",pResponse->msg);
//		printf("Error Sub Msg:%s\n",pResponse->subMsg);
//	}
//
//	destroy_top_request(pRequest);
//	destroy_top_response(pResponse);
//	destroy_taobao_client(pClient);
//}

int SendStringToPhoneFunc(LPCSTR szPhoneNum, LPCSTR szToSend)
{
	int nRet;

	pTopRequest pRequest   =   alloc_top_request();  
	pTopResponse pResponse   =   NULL;  
	pTaobaoClient pClient   =   alloc_taobao_client(url, appkey, appsecret);  

	set_api_name(pRequest,   "alibaba.aliqin.fc.sms.num.send"   );  
	//add_param(pRequest,   "extend"   ,   "123"   );  
	add_param(pRequest,   "sms_type"   ,   "normal"   );  

	std::wstring wstringTmp = AnsiToUnicode(/*"博大科技"*/"开料软件");
	std::string stringTmp = WStringToUTF8(wstringTmp.c_str());
	add_param(pRequest,   "sms_free_sign_name"   ,   /*"博大科技"*/stringTmp.c_str()   );

	CString str_sms_param;
	str_sms_param.Format(/*_T("{return_code:'%s'}")*/_T("{content:'%s'}"), szToSend);
	wstringTmp = AnsiToUnicode(str_sms_param.GetBuffer());
	str_sms_param.ReleaseBuffer();
	stringTmp = WStringToUTF8(wstringTmp.c_str());

	add_param(pRequest,   "sms_param"   ,  /*"{return_code:'000003'}"*/stringTmp.c_str()  );  
	//add_param(pRequest,   "sms_param"   ,  "{\"return_code\":\"000003\"}"  ); 
	add_param(pRequest,   "rec_num"   ,   /*"13632418997"*/szPhoneNum   );  
	add_param(pRequest,   "sms_template_code"   ,  /*"SMS_61745326"*/"SMS_63930230"  );  

	pResponse   =   top_execute(pClient,pRequest,NULL);  
	printf(   "ret code:%d\n"   ,pResponse->code);  
	nRet = pResponse->code;
	if   (pResponse->code ==  0   ){  

		pTopResponseIterator ite   =   init_response_iterator(pResponse);  

		pResultItem pResultItem   =   alloc_result_item();  

		while   (parseNext(ite, pResultItem)   ==   0   ){  

			printf("%s:%s\n",pResultItem->key,pResultItem->value);  

		}  

		destroy_response_iterator(ite);  
		destroy_result_item(pResultItem);  

	}  

	destroy_top_request(pRequest);  
	destroy_top_response(pResponse);  
	destroy_taobao_client(pClient);

	return nRet;
}

#else

#using "..\..\..\bin\SMSSender.dll"
using namespace SMSSender;

int SendStringToPhoneFunc(LPCSTR szPhoneNum, LPCSTR szToSend)
{
	CSMSSender^ pCSMSSender = gcnew CSMSSender();
	
	int nRet = pCSMSSender->SendStringToPhoneFunc(gcnew System::String((char*)szPhoneNum), gcnew System::String((char*)szToSend));
	//CString strTmp;
	//strTmp.Format(_T("%d"), nRet);
	//AfxMessageBox(strTmp);
	return nRet;
}

#endif

int SendStringToPhone(LPCSTR szPhoneNum, LPCSTR szToSend)
{
	return SendStringToPhoneFunc(szPhoneNum, szToSend);
}

