    primary_expression : IDENTIFIER
        | CONSTANT
        | STRINGLITERAL
        | '(' expression ')'
    

    postfix_expression : primary_expression
        | primary_expression '[' expression ']' [postfix_expression]
        | primary_expression '(' ')' [postfix_expression]
        | primary_expression '(' argument_expression_list ')' [postfix_expression]
        | primary_expression '.' IDENTIFIER [postfix_expression]
        | primary_expression '->' IDENTIFIER [postfix_expression]
        | primary_expression '++' [postfix_expression]
        | primary_expression '--' [postfix_expression]
        | '(' type_name ')' '{' initializer_list [','] '}' '[' expression ']' [postfix_expression]
        | '(' type_name ')' '{' initializer_list [','] '}' '(' ')' [postfix_expression]
        | '(' type_name ')' '{' initializer_list [','] '}' '(' argument_expression_list ')' [postfix_expression]
        | '(' type_name ')' '{' initializer_list [','] '}' '.' IDENTIFIER [postfix_expression]
        | '(' type_name ')' '{' initializer_list [','] '}' '->' IDENTIFIER [postfix_expression]
        | '(' type_name ')' '{' initializer_list [','] '}' '++' [postfix_expression]
        | '(' type_name ')' '{' initializer_list [','] '}' '--' [postfix_expression]

    argument_expression_list : assignment_expression {',' assignment_expression}
    

    unary_expression : postfix_expression
        | '++' unary_expression
        | '--' unary_expression
        | unary_operator cast_expression
        | 'sizeof' unary_expression
        | 'sizeof' '(' type_name ')'
    

    unary_operator : '&'
        | '*'
        | '+'
        | '-'
        | '~'
        | '!'
    

    cast_expression : unary_expression
        | '(' type_name ')' cast_expression
    

    multiplicative_expression : cast_expression
        | cast_expression '*' multiplicative_expression 
        | cast_expression '/' multiplicative_expression 
        | cast_expression '%' multiplicative_expression 
    

    additive_expression : multiplicative_expression
        | multiplicative_expression '+' additive_expression 
        | multiplicative_expression '-' additive_expression 
    

    shift_expression : additive_expression
        | additive_expression '<<' shift_expression 
        | additive_expression '>>' shift_expression 
    

    relational_expression : shift_expression
        | shift_expression '<' relational_expression 
        | shift_expression '>' relational_expression 
        | shift_expression '<=' relational_expression 
        | shift_expression '>=' relational_expression 
    

    equality_expression : relational_expression
        | relational_expression '==' equality_expression 
        | relational_expression '!=' equality_expression 
    

    and_expression : equality_expression ['&' and_expression ]
    

    exclusive_or_expression : and_expression ['^' exclusive_or_expression]
    

    inclusive_or_expression : exclusive_or_expression ['|' inclusive_or_expression ]
    

    logical_and_expression : inclusive_or_expression ['&&' logical_and_expression ]
    

    logical_or_expression : logical_and_expression ['||' logical_or_expression]
    

    conditional_expression : logical_or_expression ['?' expression ':' conditional_expression]
    

    assignment_expression : conditional_expression
        | unary_expression assignment_operator assignment_expression
    

    assignment_operator : '='
        | '*='
        | '/='
        | '%='
        | '+='
        | '-='
        | '<<='
        | '>>='
        | '&='
        | '^='
        | '|='
    

    expression : assignment_expression { ',' assignment_expression}
    

    constant_expression : conditional_expression
    

    declaration : declaration_specifiers ';'
        | declaration_specifiers init_declarator_list ';'
    

    declaration_specifiers : storage_class_specifier
        | storage_class_specifier declaration_specifiers
        | type_specifier
        | type_specifier declaration_specifiers
        | type_qualifier
        | type_qualifier declaration_specifiers
        | function_specifier
        | function_specifier declaration_specifiers
    

    init_declarator_list : init_declarator {',' init_declarator}
    

    init_declarator : declarator ['=' initializer]
    

    storage_class_specifier : 'typedef'
        | 'extern'
        | 'static'
        | 'auto'
        | 'register'
    

    type_specifier : 'void'
        | 'char'
        | 'short'
        | 'int'
        | 'long'
        | 'float'
        | 'double'
        | 'signed'
        | 'unsigned'
        | '_Bool'
        | '_Complex'
        | struct_or_union_specifier
        | enum_specifier
    

    struct_or_union_specifier : struct_or_union '{' struct_declaration_list '}'
        | struct_or_union IDENTIFIER ['{' struct_declaration_list '}']
    

    struct_or_union : 'struct'
        | 'union'
    

    struct_declaration_list : struct_declaration {struct_declaration}

    struct_declaration : specifier_qualifier_list struct_declarator_list
    

    specifier_qualifier_list : type_specifier [specifier_qualifier_list]
        | type_qualifier [specifier_qualifier_list]
    

    struct_declarator_list : struct_declarator {',' struct_declarator}
    

    struct_declarator : declarator
        | ':' constant_expression
        | declarator ':' constant_expression
    

    enum_specifier : 'enum' '{' enumerator_list '}'
        | 'enum' IDENTIFIER '{' enumerator_list '}'
        | 'enum'  '{' enumerator_list ',' '}'
        | 'enum' IDENTIFIER '{' enumerator_list ',' '}'
        | 'enum' IDENTIFIER
    

    enumerator_list : enumerator {',' enumerator}
    

    enumerator : IDENTIFIER
        | IDENTIFIER '=' constant_expression
    

    type_qualifier : 'const'
        | 'restrict'
        | 'volatile'
    

    function_specifier : 'inline'
    

    declarator : direct_declarator
        | pointer direct_declarator
    

    direct_declarator : IDENTIFIER
        | '(' declarator ')'
        | IDENTIFIER '[' ']' [direct_declarator]
        | IDENTIFIER '[' assignment_expression ']' [direct_declarator]
        | IDENTIFIER '[' type_qualifier_list  ']' [direct_declarator]
        | IDENTIFIER '[' type_qualifier_list assignment_expression ']' [direct_declarator]
        | IDENTIFIER '[' 'static' assignment_expression ']' [direct_declarator]
        | IDENTIFIER '[' 'static' type_qualifier_list assignment_expression ']' [direct_declarator]
        | IDENTIFIER '[' type_qualifier_list 'static' assignment_expression ']' [direct_declarator]
        | IDENTIFIER '[' '*' ']' [direct_declarator]
        | IDENTIFIER '[' type_qualifier_list '*' ']' [direct_declarator]
        | IDENTIFIER '(' parameter_type_list ')' [direct_declarator]
        | IDENTIFIER '(' ')' [direct_declarator]
        | IDENTIFIER '(' identifier_list ')' [direct_declarator]
        | '(' declarator ')' '[' ']' [direct_declarator]
        | '(' declarator ')' '[' assignment_expression ']' [direct_declarator]
        | '(' declarator ')' '[' type_qualifier_list  ']' [direct_declarator]
        | '(' declarator ')' '[' type_qualifier_list assignment_expression ']' [direct_declarator]
        | '(' declarator ')' '[' 'static' assignment_expression ']' [direct_declarator]
        | '(' declarator ')' '[' 'static' type_qualifier_list assignment_expression ']' [direct_declarator]
        | '(' declarator ')' '[' type_qualifier_list 'static' assignment_expression ']' [direct_declarator]
        | '(' declarator ')' '[' '*' ']' [direct_declarator]
        | '(' declarator ')' '[' type_qualifier_list '*' ']' [direct_declarator]
        | '(' declarator ')' '(' parameter_type_list ')' [direct_declarator]
        | '(' declarator ')' '(' ')' [direct_declarator]
        | '(' declarator ')' '(' identifier_list ')' [direct_declarator]
    

    pointer : '*'
        | '*' type_qualifier_list
        | '*' pointer
        | '*' type_qualifier_list pointer
    

    type_qualifier_list : type_qualifier {type_qualifier}
    

    parameter_type_list : parameter_list
        | parameter_list ',' '...'
    

    parameter_list : parameter_declaration {',' parameter_declaration}
    

    parameter_declaration : declaration_specifiers declarator
        | declaration_specifiers
        | declaration_specifiers abstract_declarator
    

    identifier_list : IDENTIFIER {',' IDENTIFIER}
    

    type_name : specifier_qualifier_list
        | specifier_qualifier_list abstract_declarator
    

    abstract_declarator : pointer
        | direct_abstract_declarator
        | pointer direct_abstract_declarator
    

    direct_abstract_declarator : '(' abstract_declarator ')' [direct_abstract_declarator]
        | '[' ']' [direct_abstract_declarator]
        | '[' assignment_expression ']' [direct_abstract_declarator]
        | '[' '*' ']' [direct_abstract_declarator]
        | '(' ')' [direct_abstract_declarator]
        | '(' parameter_type_list ')' [direct_abstract_declarator]
        

    

    initializer : assignment_expression
        | '{' initializer_list '}'
        | '{' initializer_list ',' '}'
    

    initializer_list : initializer [',' initializer_list]
        | designation initializer [',' initializer_list]

    designation : designator_list '='
    

    designator_list : designator {designator}
    

    designator : '[' constant_expression ']'
        | '.' IDENTIFIER
    

    statement : labeled_statement
        | compound_statement
        | expression_statement
        | selection_statement
        | iteration_statement
        | jump_statement
    

    labeled_statement : IDENTIFIER ':' statement
        | 'case' constant_expression ':' statement
        | 'default' ':' statement
    

    compound_statement : '{' '}'
        | '{' block_item_list '}'
    

    block_item_list : block_item {block_item}
    

    block_item : declaration
        | statement
    

    expression_statement : ';'
        | expression ';'
    

    selection_statement : 'if' '(' expression ')' statement
        | 'if' '(' expression ')' statement 'else' statement
        | 'switch' '(' expression ')' statement
    

    iteration_statement : 'while' '(' expression ')' statement
        | 'do' statement 'while' '(' expression ')' ';'
        | 'for' '(' [expression] ';' [expression] ';' [expression] ')' statement
        | 'for' '(' declaration [expression] ';' [expression] ')' statement
    

    jump_statement : 'goto' IDENTIFIER ';'
        | 'continue' ';'
        | 'break' ';'
        | 'return' ';'
        | 'return' expression ';'
    

    translation_unit : external_declaration {external_declaration}
    

    external_declaration : function_definition
        | declaration
        | PREPROCESSOR
    

    function_definition : declaration_specifiers declarator compound_statement
        | declaration_specifiers declarator declaration_list compound_statement
    

    declaration_list : declaration {declaration}
    