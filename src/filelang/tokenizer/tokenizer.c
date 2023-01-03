#include <filelang/tokenizer/tokenizer.h>
#include <filelang/types.h>
#include <filelang/tools.h>

#include <stdio.h>
#include <stdlib.h>
// TOKEN TABLE
//////////////////////////////////////////////////////////////////////
//  IDX         //  NAME                //  CONDITION               //
//////////////////////////////////////////////////////////////////////
//  0x00        //  NULL                //                          //  if cannot convert text to token
//  0x01        //  VARIABLE_NAME       //  [_A-Za-z][_A-Za-z0-9]*  //  contented in memory list
//  0x02        //  VALUE               //  [0-9]*[ifdc]?     //  suffix allows to understand type of value. Preprocessor calculates constant operations
//  0x03        //  SEMICOLON           //  ;                       //  Separates code strings
////////////////////KW////////////////////////////////////////////////
//  0x04        //  STRUCTURE           //  structure               //  creates list of some different data, that'll be saved in memory
//  0x05        //  INTERFACE           //  interface               //  Creates base for structures. If not implemented, it mustn't be written to memory. Interface may be used as argument in function like structure
//  0x06        //  TEMPLATE            //  template                //  May be used as argument in function like structure and type. Combines with INTERFACE: template<type> interface data {type value;}; structure infodata implements data<unsigned int> {};
//  0x07        //  TYPE_DEFINITION     //  typedef                 //  turns multiple keywords, structures and interfaces in one keyword: typedef unsigned int uint32_t; typedef structure s_infodata infodata; structure s_infodata may not be exists
//////////////////////////////////////////////////////////////////////
//  0x08        //  RETURN              //  return                  //  returns something from function. Stops working of it
//  0x09        //  NEW_LINE            //  \n                      //
//  0x0A        //                      //                          //
//  0x0B        //                      //                          //
//////////////////////////////////////////////////////////////////////
//  0x0C        //                      //                          //
//  0x0D        //                      //                          //
//  0x0E        //                      //                          //
//  0x0F        //                      //                          //
//////////////////////////////////////////////////////////////////////
//  0x10        //                      //                          //
//  0x11        //                      //                          //
//  0x12        //                      //                          //
//  0x13        //                      //                          //
////////////////////KW_MISC_DATA//////////////////////////////////////
//  0x14        //  UNSIGNED            //  unsigned                //  if added, than last bit in value turned in memory for value. unsigned int uses full dword for value
//  0x15        //  SIGNED              //  signed                  //  default. signed int uses 31 bits for value, and one bit for sign.
//  0x16        //  IMPLEMENTS          //  implements              //  allows to insert interface in structure. structure infodata implements data {};
//  0x17        //  EXTENDS             //  extends                 //  allows to append another interface by first. interface data {int a;}; interface extdata extends data {int b;}; // extdata <=> {int a; int b;}
////////////////////KW_TYPE///////////////////////////////////////////
//  0x18        //  TYPE_INT            //  int                     //  SIZE = 4 BYTE = 1 DWORD. int type
//  0x19        //  TYPE_FLOAT          //  float                   //  SIZE = 4 BYTE = 1 DWORD. float type.
//  0x1A        //  TYPE_CHAR           //  char                    //  SIZE = 1 BYTE. char type. Contains one char, or value less than 0x100
//  0x1B        //  TYPE_VOID           //  void                    //  SIZE = 1 BYTE. void type. Methods return it by default. Equals 0.
//////////////////////////////////////////////////////////////////////
//  0x1C        //  CURVY_BRACKET_OPEN  //  {                       //  
//  0x1D        //  CURVY_BRACKET_CLOSE //  }                       //  
//  0x1E        //  BRACKET_OPEN        //  (                       //  
//  0x1F        //  BRACKET_CLOSE       //  )                       //  
//////////////////////////////////////////////////////////////////////
//  0x20        //  SQUARE_BRACKET_OPEN //  [                       //  
//  0x21        //  SQUARE_BRACKET_CLOSE//  ]                       //  
//  0x22        //  AMPERSIDE           //  &                       //  
//  0x23        //  EQUAL               //  =                       //  
////////////////////VAL///////////////////////////////////////////////
//  0x24        //  PLUS                //  +                       //
//  0x25        //  MINUS               //  -                       //
//  0x26        //  ASTERISK            //  *                       //
//  0x27        //  SLASH               //  /                       //
//////////////////////////////////////////////////////////////////////
//  0x28        //  CMP_EQ              //  ==                      //
//  0x29        //  CMP_NEQ             //  !=                      //
//  0x2A        //  CMP_MORE            //  >                       //
//  0x2B        //  CMP_LESS            //  <                       //
//////////////////////////////////////////////////////////////////////
//  0x2C        //  CMP_LESS_OR_EQ      //  <=                      //
//  0x2D        //  CMP_MORE_OR_EQ      //  >=                      //
//  0x2E        //  LOGIC_AND           //  &&                      //
//  0x2F        //  LOGIC_OR            //  ||                      //
//////////////////////////////////////////////////////////////////////
//  0x30        //  LOGIC_NOT           //  !                       //
//  0x31        //  PERCENT             //  %                       //
//  0x32        //  INCREMENT           //  ++                      //
//  0x33        //  DECREMENT           //  --                      //
//////////////////////////////////////////////////////////////////////
//  0x34        //  OP_ADD_EQ           //  +=                      //
//  0x35        //  OP_SUB_EQ           //  -=                      //
//  0x36        //  OP_MUL_EQ           //  *=                      //
//  0x37        //  OP_DIV_EQ           //  /=                      //
//////////////////////////////////////////////////////////////////////
//  0x38        //  OP_DWR_EQ           //  %=                      //
//  0x39        //  COMM_SLINE          //  //                      //
//  0x3A        //  COMM_MLINE_OPEN     //  /*                      //
//  0x3B        //  COMM_MLINE_CLOSE    //  */                      //
//////////////////////////////////////////////////////////////////////
//  0x3C        //                      //                          //
//  0x3D        //                      //                          //
//  0x3E        //                      //                          //
//  0x3F        //                      //                          //
//////////////////////////////////////////////////////////////////////

