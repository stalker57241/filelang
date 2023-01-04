# filelang
Language for file manipulating

BNF of language:
```
interface_expression:   FL_TOKEN_TYPE_INTERFACE <varname> <block_expression>
                    |   FL_TOKEN_TYPE_INTERFACE <varname> FL_TOKEN_TYPE_EXTENDS <varname.1>, <varname.2> <block_expression>
                    |   FL_TOKEN_TYPE_TEMPLATE FL_TOKEN_TYPE_CMP_LESS <varname> FL_TOKEN_TYPE_CMP_MORE FL_TOKEN_TYPE_INTERFACE <varname> <block_expression>
                    |   FL_TOKEN_TYPE_TEMPLATE FL_TOKEN_TYPE_CMP_LESS <varname> FL_TOKEN_TYPE_CMP_MORE FL_TOKEN_TYPE_INTERFACE <varname> FL_TOKEN_TYPE_EXTENDS <varname.1>, <varname.2> <block_expression> 
                    ;
structure_expression:   FL_TOKEN_TYPE_STRUCTURE <varname> <block_expression>
                    |   FL_TOKEN_TYPE_STRUCTURE <varname> FL_TOKEN_TYPE_IMPLEMENTS <varname> <block_expression>
                    ;
block_expression: FL_TOKEN_TYPE_CURVY_BRACKET_OPEN <expression.1> FL_TOKEN_TYPE_SEMICOLON <expression.2> FL_TOKEN_TYPE_SEMICOLON ... FL_TOKEN_TYPE_SEMICOLON <expression.n> FL_TOKEN_TYPE_SEMICOLON FL_TOKEN_TYPE_CURVY_BRACKET_CLOSE
call_expression: <varname> FL_TOKEN_TYPE_BRACKET_OPEN <expression.1>, <expression.2>, ..., <expression.n> FL_TOKEN_TYPE_BRACKET_CLOSE;
equal_expression:
                <varname> FL_TOKEN_TYPE_EQUAL <expression>
            |   <varname> FL_TOKEN_TYPE_OP_ADD_EQ <expression>
            |   <varname> FL_TOKEN_TYPE_OP_SUB_EQ <expression>
            |   <varname> FL_TOKEN_TYPE_OP_MUL_EQ <expression>
            |   <varname> FL_TOKEN_TYPE_OP_DIV_EQ <expression>
            |   <varname> FL_TOKEN_TYPE_OP_DWR_EQ <expression> 
            ;
method_definition:  <type> <varname> FL_TOKEN_TYPE_BRACKET_OPEN <expression.1>, <expression.2>, ..., <expression.n> FL_TOKEN_TYPE_BRACKET_CLOSE |
                    <method_definition> <block_expression>
definition: 
expression: <value>
          | <varname>
          | <expression> FL_TOKEN_TYPE_PLUS <expression>
          | <expression> FL_TOKEN_TYPE_MINUS <expression>
          | <expression> FL_TOKEN_TYPE_ASTERISK <expression>
          | <expression> FL_TOKEN_TYPE_SLASH <expression>
          | <expression> FL_TOKEN_TYPE_PERCENT <expression>
          ;
<command_expression>:
                    |   <expression> FL_TOKEN_TYPE_SEMICOLON
                    |   <equal_expression> FL_TOKEN_TYPE_SEMICOLON
                    |   <call_expression> FL_TOKEN_TYPE_SEMICOLON
                    |   <interface_expression> FL_TOKEN_TYPE_SEMICOLON
                    |   <method_definition> FL_TOKEN_TYP_SEMICOLON
                    ;
value:      FL_TOKEN_TYPE_VALUE;
varname:    FL_TOKEN_TYPE_VARNAME;
type:       <varname>
        |   FL_TOKEN_TYPE_INT
        |   FL_TOKEN_TYPE_FLOAT
        |   FL_TOKEN_TYPE_CHAR
        |   FL_TOKEN_TYPE_VOID
        ;
svarname:   <varname>
        |   <svarname> FL_TOKEN_TYPE_DOT <varname>
        |   <svarname> FL_TOKEN_TYPE_DOT <svarname>
        |   <svarname> FL_TOKEN_TYPE_ARROW <varname>
        |   <svarname> FL_TOKEN_TYPE_ARROW <svarname>
        ;
```
