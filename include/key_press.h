#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
using std::vector;
using std::string;

string get_command(vector<string> keys){
	string items = "xdotool key";
	for(auto i = keys.begin(); i != keys.end(); ++i){
		items = items + " " + *i;
	}
	return items;
}

string get_command(string key){
	return "xdotool key "+key;
}

void push_keys(vector<string> keys){
	system(get_command(keys).c_str());
}

void push_keys(string key){
	system(get_command(key).c_str());
}

