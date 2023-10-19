#include <algorithm>
#include <cstdio>
#include <iostream>
#include <utility>
#include <vector>
using namespace std;

typedef vector<pair<string, string>> token_info;
typedef vector<pair<string, string>> left_token;
typedef vector<pair<string, string>> token_input;

struct AST {
  string grammar;
  vector<AST *> childrens;
};

/*********************|
|       RE_RULE       |
|*********************/
        
string num(string, string);
string funcName(string, string);
string className(string, string);
string leftParen(string, string);
string rightParen(string, string);
string SPACE(string, string);
string EOL(string, string);
vector<string (*)(string, string)> rules ={num, funcName, className, leftParen, rightParen, SPACE, EOL};
string rule_name[] = {"num", "funcName", "className", "leftParen", "rightParen", "SPACE", "EOL"};


/**************************|
|       GRAMMAR_RULE       |
|**************************/
        
left_token Args(token_input);
left_token Class(token_input);
left_token Func(token_input);
left_token Proc(token_input);
left_token Stmt(token_input);
left_token Arg(token_input);
left_token null(token_input);
left_token gnum(token_input);
left_token gfuncName(token_input);
left_token gclassName(token_input);
left_token gleftParen(token_input);
left_token grightParen(token_input);
vector<left_token (*)(token_input)> nullable_rule = {null, Args, };

/*****************|
|       env       |
|*****************/
bool debug = false;

/**************************|
|       control_func       |
|**************************/
string getfullinput(string);
bool scanner(string, token_info &);
bool parser(token_input);
bool nullable(left_token (*)(token_input));

/***********************|
|       main_flow       |
|***********************/
int main() {
  string input = "";
  token_info tokens;
  AST grammar_tree;

  input = getfullinput(input);
  if (!scanner(input, tokens)) {
    cout << "invalid input" << endl; // invalid in scanner
    return 0;
  }

  if (debug) {
    for (auto token : tokens)
      cout << token.first << " ";
    cout << endl;
  }

  if (!parser(tokens)) {
    cout << "invalid input" << endl; // invalid in parser
    return 0;
  }

  for (auto token : tokens)
    cout << token.first << " " << token.second << endl;

  return 0;
}

bool scanner(string input, token_info &tokens) {
  while (input.size()) {
    string token = "";
    int rule_num = 0;

    for (auto rule : rules) {
      if (token.size() != 0)
        break;

      token = rule(input, "");
      if(debug){
        cout<<"token:"<<token<<endl;
        cout<<"input:"<<input<<endl;
      }
      rule_num++;
    }

    if (token.size() == 0)
      // input.substr(1, input.size()-1)
      return false;
    else {
      rule_num--;
      if (rule_name[rule_num] != "SPACE" && rule_name[rule_num] != "EOL")
        tokens.push_back({rule_name[rule_num], token});

      input = input.substr(token.size(), input.size() - token.size());
    }
  }
  return true;
}

string getfullinput(string input) {
  string line;
  while (getline(cin, line)) {
    input += line + "\n";
  }

  return input;
}

bool nullable(left_token (*rule)(token_input)) {
  for (auto grule : nullable_rule)
    if (rule == grule)
      return true;
  return false;
}

bool parser(token_info tokens) {
if (Proc(tokens).size() == 0)

        return true;

  return false;
}

left_token Proc(token_info tokens) {
  if(debug)
    cout << "Proc" << endl;

  token_info parsed_token = tokens;

  vector<left_token (*)(token_input)> grammar_0 = {Stmt};
  vector<vector<left_token (*)(token_input)>> grammars = {grammar_0, };


  for (auto grammar : grammars) {
    token_info temp_token = tokens;

    for (auto rule : grammar) {
      token_info temp_parsed_token;
      temp_parsed_token = rule(temp_token);

      if (temp_parsed_token.size() == temp_token.size() && !nullable(rule))
        goto next_round;

      temp_token = temp_parsed_token;
    }

    // grammar 正確
    parsed_token = temp_token;
    return parsed_token;

  next_round:;
  }

  return parsed_token;
}
        
left_token Stmt(token_info tokens) {
  if(debug)
    cout << "Stmt" << endl;

  token_info parsed_token = tokens;

  vector<left_token (*)(token_input)> grammar_0 = {Class};
  vector<left_token (*)(token_input)> grammar_1 = {Func};
  vector<vector<left_token (*)(token_input)>> grammars = {grammar_0, grammar_1, };


  for (auto grammar : grammars) {
    token_info temp_token = tokens;

    for (auto rule : grammar) {
      token_info temp_parsed_token;
      temp_parsed_token = rule(temp_token);

      if (temp_parsed_token.size() == temp_token.size() && !nullable(rule))
        goto next_round;

      temp_token = temp_parsed_token;
    }

    // grammar 正確
    parsed_token = temp_token;
    return parsed_token;

  next_round:;
  }

  return parsed_token;
}
        
left_token Class(token_info tokens) {
  if(debug)
    cout << "Class" << endl;

  token_info parsed_token = tokens;

  vector<left_token (*)(token_input)> grammar_0 = {gclassName, gleftParen, grightParen, Args, };
  vector<vector<left_token (*)(token_input)>> grammars = {grammar_0, };


  for (auto grammar : grammars) {
    token_info temp_token = tokens;

    for (auto rule : grammar) {
      token_info temp_parsed_token;
      temp_parsed_token = rule(temp_token);

      if (temp_parsed_token.size() == temp_token.size() && !nullable(rule))
        goto next_round;

      temp_token = temp_parsed_token;
    }

    // grammar 正確
    parsed_token = temp_token;
    return parsed_token;

  next_round:;
  }

  return parsed_token;
}
        
