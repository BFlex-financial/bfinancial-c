/* 
*  spawn_mock.c - Mockup src for general payment methods.
*
*  Written by Renato Fermi <repiazza@gmail.com> in August 2024
*  
**/
#include <stdio.h>
#include "payments.h"

void vTestSpawn() {
  STRUCT_PIX_CREATE stPixCreate;
  stPixCreate.dAmount = 2.0;
  snprintf(stPixCreate.szCustomerEmail, MAX_LEN, "%s", "lucasdwbfff@gmail.com");

  printf(
"PixCreate:\n  Amount: %.2f\n  Customer Email: %s\n",
    stPixCreate.dAmount,
    stPixCreate.szCustomerEmail
  );
}

int main() {
  vTestSpawn();
  return 0;
}