#define FL_TOKEN_TYPE_NULL                  0x00 //
#define FL_TOKEN_TYPE_VARNAME               0x01 //
#define FL_TOKEN_TYPE_VALUE                 0x02 //
#define FL_TOKEN_TYPE_SEMICOLON             0x03 // ;

#define FL_TOKEN_TYPE_STRUCTURE             0x04 //
#define FL_TOKEN_TYPE_INTERFACE             0x05 //
#define FL_TOKEN_TYPE_TEMPLATE              0x06 //
#define FL_TOKEN_TYPE_TYPE_DEFINITION       0x07 //

#define FL_TOKEN_TYPE_RETURN                0x08 // 
#define FL_TOKEN_TYPE_NEW_LINE              0x09 // \n


#define FL_TOKEN_TYPE_UNSIGNED              0x14 // 
#define FL_TOKEN_TYPE_SIGNED                0x15 // 
#define FL_TOKEN_TYPE_IMPLEMENTS            0x16 // 
#define FL_TOKEN_TYPE_EXTENDS               0x17 // 

#define FL_TOKEN_TYPE_TYPE_INT              0x18 // 
#define FL_TOKEN_TYPE_TYPE_FLOAT            0x19 // 
#define FL_TOKEN_TYPE_TYPE_CHAR             0x1A // 
#define FL_TOKEN_TYPE_TYPE_VOID             0x1B // 

#define FL_TOKEN_TYPE_CURVY_BRACKET_OPEN    0x1C // 
#define FL_TOKEN_TYPE_CURVY_BRACKET_CLOSE   0x1D // 
#define FL_TOKEN_TYPE_BRACKET_OPEN          0x1E // 
#define FL_TOKEN_TYPE_BRACKET_CLOSE         0x1F // 