left_token Func(token_info tokens) {
  if(debug)
    cout << "Func" << endl;

  token_info parsed_token = tokens;

  vector<left_token (*)(token_input)> grammar_0 = {gleftParen, gfuncName, Args, grightParen, };
  vector<vector<left_token (*)(token_input)>> grammars = {grammar_0, };


  for (auto grammar : grammars) {
    token_info temp_token = tokens;

    for (auto rule : grammar) {
      token_info temp_parsed_token;
      temp_parsed_token = rule(temp_token);

      if (temp_parsed_token.size() == temp_token.size() && !nullable(rule))
        goto next_round;

      temp_token = temp_parsed_token;
    }

    // grammar 正確
    parsed_token = temp_token;
    return parsed_token;

  next_round:;
  }

  return parsed_token;
}
        
left_token Args(token_info tokens) {
  if(debug)
    cout << "Args" << endl;

  token_info parsed_token = tokens;

  vector<left_token (*)(token_input)> grammar_0 = {Arg, Args, };
  vector<left_token (*)(token_input)> grammar_1 = {null};
  vector<vector<left_token (*)(token_input)>> grammars = {grammar_0, grammar_1, };


  for (auto grammar : grammars) {
    token_info temp_token = tokens;

    for (auto rule : grammar) {
      token_info temp_parsed_token;
      temp_parsed_token = rule(temp_token);

      if (temp_parsed_token.size() == temp_token.size() && !nullable(rule))
        goto next_round;

      temp_token = temp_parsed_token;
    }

    // grammar 正確
    parsed_token = temp_token;
    return parsed_token;

  next_round:;
  }

  return parsed_token;
}
        
left_token Arg(token_info tokens) {
  if(debug)
    cout << "Arg" << endl;

  token_info parsed_token = tokens;

  vector<left_token (*)(token_input)> grammar_0 = {gnum};
  vector<left_token (*)(token_input)> grammar_1 = {Func};
  vector<vector<left_token (*)(token_input)>> grammars = {grammar_0, grammar_1, };


  for (auto grammar : grammars) {
    token_info temp_token = tokens;

    for (auto rule : grammar) {
      token_info temp_parsed_token;
      temp_parsed_token = rule(temp_token);

      if (temp_parsed_token.size() == temp_token.size() && !nullable(rule))
        goto next_round;

      temp_token = temp_parsed_token;
    }

    // grammar 正確
    parsed_token = temp_token;
    return parsed_token;

  next_round:;
  }

  return parsed_token;
}
        

left_token null(token_info tokens) {
  if (debug)
    cout << "null" << endl;
  return tokens;
}
    
left_token gnum(token_info tokens) {
  if (tokens.empty())
    return tokens;

  if (debug)
    cout << "gnum" << endl;

  if (tokens.begin()->first == "num")
    tokens.erase(tokens.begin());

  return tokens;
}
            
left_token gfuncName(token_info tokens) {
  if (tokens.empty())
    return tokens;

  if (debug)
    cout << "gfuncName" << endl;

  if (tokens.begin()->first == "funcName")
    tokens.erase(tokens.begin());

  return tokens;
}
            
left_token gclassName(token_info tokens) {
  if (tokens.empty())
    return tokens;

  if (debug)
    cout << "gclassName" << endl;

  if (tokens.begin()->first == "className")
    tokens.erase(tokens.begin());

  return tokens;
}
            
left_token gleftParen(token_info tokens) {
  if (tokens.empty())
    return tokens;

  if (debug)
    cout << "gleftParen" << endl;

  if (tokens.begin()->first == "leftParen")
    tokens.erase(tokens.begin());

  return tokens;
}
            
left_token grightParen(token_info tokens) {
  if (tokens.empty())
    return tokens;

  if (debug)
    cout << "grightParen" << endl;

  if (tokens.begin()->first == "rightParen")
    tokens.erase(tokens.begin());

  return tokens;
}
            
string num(string input, string temp_token) {
  if (input.size() == 0)
    return temp_token;

  if (temp_token.size() == 0 &&
      ((input[0] >= '0' && input [0] <= '9')))
    return temp_token + input[0];



  return temp_token;
}
        
string funcName(string input, string temp_token) {
  if (input.size() == 0)
    return temp_token;

  if (temp_token.size() == 0 &&
      ((input[0] >= 'a' && input [0] <= 'z')))
    return temp_token + input[0];



  return temp_token;
}
        
string className(string input, string temp_token) {
  if (input.size() == 0)
    return temp_token;

  if (temp_token.size() == 0 &&
      ((input[0] >= 'A' && input [0] <= 'Z')))
    return temp_token + input[0];



  return temp_token;
}
        
string leftParen(string input, string temp_token) {
  if (input.size() == 0)
    return temp_token;

  if (temp_token.size() == 0 &&
      (input[0] == '('))
    return temp_token + input[0];



  return temp_token;
}
        
string rightParen(string input, string temp_token) {
  if (input.size() == 0)
    return temp_token;

  if (temp_token.size() == 0 &&
      (input[0] == ')'))
    return temp_token + input[0];



  return temp_token;
}
        

string SPACE(string input, string temp_token) {
  if (input.size() == 0)
    return temp_token;

  if (input[0] == ' ' && temp_token == "")
    return temp_token + input[0];

  return temp_token;
}

string EOL(string input, string temp_token) {
  if (input.size() == 0)
    return temp_token;

  if (input[0] == '\n' && temp_token == "")
    return temp_token + input[0];

  return temp_token;
}
    

