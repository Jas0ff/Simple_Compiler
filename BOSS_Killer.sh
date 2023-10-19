#!/bin/zsh

figlet BOSS KILLER | lolcat

while IFS= read -r line; do
    input_data+="$line"$'\n'
done

echo "$input_data"

echo "-----------------------------"
echo "Generating payload..."
python ./rule_operate.py

echo "Generating C++ code..."
python ./generater.py >out.cpp

echo "G++ Compiling..."
g++ -o out out.cpp

echo "DONE!" | figlet | lolcat
echo "Meow" | cowsay -f turtle | lolcat