#define FL_TOKEN_TYPE_SQUARE_BRACKET_OPEN   0x20 // 
#define FL_TOKEN_TYPE_SQUARE_BRACKET_CLOSE  0x21 // 
#define FL_TOKEN_TYPE_AMPERSIDE             0x22 // 
#define FL_TOKEN_TYPE_EQUAL                 0x23 // 

#define FL_TOKEN_TYPE_PLUS                  0x24 // 
#define FL_TOKEN_TYPE_MINUS                 0x25 // 
#define FL_TOKEN_TYPE_ASTERISK              0x26 // 
#define FL_TOKEN_TYPE_SLASH                 0x27 // 

#define FL_TOKEN_TYPE_CMP_EQ                0x28 // 
#define FL_TOKEN_TYPE_CMP_NEQ               0x29 // 
#define FL_TOKEN_TYPE_CMP_MORE              0x2A // 
#define FL_TOKEN_TYPE_CMP_LESS              0x2B // 

#define FL_TOKEN_TYPE_CMP_MORE_OR_EQ        0x2C // 
#define FL_TOKEN_TYPE_CMP_LESS_OR_EQ        0x2D // 
#define FL_TOKEN_TYPE_LOGIC_AND             0x2E // 
#define FL_TOKEN_TYPE_LOGIC_OR              0x2F // 

#define FL_TOKEN_TYPE_LOGIC_NOT             0x30 // !
#define FL_TOKEN_TYPE_PERCENT               0x31 // %
#define FL_TOKEN_TYPE_INCREMENT             0x32 // ++
#define FL_TOKEN_TYPE_DECREMENT             0x33 // --

#define FL_TOKEN_TYPE_OP_ADD_EQ             0x34 // +=
#define FL_TOKEN_TYPE_OP_SUB_EQ             0x35 // -=
#define FL_TOKEN_TYPE_OP_MUL_EQ             0x36 // *=
#define FL_TOKEN_TYPE_OP_DIV_EQ             0x37 // /=

#define FL_TOKEN_TYPE_OP_DWR_EQ             0x38 // %= // DWR = Divide With remainder
#define FL_TOKEN_TYPE_COMM_SLINE            0x39 // //
#define FL_TOKEN_TYPE_COMM_MLINE_OPEN       0x3A // /*
#define FL_TOKEN_TYPE_COMM_MLINE_CLOSE      0x3B // */
struct s_token {
    FLuint8_t token_type;
    char text[256];
};

FLuint32_t fltCreateToken(FLuint8_t token_type, FLtoken* ptoken) {
    ptoken->token_type = token_type;
    return 0;
}

FLuint32_t fltTokenText(char text[256], FLtoken* ptoken) {
    strcpy(&(ptoken->text), text);
    return 0;
}


