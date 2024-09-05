/* 
*  bflib.c - C Language library of BFinancial.
*
*  Written by Renato Fermi <repiazza@gmail.com> in August 2024
*  
**/
#include <curl/curl.h>
#include <cjson/cJSON.h>
#include <string.h>
#include <stdlib.h>
#include "curl_req.h"
#include "payments.h"

#define FALSE 0
#define TRUE  1

#define URL_PAYM_API "http://127.0.0.1:8080"
#define MAX_LEN 256

int bInitPayments(const char* szAuth, STRUCT_PAYMENTS* pstPayments) {
  if (pstPayments == NULL) {
    return FALSE;  
  }

  snprintf(pstPayments->szApi, MAX_LEN, "%s", URL_PAYM_API);
  snprintf(pstPayments->szAuth, MAX_LEN, "%s", szAuth);
  
  return TRUE;  
}

// Função para login e criação de um CLIENT
STRUCT_CLIENT* pstClientLogin(const char* szAuth) {
  STRUCT_CLIENT* pstClient;  // pst para ponteiro para struct

  if ( (pstClient = (STRUCT_CLIENT*)malloc(sizeof(STRUCT_CLIENT))) == NULL )
    return NULL;
  
  memset(pstClient, 0, sizeof(STRUCT_CLIENT));
  snprintf(pstClient->szAuth, MAX_LEN, "%s", szAuth);

  // Inicializa STRUCT_PAYMENTs com stPayments de STRUCT_CLIENT
  if ( !bInitPayments(pstClient->szAuth, &(pstClient->stPayments)) ) {
    free(pstClient);  // Libera a memória alocada para pstClient em caso de falha
    return NULL;    
  }

  return pstClient;
}

STRUCT_PAYMENT_RESPONSE* pstPaymentCreate(STRUCT_PAYMENTS* pstPayments, const STRUCT_PAYMENT_CREATE* pstPaymentCreate) {
  char* pszResponse = NULL;
  char szData[1024];
  char szAuthHeader[256];
  CURL* pCurl;
  CURLcode res;
  struct curl_slist* pHeaders = NULL;
  STRUCT_PAYMENT_RESPONSE* pstPaymentResponse = NULL;

  pCurl = curl_easy_init();
  if (!pCurl)
    return NULL;

  memset(szAuthHeader, 0, sizeof(szAuthHeader));
  pHeaders = curl_slist_append(pHeaders, "Content-Type: application/json");
  snprintf(szAuthHeader, sizeof(szAuthHeader), "Authorization-key: %s", pstPayments->szAuth);
  pHeaders = curl_slist_append(pHeaders, szAuthHeader);

  if (pstPaymentCreate->eMethod == METHOD_CARD) {
    const STRUCT_CARD_CREATE *pstCardCreate = &pstPaymentCreate->uData.stCardCreate;
    snprintf(szData, sizeof(szData),
  "{"
    "\"method\":\"Card\","     // -- Fixo
    "\"amount\":%.2f,"         // 01
    "\"number\":\"%s\","       // 02
    "\"cvv\":\"%s\","          // 03
    "\"payer_email\":\"%s\","  // 04
    "\"payer_name\":\"%s\","   // 05
    "\"payer_cpf\":\"%s\","    // 06
    "\"expiration_year\":%d,"  // 07
    "\"expiration_month\":%d"  // 08
  "}",
      pstCardCreate->dAmount,         // 01   
      pstCardCreate->szNumber,        // 02     
      pstCardCreate->szCvv,           // 03
      pstCardCreate->szCustomerEmail, // 04  
      "John Doe",                     // 05 
      "12345678900",                  // 06 
      2025,                           // 07 
      12                              // 08     
    );
  }
  else if (pstPaymentCreate->eMethod == METHOD_PIX) {
    const STRUCT_PIX_CREATE *pstPixCreate = &pstPaymentCreate->uData.stPixCreate;
    snprintf(szData, sizeof(szData),
  "{"
    "\"method\":\"Pix\","              // -- Fixo
    "\"amount\":%.2f,"                 // 01
    "\"payer_email\":\"%s\""           // 02
  "}",
      pstPixCreate->dAmount,           // 01
      pstPixCreate->szCustomerEmail    // 02
    );
  }

  curl_easy_setopt(pCurl, CURLOPT_URL, "http://127.0.0.1:8080/payment/create");
  curl_easy_setopt(pCurl, CURLOPT_HTTPHEADER, pHeaders);
  curl_easy_setopt(pCurl, CURLOPT_POSTFIELDS, szData);
  curl_easy_setopt(pCurl, CURLOPT_WRITEFUNCTION, zWriteResponse);
  curl_easy_setopt(pCurl, CURLOPT_WRITEDATA, &pszResponse);

  res = curl_easy_perform(pCurl);
  if (res != CURLE_OK) {
    fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
  } 
  else {
    // Parseia a resposta JSON
    cJSON *pJsonResponse = cJSON_Parse(pszResponse);
    if (pJsonResponse) {
      cJSON *pData = cJSON_GetObjectItem(pJsonResponse, "data");
      if (pData) {
        // Aloca e preenche a estrutura de resposta
        pstPaymentResponse = (STRUCT_PAYMENT_RESPONSE*)malloc(sizeof(STRUCT_PAYMENT_RESPONSE));
        if (pstPaymentResponse != NULL) {
          memset(pstPaymentResponse, 0, sizeof(STRUCT_PAYMENT_RESPONSE));
          // Preenche os dados comuns
          strcpy(pstPaymentResponse->szPaymentId, cJSON_GetObjectItem(pData, "payment_id")->valuestring);

          if (pstPaymentCreate->eMethod == METHOD_CARD) {
            pstPaymentResponse->dTotalAmount = cJSON_GetObjectItem(pData, "total_amount")->valuedouble;
            pstPaymentResponse->dIncrease = cJSON_GetObjectItem(pData, "increase")->valuedouble;
          } 
          else if (pstPaymentCreate->eMethod == METHOD_PIX) {
            strcpy(pstPaymentResponse->szQrCode, cJSON_GetObjectItem(pData, "qr_code")->valuestring);
            strcpy(pstPaymentResponse->szLiteral, cJSON_GetObjectItem(pData, "literal")->valuestring);
          }
        }
      }
      cJSON_Delete(pJsonResponse);
    }
  }

  // Limpa recursos
  curl_easy_cleanup(pCurl);
  curl_slist_free_all(pHeaders);
  free(pszResponse);

  return pstPaymentResponse;
}

