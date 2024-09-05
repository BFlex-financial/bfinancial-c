/* 
*  payments.h - main structures and definitions.
*
*  Written by Renato Fermi <repiazza@gmail.com> in August 2024
*  
**/
#ifndef PAYMENTS_H
  #define PAYMENTS_H

  #include <stdlib.h>
  #include <string.h>

  #define MAX_LEN 256

  typedef struct STRUCT_PIX_CREATE {
    double dAmount;
    char szCustomerEmail[MAX_LEN];
  } STRUCT_PIX_CREATE;

  typedef struct STRUCT_CARD_CREATE {
    double dAmount;
    char szCustomerEmail[MAX_LEN];
    char szNumber[MAX_LEN];
    char szCvv[MAX_LEN];
    char szPayerName[MAX_LEN]; 
    char szPayerCpf[MAX_LEN];  
    int iExpirationMonth;      
    int iExpirationYear;       
  } STRUCT_CARD_CREATE;

  typedef enum ENUM_PAYMENT_METHOD {
    METHOD_CARD,
    METHOD_PIX
  } ENUM_PAYMENT_METHOD;

  typedef union UNION_PAYMENT_DATA {
    STRUCT_CARD_CREATE stCardCreate;
    STRUCT_PIX_CREATE stPixCreate;
  } UNION_PAYMENT_DATA;

  typedef struct STRUCT_PAYMENT_CREATE {
    ENUM_PAYMENT_METHOD eMethod;
    UNION_PAYMENT_DATA uData;
  } STRUCT_PAYMENT_CREATE;

  typedef struct STRUCT_PAYMENTS {
    char szApi[MAX_LEN];  
    char szAuth[MAX_LEN]; 
  } STRUCT_PAYMENTS;

  typedef struct STRUCT_CLIENT {
    char szAuth[MAX_LEN];  
    STRUCT_PAYMENTS stPayments; 
  } STRUCT_CLIENT;

  typedef struct STRUCT_PAYMENT_RESPONSE {
    char szPaymentId[MAX_LEN];
    double dTotalAmount;
    double dIncrease;          // Pode ser 0.0 se não houver aumento
    char szQrCode[MAX_LEN];    // Específico para PIX
    char szLiteral[MAX_LEN];   // Específico para PIX
  } STRUCT_PAYMENT_RESPONSE;

  // Prototypes
  STRUCT_CLIENT* pstClientLogin(const char* szAuth);
  STRUCT_PAYMENT_RESPONSE* pstPaymentCreate(STRUCT_PAYMENTS* pstPayments, const STRUCT_PAYMENT_CREATE* pstPaymentCreate);
  
#endif // PAYMENTS_H
