import payload

setting = payload.settings


def main():
    code = assemblecode()
    print(code)


def assemblecode():
    code = "".join(open("./initial", "r").readlines())

    code = scanner_initial(code)
    if setting["parser"]:
        code = parser_initial(code)

    if setting["parser"]:
        code += "".join(open("./main", "r").readlines())
    else:
        code += "".join(open("./main_no_parser", "r").readlines())

    if setting["parser"]:
        code = parser_main(code)
        code = parser_rule(code)

    code = scanner_rule(code)

    return code


def scanner_initial(code):
    code += """
/*********************|
|       RE_RULE       |
|*********************/
        """

    code += "\n"
    for rule in setting["re_rule"]:
        code += "string " + rule + "(string, string);\n"

    code += "string SPACE(string, string);\n"
    code += "string EOL(string, string);\n"
    code += "vector<string (*)(string, string)> rules ={"
    for rule in setting["re_rule"]:
        code += rule + ", "

    code += "SPACE, EOL};\n"
    code += "string rule_name[] = {"
    for rule in setting["re_rule"]:
        code += '"' + rule + '"' + ", "
    code += '"SPACE", "EOL"};\n'
    code += "\n"

    return code


def parser_initial(code):
    code += """
/**************************|
|       GRAMMAR_RULE       |
|**************************/
        """

    code += "\n"
    for rule in setting["gm_rule"]:
        code += "left_token " + rule + "(token_input);\n"

    code += "left_token null(token_input);\n"

    for rule in setting["re_rule"]:
        code += "left_token g" + rule + "(token_input);\n"

    code += "vector<left_token (*)(token_input)> nullable_rule = {null, "
    for rule in setting["gm_nullable_rule"]:
        code += rule + ", "

    code += "};\n"
    code += "\n"

    return code


def parser_main(code):
    code += "\n"
    code += "bool parser(token_info tokens) {\n"
    code += "if (" + setting["gm_getin_rule"] + "(tokens).size() == 0)\n"
    code += """
        return true;

  return false;
}
"""
    code += "\n"

    return code


def parser_rule(code):
    rules = payload.gm_rule

    for key in rules.keys():
        code += "left_token " + key + "(token_info tokens) {\n"
        code += "  if(debug)\n"
        code += '    cout << "' + key + '" << endl;\n'
        code += "\n"
        code += "  token_info parsed_token = tokens;\n"
        code += "\n"

        grammar_num = 0
        for grammar in rules[key]:
            if type(grammar) == str:
                code += (
                    "  vector<left_token (*)(token_input)> grammar_"
                    + str(grammar_num)
                    + " = {"
                    + grammar
                    + "};\n"
                )
            else:
                code += (
                    "  vector<left_token (*)(token_input)> grammar_"
                    + str(grammar_num)
                    + " = {"
                )
                for rule in grammar:
                    code += rule + ", "

                code += "};\n"

            grammar_num += 1

        code += "  vector<vector<left_token (*)(token_input)>> grammars = {"
        for i in range(grammar_num):
            code += "grammar_" + str(i) + ", "
        code += "};\n"

        code += "\n"

        code += """
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
        """

        code += "\n"

    # ----end-of-grammars-node-----
    code += """
left_token null(token_info tokens) {
  if (debug)
    cout << "null" << endl;
  return tokens;
}
    """
    code += "\n"

    for rule in setting["re_rule"]:
        code += "left_token g" + rule + "(token_info tokens) {\n"
        code += (
            "  if (tokens.empty())\n"
            + "    return tokens;\n"
            + "\n"
            + "  if (debug)\n"
            + '    cout << "g'
            + rule
            + '" << endl;\n'
            + "\n"
            + '  if (tokens.begin()->first == "'
            + rule
            + '")'
            + """
    tokens.erase(tokens.begin());

  return tokens;
}
            """
        )

        code += "\n"
    return code


def scanner_rule(code):
    re_rules = payload.re_rule

    for re in re_rules.keys():
        code += (
            "string "
            + re
            + "(string input, string temp_token) {\n"
            + "  if (input.size() == 0)\n"
            + "    return temp_token;\n"
            + "\n"
        )

        # rule seperate by |
        for rule in re_rules[re]:
            for index, element in enumerate(rule):
                if index == 0:
                    code += "  if (temp_token.size() == 0 &&\n"

                else:
                    code += "  if (temp_token.size() != 0 &&\n"

                code += "      ("
                # element has mutiple choose
                for partical in list(element):
                    if partical == "all":
                        # * as a marker, not special use
                        continue

                    if "not" in partical:
                        partical = partical[3:]
                        if "to" in partical:
                            front = partical.split("to")[0]
                            end = partical.split("to")[1]

                            code += (
                                "("
                                + "input[0] <= '"
                                + front
                                + "' || input [0] >= '"
                                + end
                                + "')"
                            )
                        else:
                            code += "input[0] != '" + partical + "'"

                    elif "to" in partical:
                        front = partical.split("to")[0]
                        end = partical.split("to")[1]

                        code += (
                            "("
                            + "input[0] >= '"
                            + front
                            + "' && input [0] <= '"
                            + end
                            + "')"
                        )
                    else:
                        code += "input[0] == '" + partical + "'"

                    if partical not in element[-1]:
                        code += " || "
                code += "))\n"

                if len(rule) == 1:
                    code += "    return temp_token + input[0];\n"

                elif index == len(rule) - 1 and type(element) == str:
                    code += "    return temp_token + input[0];\n"

                else:
                    code += (
                        "    return "
                        + re
                        + "(input.substr(1, input.size() - 1), temp_token + input[0]);\n"
                    )

                code += "\n"

        code += "\n"
        code += """
  return temp_token;
}
        """
        code += "\n"

    code += """
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

  if (input[0] == '\\n' && temp_token == "")
    return temp_token + input[0];

  return temp_token;
}
    """

    code += "\n"

    return code


if __name__ == "__main__":
    main()
