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
string ID(string, string);
string STRLIT(string, string);
string LBR(string, string);
string RBR(string, string);
string DOT(string, string);
string SPACE(string, string);
string EOL(string, string);
vector<string (*)(string, string)> rules = {ID,  STRLIT, LBR, RBR,
                                            DOT, SPACE,  EOL};
string rule_name[] = {"ID", "STRLIT", "LBR", "RBR", "DOT", "SPACE", "EOL"};

/**************************|
|       GRAMMAR_RULE       |
|**************************/
left_token program(token_input);
left_token stmts(token_input);
left_token stmt(token_input);
left_token primary(token_input);
left_token primary_tail(token_input);
left_token null(token_input);
left_token gID(token_input);
left_token gSTRLIT(token_input);
left_token gLBR(token_input);
left_token gRBR(token_input);
left_token gDOT(token_input);

vector<left_token (*)(token_input)> nullable_rule = {
    null,
    stmts,
    stmt,
    primary_tail,
};

/*****************|
|       env       |
|*****************/
bool debug = true;

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

bool parser(token_info tokens) {
  if (program(tokens).size() == 0)
    return true;

  return false;
}

bool scanner(string input, token_info &tokens) {
  while (input.size()) {
    string token = "";
    int rule_num = 0;

    for (auto rule : rules) {
      if (token.size() != 0)
        break;

      token = rule(input, "");
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

left_token program(token_info tokens) {
  if (debug)
    cout << "program" << endl;

  token_info parsed_token = tokens;

  vector<left_token (*)(token_input)> grammar_stmts = {stmts};
  vector<vector<left_token (*)(token_input)>> grammars = {grammar_stmts};

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

left_token stmts(token_info tokens) {
  if (debug)
    cout << "stmts" << endl;
  token_info parsed_token = tokens;

  vector<left_token (*)(token_input)> grammar_stmt_stmts = {stmt, stmts};
  vector<left_token (*)(token_input)> grammar_null = {null};
  vector<vector<left_token (*)(token_input)>> grammars = {grammar_stmt_stmts,
                                                          grammar_null};

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

left_token stmt(token_info tokens) {
  if (debug)
    cout << "stmt" << endl;
  token_info parsed_token = tokens;

  vector<left_token (*)(token_input)> grammar_primary = {primary};
  vector<left_token (*)(token_input)> grammar_STRLIT = {gSTRLIT};
  vector<left_token (*)(token_input)> grammar_null = {null};
  vector<vector<left_token (*)(token_input)>> grammars = {
      grammar_primary, grammar_STRLIT, grammar_null};

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

left_token primary(token_info tokens) {
  if (debug)
    cout << "primary" << endl;
  token_info parsed_token = tokens;

  vector<left_token (*)(token_input)> grammar_ID_primarytail = {gID,
                                                                primary_tail};
  vector<vector<left_token (*)(token_input)>> grammars = {
      grammar_ID_primarytail};

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

left_token primary_tail(token_info tokens) {
  if (debug)
    cout << "primary_tail" << endl;

  token_info parsed_token = tokens;

  vector<left_token (*)(token_input)> grammar_DOT_ID_primarytail = {
      gDOT, gID, primary_tail};
  vector<left_token (*)(token_input)> grammar_LBR_stmt_RBR_primarytail = {
      gLBR, stmt, gRBR, primary_tail};
  vector<left_token (*)(token_input)> grammar_null = {null};

  vector<vector<left_token (*)(token_input)>> grammars = {
      grammar_DOT_ID_primarytail, grammar_LBR_stmt_RBR_primarytail,
      grammar_null};

  for (auto grammar : grammars) {
    token_info temp_token = tokens;

    for (auto rule : grammar) {
      token_info temp_parsed_token;
      temp_parsed_token = rule(temp_token);

      // 不是語法不對 就是 可以null
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

left_token gID(token_info tokens) {
  if (tokens.empty())
    return tokens;

  if (debug)
    cout << "gID" << endl;

  if (tokens.begin()->first == "ID")
    tokens.erase(tokens.begin());

  return tokens;
}

left_token gSTRLIT(token_info tokens) {
  if (tokens.empty())
    return tokens;

  if (debug)
    cout << "gSTRLIT" << endl;

  if (tokens.begin()->first == "STRLIT")
    tokens.erase(tokens.begin());

  return tokens;
}

left_token gLBR(token_info tokens) {
  if (tokens.empty())
    return tokens;

  if (debug)
    cout << "gLBR" << endl;

  if (tokens.begin()->first == "LBR")
    tokens.erase(tokens.begin());

  return tokens;
}

left_token gRBR(token_info tokens) {
  if (tokens.empty())
    return tokens;

  if (debug)
    cout << "gRBR" << endl;

  if (tokens.begin()->first == "RBR")
    tokens.erase(tokens.begin());

  return tokens;
}

left_token gDOT(token_info tokens) {
  if (tokens.empty())
    return tokens;

  if (debug)
    cout << "gDOT" << endl;

  if (tokens.begin()->first == "DOT")
    tokens.erase(tokens.begin());

  return tokens;
}

string ID(string input, string temp_token) {
  if (input.size() == 0)
    return temp_token;

  if (temp_token.size() == 0 &&
      ((input[0] >= 'A' && input[0] <= 'Z') ||
       (input[0] >= 'a' && input[0] <= 'z') || input[0] == '_'))
    return ID(input.substr(1, input.size() - 1), temp_token + input[0]);

  if (temp_token.size() != 0 &&
      ((input[0] >= 'A' && input[0] <= 'Z') ||
       (input[0] >= 'a' && input[0] <= 'z') ||
       (input[0] >= '0' && input[0] <= '9') || input[0] == '_'))
    return ID(input.substr(1, input.size() - 1), temp_token + input[0]);

  return temp_token;
}

string STRLIT(string input, string temp_token) {
  if (input.size() == 0)
    return "";

  if (input[0] == '"' && temp_token == "")
    return STRLIT(input.substr(1, input.size() - 1), temp_token + input[0]);

  if (input[0] != '"' && temp_token.size() != 0)
    return STRLIT(input.substr(1, input.size() - 1), temp_token + input[0]);

  if (input[0] == '"' && temp_token.size() != 0)
    return temp_token + input[0];

  return temp_token;
}

string LBR(string input, string temp_token) {
  if (input.size() == 0)
    return temp_token;

  if (input[0] == '(' && temp_token == "")
    return temp_token + input[0];

  return temp_token;
}

string RBR(string input, string temp_token) {
  if (input.size() == 0)
    return temp_token;

  if (input[0] == ')' && temp_token == "")
    return temp_token + input[0];

  return temp_token;
}

string DOT(string input, string temp_token) {
  if (input.size() == 0)
    return temp_token;

  if (input[0] == '.' && temp_token == "")
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
