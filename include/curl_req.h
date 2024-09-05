/* 
*  curl_req.h - callbacks and setups for curl.
*
*  Written by Renato Fermi <repiazza@gmail.com> in August 2024
*  
**/
#ifndef CURL_REQ_H
  #define CURL_REQ_H

  #include <stdlib.h>
  #include <string.h>

  size_t zWriteResponse(void* pContents, size_t zSize, size_t zMemSize, void* pUserp) {
    size_t zTotalSize = zSize * zMemSize;
    char** pszResponse = (char**)pUserp;
    size_t zOldLen = *pszResponse ? strlen(*pszResponse) : 0;
    char* szNewData = (char*)malloc(zOldLen + zTotalSize + 1);

    if (szNewData == NULL) {
      return 0;
    }

    if (*pszResponse) {
      memcpy(szNewData, *pszResponse, zOldLen);
      free(*pszResponse);  // Libera a memória antiga
    }

    // Adiciona o novo conteúdo
    memcpy(szNewData + zOldLen, pContents, zTotalSize);
    *pszResponse = szNewData;

    return zTotalSize;
  }

#endif // CURL_REQ_H