unsigned int fltTokenize(FLcstr text, FLtoken** ptoken_arr) {
    int seps = 0;
    FLuint32_t tlen = 0;
    while (text[tlen] != '\0') {
        tlen++;
    }
    FLuint32_t counter = 0;

    printf("counting\n");
    for (FLuint32_t i = 1U; i < tlen; i++) {
        #define CURR_CHAR text[i]
        #define PREV_CHAR text[i - 1]
        switch (PREV_CHAR)
        {
        case ' ':
            counter++;
            break;
        case '\t':
            counter++;
            break;
        case '\n':
            counter++;
            break;
        case '+':
            counter++;
            if ((CURR_CHAR == '=') ||
                (CURR_CHAR == '+')) {
                i++;
            }
            counter++;
            break;
        case '-':
            counter++;
            if ((CURR_CHAR == '=') ||
                (CURR_CHAR == '-')) {
                i++;
            }
            counter++;
            break;
        case '*':
            counter++;
            if (
                (CURR_CHAR == '=') ||
                (CURR_CHAR == '/')
            ) {
                i++;
            }
            counter++;
            break;
        case '/':
            counter++;
            if ((CURR_CHAR == '=') ||
                (CURR_CHAR == '/') ||
                (CURR_CHAR == '*')
            ) {
                i++;
            }
            counter++;
            break;
        case '%':
            counter++;
            if (CURR_CHAR == '=') {
                i++;
            }
            counter++;
            break;
        case '<':
            counter++;
            if (CURR_CHAR == '=') {
                i++;
            }
            counter++;
            break;
        case '>':
            counter++;
            if (CURR_CHAR == '=') {
                i++;
            }
            counter++;
            break;
        case '!':
            counter++;
            if (CURR_CHAR == '=') {
                i++;
            } 
            counter++;
            break;
        case ';':
            counter++;
            counter++;
            break;
        case '{':
            counter++;
            counter++;
            break;
        case '}':
            counter++;
            counter++;
            break;
        case '[':
            counter++;
            counter++;
            break;
        case ']':
            counter++;
            counter++;
            break;
        case '(':
            counter++;
            counter++;
            break;
        case ')':
            counter++;
            counter++;
            break;
        case '&':
            counter++;
            counter++;
            break;
        case '=':
            counter++;
            if (CURR_CHAR == '=') {
                i++;
            }
            counter++;
            break;
        }
    }
    printf("counter: 0x%x\n", counter);
    
    *ptoken_arr = (FLtoken*) malloc(sizeof(FLtoken) * counter);
    FLuint32_t  token_id = 0,
                lw_id = 0;
    
    char last_word[0x100];
    #define NULL_CHAR 0x00
    for (FLuint32_t i = 0U; i < 0x100; i++) {
        last_word[i] = NULL_CHAR;
    }

    for (FLuint32_t i = 1U; i < tlen; i++) {
        #define CURR_CHAR text[i]
        #define PREV_CHAR text[i - 1]
        FLtoken token;
        FLuint8_t token_type;
        // printf("prev: %c\tcurr: %c\n", PREV_CHAR, CURR_CHAR);
        switch (PREV_CHAR)
        {
        case ' ':
            if(lw_id > 0) {
                fltGetTokenType(last_word, &token_type);
                fltCreateToken(token_type, &token);
                fltTokenText(last_word, &token);
                (*ptoken_arr)[token_id++] = token;
                lw_id = 0;
                for(int k = 0; k < 0x100; k++) {
                    last_word[k] = NULL_CHAR;
                }
            }
            break;
        case '\t':
            if (lw_id > 0) {
                fltGetTokenType(last_word, &token_type);
                fltCreateToken(token_type, &token);
                fltTokenText(last_word, &token);
                (*ptoken_arr)[token_id++] = token;
                lw_id = 0;
                for(int k = 0; k < 0x100; k++) {
                    last_word[k] = NULL_CHAR;
                }
            }
            break;
        case '\n':
            if (lw_id > 0) {
                fltGetTokenType(last_word, &token_type);
                fltCreateToken(token_type, &token);
                fltTokenText(last_word, &token);
                (*ptoken_arr)[token_id++] = token;
                lw_id = 0;
                for(int k = 0; k < 0x100; k++) {
                    last_word[k] = NULL_CHAR;
                }
            }
            fltCreateToken(FL_TOKEN_TYPE_NEW_LINE, &token);
            fltTokenText("+=", &token);
            (*ptoken_arr)[token_id++] = token;
            break;
        case '+':
            if (lw_id > 0) {
                fltGetTokenType(last_word, &token_type);
                fltCreateToken(token_type, &token);
                fltTokenText(last_word, &token);
                (*ptoken_arr)[token_id++] = token;
                lw_id = 0;
                for(int k = 0; k < 0x100; k++) {
                    last_word[k] = NULL_CHAR;
                }
            }
            if (CURR_CHAR == '=') {
                fltCreateToken(FL_TOKEN_TYPE_OP_ADD_EQ, &token);
                fltTokenText("+=", &token);
                i++;
            }
            else if (CURR_CHAR == '+') {
                fltCreateToken(FL_TOKEN_TYPE_INCREMENT, &token);
                fltTokenText("++", &token);
                i++;
            }
            else {
                fltCreateToken(FL_TOKEN_TYPE_PLUS, &token);
                fltTokenText("+", &token);
            }
            (*ptoken_arr)[token_id++] = token;
            break;
        case '-':
            if (lw_id > 0) {
                fltGetTokenType(last_word, &token_type);
                fltCreateToken(token_type, &token);
                fltTokenText(last_word, &token);
                (*ptoken_arr)[token_id++] = token;
                lw_id = 0;
                for(int k = 0; k < 0x100; k++) {
                    last_word[k] = NULL_CHAR;
                }
            }
            if (CURR_CHAR == '=') {
                fltCreateToken(FL_TOKEN_TYPE_OP_SUB_EQ, &token);
                fltTokenText("-=", &token);
                i++;
            }
            else if (CURR_CHAR == '-') {
                fltCreateToken(FL_TOKEN_TYPE_DECREMENT, &token);
                fltTokenText("--", &token);
                i++;
            }
            else {
                fltCreateToken(FL_TOKEN_TYPE_MINUS, &token);
                fltTokenText("-", &token);
            }
            (*ptoken_arr)[token_id++] = token;
            break;
        case '*':
            if (lw_id > 0) {
                fltGetTokenType(last_word, &token_type);
                fltCreateToken(token_type, &token);
                fltTokenText(last_word, &token);
                (*ptoken_arr)[token_id++] = token;
                lw_id = 0;
                for(int k = 0; k < 0x100; k++) {
                    last_word[k] = NULL_CHAR;
                }
            }
            if (CURR_CHAR == '=') {
                fltCreateToken(FL_TOKEN_TYPE_OP_SUB_EQ, &token);
                fltTokenText("*=", &token);
                i++;
            }
            else if (CURR_CHAR == '/') {
                fltCreateToken(FL_TOKEN_TYPE_COMM_MLINE_CLOSE, &token);
                fltTokenText("*/", &token);
                i++;
            }
            else {
                fltCreateToken(FL_TOKEN_TYPE_MINUS, &token);
                fltTokenText("*", &token);
            }
            (*ptoken_arr)[token_id++] = token;
            break;
        case '/':
            if (lw_id > 0) {
                fltGetTokenType(last_word, &token_type);
                fltCreateToken(token_type, &token);
                fltTokenText(last_word, &token);
                (*ptoken_arr)[token_id++] = token;
                lw_id = 0;
                for(int k = 0; k < 0x100; k++) {
                    last_word[k] = NULL_CHAR;
                }
            }
            if (CURR_CHAR == '=') {
                fltCreateToken(FL_TOKEN_TYPE_OP_SUB_EQ, &token);
                fltTokenText("*=", &token);
                i++;
            }
            else if (CURR_CHAR == '/') {
                fltCreateToken(FL_TOKEN_TYPE_COMM_SLINE, &token);
                fltTokenText("//", &token);
                i++;
            }
            else if (CURR_CHAR == '*') {
                fltCreateToken(FL_TOKEN_TYPE_COMM_MLINE_OPEN, &token);
                fltTokenText("/*", &token);
                i++;
            }
            else {
                fltCreateToken(FL_TOKEN_TYPE_MINUS, &token);
                fltTokenText("/", &token);
            }
            (*ptoken_arr)[token_id++] = token;
            break;
        case '%':
            if (lw_id > 0) {
                fltGetTokenType(last_word, &token_type);
                fltCreateToken(token_type, &token);
                fltTokenText(last_word, &token);
                (*ptoken_arr)[token_id++] = token;
                lw_id = 0;
                for(int k = 0; k < 0x100; k++) {
                    last_word[k] = NULL_CHAR;
                }
            }
            if (CURR_CHAR == '=') {
                fltCreateToken(FL_TOKEN_TYPE_OP_DWR_EQ, &token);
                fltTokenText("%=", &token);
                i++;
            }
            else {
                fltCreateToken(FL_TOKEN_TYPE_PERCENT, &token);
                fltTokenText("%", &token);
            }
            (*ptoken_arr)[token_id++] = token;
            break;
        case '<':
            if (lw_id > 0) {
                fltGetTokenType(last_word, &token_type);
                fltCreateToken(token_type, &token);
                fltTokenText(last_word, &token);
                (*ptoken_arr)[token_id++] = token;
                lw_id = 0;
                for(int k = 0; k < 0x100; k++) {
                    last_word[k] = NULL_CHAR;
                }
            }
            if (CURR_CHAR == '=') {
                fltCreateToken(FL_TOKEN_TYPE_CMP_LESS_OR_EQ, &token);
                fltTokenText("<=", &token);
                i++;
            }
            else {
                fltCreateToken(FL_TOKEN_TYPE_CMP_LESS, &token);
                fltTokenText("<", &token);
            }
            (*ptoken_arr)[token_id++] = token;
            break;
        case '>':
            if (lw_id > 0) {
                fltGetTokenType(last_word, &token_type);
                fltCreateToken(token_type, &token);
                fltTokenText(last_word, &token);
                (*ptoken_arr)[token_id++] = token;
                lw_id = 0;
                for(int k = 0; k < 0x100; k++) {
                    last_word[k] = NULL_CHAR;
                }
            }
            if (CURR_CHAR == '=') {
                fltCreateToken(FL_TOKEN_TYPE_CMP_MORE_OR_EQ, &token);
                fltTokenText(">=", &token);
                i++;
            }
            else {
                fltCreateToken(FL_TOKEN_TYPE_CMP_MORE, &token);
                fltTokenText(">", &token);
            }
            (*ptoken_arr)[token_id++] = token;
            break;
        case '!':
            if (lw_id > 0) {
                fltGetTokenType(last_word, &token_type);
                fltCreateToken(token_type, &token);
                fltTokenText(last_word, &token);
                (*ptoken_arr)[token_id++] = token;
                lw_id = 0;
                for(int k = 0; k < 0x100; k++) {
                    last_word[k] = NULL_CHAR;
                }
            }
            if (CURR_CHAR == '=') {
                fltCreateToken(FL_TOKEN_TYPE_CMP_NEQ, &token);
                fltTokenText("!=", &token);
                i++;
            }
            else {
                fltCreateToken(FL_TOKEN_TYPE_LOGIC_NOT, &token);
                fltTokenText("!", &token);
            }
            (*ptoken_arr)[token_id++] = token;
            break;
        case ';':
            if (lw_id > 0) {
                fltGetTokenType(last_word, &token_type);
                fltCreateToken(token_type, &token);
                fltTokenText(last_word, &token);
                (*ptoken_arr)[token_id++] = token;
                lw_id = 0;
                for(int k = 0; k < 0x100; k++) {
                    last_word[k] = NULL_CHAR;
                }
            }
            fltCreateToken(FL_TOKEN_TYPE_SEMICOLON, &token);
            fltTokenText(";", &token);
            (*ptoken_arr)[token_id++] = token;
            break;
        case '{':
            if (lw_id > 0) {
                fltGetTokenType(last_word, &token_type);
                fltCreateToken(token_type, &token);
                fltTokenText(last_word, &token);
                (*ptoken_arr)[token_id++] = token;
                lw_id = 0;
                for(int k = 0; k < 0x100; k++) {
                    last_word[k] = NULL_CHAR;
                }
            }
            fltCreateToken(FL_TOKEN_TYPE_CURVY_BRACKET_OPEN, &token);
            fltTokenText("{", &token);
            (*ptoken_arr)[token_id++] = token;
            break;
        case '}':
            if (lw_id > 0) {
                fltGetTokenType(last_word, &token_type);
                fltCreateToken(token_type, &token);
                fltTokenText(last_word, &token);
                (*ptoken_arr)[token_id++] = token;
                lw_id = 0;
                for(int k = 0; k < 0x100; k++) {
                    last_word[k] = NULL_CHAR;
                }
            }
            fltCreateToken(FL_TOKEN_TYPE_CURVY_BRACKET_CLOSE, &token);
            fltTokenText("}", &token);
            (*ptoken_arr)[token_id++] = token;
            break;
        case '[':
            if (lw_id > 0) {
                fltGetTokenType(last_word, &token_type);
                fltCreateToken(token_type, &token);
                fltTokenText(last_word, &token);
                (*ptoken_arr)[token_id++] = token;
                lw_id = 0;
                for(int k = 0; k < 0x100; k++) {
                    last_word[k] = NULL_CHAR;
                }
            }
            fltCreateToken(FL_TOKEN_TYPE_SQUARE_BRACKET_OPEN, &token);
            fltTokenText("[", &token);
            (*ptoken_arr)[token_id++] = token;
            break;
        case ']':
            if (lw_id > 0) {
                fltGetTokenType(last_word, &token_type);
                fltCreateToken(token_type, &token);
                fltTokenText(last_word, &token);
                (*ptoken_arr)[token_id++] = token;
                lw_id = 0;
                for(int k = 0; k < 0x100; k++) {
                    last_word[k] = NULL_CHAR;
                }
            }
            fltCreateToken(FL_TOKEN_TYPE_SQUARE_BRACKET_CLOSE, &token);
            fltTokenText("]", &token);
            (*ptoken_arr)[token_id++] = token;
            break;
        case '(':
            if (lw_id > 0) {
                fltGetTokenType(last_word, &token_type);
                fltCreateToken(token_type, &token);
                fltTokenText(last_word, &token);
                (*ptoken_arr)[token_id++] = token;
                lw_id = 0;
                for(int k = 0; k < 0x100; k++) {
                    last_word[k] = NULL_CHAR;
                }
            }
            fltCreateToken(FL_TOKEN_TYPE_BRACKET_OPEN, &token);
            fltTokenText("(", &token);
            (*ptoken_arr)[token_id++] = token;
            break;
        case ')':
            if (lw_id > 0) {
                fltGetTokenType(last_word, &token_type);
                fltCreateToken(token_type, &token);
                fltTokenText(last_word, &token);
                (*ptoken_arr)[token_id++] = token;
                lw_id = 0;
                for(int k = 0; k < 0x100; k++) {
                    last_word[k] = NULL_CHAR;
                }
            }
            fltCreateToken(FL_TOKEN_TYPE_BRACKET_CLOSE, &token);
            fltTokenText(")", &token);
            (*ptoken_arr)[token_id++] = token;
            break;
        case '&':
            if (lw_id > 0) {
                fltGetTokenType(last_word, &token_type);
                fltCreateToken(token_type, &token);
                fltTokenText(last_word, &token);
                (*ptoken_arr)[token_id++] = token;
                lw_id = 0;
                for(int k = 0; k < 0x100; k++) {
                    last_word[k] = NULL_CHAR;
                }
            }
            fltCreateToken(FL_TOKEN_TYPE_AMPERSIDE, &token);
            fltTokenText("&", &token);
            (*ptoken_arr)[token_id++] = token;
            break;
        case '=':
            if (lw_id > 0) {
                fltGetTokenType(last_word, &token_type);
                fltCreateToken(token_type, &token);
                fltTokenText(last_word, &token);
                (*ptoken_arr)[token_id++] = token;
                lw_id = 0;
                for(int k = 0; k < 0x100; k++) {
                    last_word[k] = NULL_CHAR;
                }
            }
            if (CURR_CHAR == '=') {
                fltCreateToken(FL_TOKEN_TYPE_CMP_EQ, &token);
                fltTokenText("==", &token);
            }
            else {
                fltCreateToken(FL_TOKEN_TYPE_EQUAL, &token);
                fltTokenText("=", &token);
            }
            (*ptoken_arr)[token_id++] = token;
            break;
        default:
            last_word[lw_id++] = PREV_CHAR;
            // printf("lword: %s\n", last_word);
            if (lw_id > 0xFF) {
                printf("buffer overflow\n");
                return 2;
            }
            break;
        }
    }
    *ptoken_arr = realloc(*ptoken_arr, token_id * sizeof(FLtoken));
    FLtoken t;
    for(int i = 0; i < token_id; i++) {
        t = (*ptoken_arr)[i];
        if (t.token_type == FL_TOKEN_TYPE_NEW_LINE) {
            printf("0x%x\t| 0x0%x:\\n\n", i, t.token_type);
            
        }
        else {
                if (t.token_type < 0x10) printf("0x%x\t| 0x0%x:%s\n", i, t.token_type, t.text);
                else printf("0x%x\t| 0x%x:%s\n", i, t.token_type, t.text);
            }
        }
    return 0;
}

