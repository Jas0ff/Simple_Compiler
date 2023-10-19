RE = []
GM = []
import re


def parse(exp, text):
    parsed = []
    matches = re.finditer(exp, text)

    splitter = set()
    for match in matches:
        splitter.add(match.start())
        splitter.add(match.end())

    splitter.add(0)
    splitter.add(len(text))
    splitter = sorted(splitter)

    for index in range(len(splitter) - 1):
        parsed.append(text[splitter[index] : splitter[index + 1]])

    return parsed


def test(text):
    text = re.sub(r"(\w)-(\w)", r"\1to\2", text)
    parsed = []
    parsed = parse(r"\[(.*?)\]", text)

    for temp in parsed:
        temp2 = parse(r"\wto\w", temp)

        temp2 = [item for item in temp2 if (item != "[" and item != "]")]

        parsed.insert(parsed.index(temp), temp2)
        parsed.remove(temp)

    for index in range(len(parsed)):
        if parsed[index][0] == "*":
            if type(parsed[index - 1]) == list:
                parsed[index - 1].insert(0, "all")

            else:
                temp = []
                temp.append("all")
                temp.append(parsed[index - 1])
                del parsed[index - 1]
                parsed.insert(index - 1, temp)
            del parsed[index]

    return parsed


def struct2payload():
    settings = {}

    if len(RE) > 0:
        settings["scanner"] = True
        settings["re_rule"] = [x[0] for x in RE]

    if len(GM) > 0:
        settings["parser"] = True
        settings["gm_rule"] = list(set(x[0] for x in GM))
        settings["gm_getin_rule"] = GM[0][0]

        null = []
        for rule in GM:
            if "null" in rule[1]:
                null.append(rule[0])

        settings["gm_nullable_rule"] = null

    # print(settings)

    gm_rule = {}
    for rule in GM:
        gm_rule[rule[0]] = []

    for rule in GM:
        if len(rule[1]) > 1:
            gm_rule[rule[0]].append(rule[1])
        else:
            gm_rule[rule[0]].append(rule[1][0])

    # print(gm_rule)

    re_rule = {}
    for rule in RE:
        re_rule[rule[0]] = []

    for rule in RE:
        grammars = rule[1].split("|")

        for grammar in grammars:
            re_rule[rule[0]].append(test(grammar))

    # print(re_rule)
    f = open("./payload.py", "w")

    f.write("settings = ")
    f.write(str(settings))
    f.write("\ngm_rule = ")
    f.write(str(gm_rule))
    f.write("\nre_rule = ")
    f.write(str(re_rule))
    f.close()


def txt2struct():
    raw = open("./rule.txt", "r")

    for line in raw:
        if line == "[RE]\n":
            for re_line in raw:
                if re_line == "\n":
                    break
                RE.append(re_line)

        if line == "[GM]\n":
            for gm_line in raw:
                if gm_line == "[GM]\n":
                    break
                GM.append(gm_line)


def transformstruct():
    for rule in RE:
        rule_id = rule.strip().split(" ")[0].strip()
        rule_cn = "".join(rule.strip().split(" ")[1:])

        temp = []
        temp.append(rule_id)
        temp.append(rule_cn)

        RE.insert(RE.index(rule), temp)
        RE.remove(rule)

    for rule in GM:
        rule_id = rule.strip().split("=")[0].strip()
        rule_cn = "".join(rule.strip().split("=")[1:]).strip()
        rule_cn = rule_cn.split(" ")

        for temp in rule_cn:
            if any(temp in sub_list[0] for sub_list in RE):
                rule_cn.insert(rule_cn.index(temp), "g" + temp)
                rule_cn.remove(temp)

        temp = []
        temp.append(rule_id)
        temp.append(rule_cn)

        GM.insert(GM.index(rule), temp)
        GM.remove(rule)


if __name__ == "__main__":
    txt2struct()
    transformstruct()
    struct2payload()
