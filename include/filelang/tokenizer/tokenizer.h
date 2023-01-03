#ifndef FILELANG_TOKENIZER_H
#define FILELANG_TOKENIZER_H

#include <filelang/types.h>
// function prefix is flt

typedef struct s_token FLtoken;

FLuint32_t fltCreateToken(FLuint8_t token_type, FLtoken* ptoken);
FLuint32_t fltTokenText(FLcstr text, FLtoken* ptoken);

FLuint32_t fltTokenize(FLcstr text, FLtoken** ptoken_arr);
FLuint32_t fltGetTokenType(FLcstr text, FLuint8_t* ptoken_type);
#endif