FLuint32_t fltGetTokenType(FLcstr text, FLuint8_t* ptoken_type) {
            if (fltStrCmp(text, "structure")) {*ptoken_type = FL_TOKEN_TYPE_STRUCTURE;}
    else    if (fltStrCmp(text, "interface")) {*ptoken_type = FL_TOKEN_TYPE_INTERFACE;}
    else    if (fltStrCmp(text, "template")) {*ptoken_type = FL_TOKEN_TYPE_TEMPLATE;}
    else    if (fltStrCmp(text, "typedef")) {*ptoken_type = FL_TOKEN_TYPE_TYPE_DEFINITION;}
    else    if (fltStrCmp(text, "return")) {*ptoken_type = FL_TOKEN_TYPE_RETURN;}
    else    if (fltStrCmp(text, "unsigned")) {*ptoken_type = FL_TOKEN_TYPE_UNSIGNED;}
    else    if (fltStrCmp(text, "signed")) {*ptoken_type = FL_TOKEN_TYPE_SIGNED;}
    else    if (fltStrCmp(text, "implements")) {*ptoken_type = FL_TOKEN_TYPE_IMPLEMENTS;}
    else    if (fltStrCmp(text, "extends")) {*ptoken_type = FL_TOKEN_TYPE_EXTENDS;}
    else    if (fltStrCmp(text, "int")) {*ptoken_type = FL_TOKEN_TYPE_TYPE_INT;}
    else    if (fltStrCmp(text, "float")) {*ptoken_type = FL_TOKEN_TYPE_TYPE_FLOAT;}
    else    if (fltStrCmp(text, "char")) {*ptoken_type = FL_TOKEN_TYPE_TYPE_CHAR;}
    else    if (fltStrCmp(text, "void")) {*ptoken_type = FL_TOKEN_TYPE_TYPE_VOID;}
    else    {
        int tlen = strlen(text);
        FLuint8_t type = FL_TOKEN_TYPE_NULL;
        for(int i = 0; i < tlen; i++) {
            if (type == FL_TOKEN_TYPE_NULL) {
                if (text[i] >= '0' && text[i] <= '9') {
                    type = FL_TOKEN_TYPE_VALUE;
                }
                else if (text[i] == '_' || (text[i] >= 'a' && text[i] <= 'z')) {
                    type = FL_TOKEN_TYPE_VARNAME;
                }
            }
            else if (type == FL_TOKEN_TYPE_VALUE) {
                if (text[i] >= '0' && text[i] <= '9') {
                    type = FL_TOKEN_TYPE_VALUE;
                }
                else {
                    *ptoken_type = FL_TOKEN_TYPE_NULL;
                    return 0x01;
                }
            }
            else if (type == FL_TOKEN_TYPE_VARNAME) {
                if (text[i] == '_' || (text[i] >= 'a' && text[i] <= 'z')) {
                    type = FL_TOKEN_TYPE_VARNAME;
                }
                else {
                    *ptoken_type = FL_TOKEN_TYPE_NULL;
                    return 0x01;
                }
            }
        }
        *ptoken_type = type;
        if (type == FL_TOKEN_TYPE_NULL) {
            return 0x01;
        }
    }
    return 0x00;
}
