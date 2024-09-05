/* 
*  media_mock.c - Mockup src for general payment methods.
*
*  Written by Renato Fermi <repiazza@gmail.com> in August 2024
*  
**/

#include <stdio.h>
#include "payments.h"

void vTestCard() {
  STRUCT_CLIENT* pstClient;
  STRUCT_PAYMENT_RESPONSE* pstPaymentResponse;
  STRUCT_PAYMENT_CREATE stPaymentCreate;
  STRUCT_CARD_CREATE* pstCardCreate = &stPaymentCreate.uData.stCardCreate; // Ponteiro para STRUCT_CARD_CREATE

  if ( (pstClient = pstClientLogin("admin")) == NULL) {
    printf("Erro ao iniciar o cliente.\n");
    return;
  }

  stPaymentCreate.eMethod = METHOD_CARD;

  pstCardCreate->dAmount = 22.0;
  snprintf(pstCardCreate->szCustomerEmail, MAX_LEN, "%s", "test@gmail.com");
  snprintf(pstCardCreate->szNumber, MAX_LEN, "%s", "5031433215406351");
  snprintf(pstCardCreate->szCvv, MAX_LEN, "%s", "123");
  snprintf(pstCardCreate->szPayerName, MAX_LEN, "%s", "test user");
  snprintf(pstCardCreate->szPayerCpf, MAX_LEN, "%s", "12345678909");
  pstCardCreate->iExpirationMonth = 11;
  pstCardCreate->iExpirationYear = 2025;

  pstPaymentResponse = pstPaymentCreate(&pstClient->stPayments, &stPaymentCreate);
  if (pstPaymentResponse) {
    printf(
      "Payment Response:\n"
      "\tPayment ID: %s\n"
      "\tTotal Amount: %.2f\n", 
      pstPaymentResponse->szPaymentId, 
      pstPaymentResponse->dTotalAmount
    );
    free(pstPaymentResponse); 
  } else {
    printf("Falhou ao criar o pagamento.\n");
  }
  free(pstClient); 
}

void vTestPix() {
  STRUCT_CLIENT* pstClient;
  STRUCT_PAYMENT_CREATE stPaymentCreate;
  STRUCT_PAYMENT_RESPONSE* pstPaymentResponse;
  STRUCT_PIX_CREATE* pstPixCreate = &stPaymentCreate.uData.stPixCreate; // Ponteiro para STRUCT_PIX_CREATE

  if ( (pstClient = pstClientLogin("admin")) == NULL) {
    printf("Erro ao iniciar o cliente.\n");
    return;
  }

  stPaymentCreate.eMethod = METHOD_PIX;

  pstPixCreate->dAmount = 22.0;
  snprintf(pstPixCreate->szCustomerEmail, MAX_LEN, "%s", "test@gmail.com");

  pstPaymentResponse = pstPaymentCreate(&pstClient->stPayments, &stPaymentCreate);
  if (pstPaymentResponse) {
    printf(
      "Payment Response:\n"
      "\tPayment ID: %s\n"
      "\tTotal Amount: %.2f\n", 
      pstPaymentResponse->szPaymentId, 
      pstPaymentResponse->dTotalAmount
    );
    free(pstPaymentResponse);  // Libera a memória alocada
  } 
  else {
    printf("Erro ao gerar o pagamento.\n");
  }

  free(pstClient);  // Libera a memória alocada para o cliente
}

int main() {
  printf("\na\n");
  vTestPix();
  vTestCard();
  printf("b\n");
  return 0;
}
