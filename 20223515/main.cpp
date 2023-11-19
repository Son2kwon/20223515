#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
#include<vector>
#include"Parser.h"
#include"lexical_analyzer.h"
using namespace std;

extern vector<string> tokenStream;
int type;

int main(int argc, char* argv[]) {
	ifstream f;
	string file_name;
	if (argv[1] == "-v") {
		file_name = argv[2];
		type = typeB;
	}
	else {
		file_name = argv[1];
		type = typeA;
	}
		

	f.open(file_name);
	vector<Sentence> sentences;	// 문장들을 저장하는 sentences vector

	string line;

	if (f.is_open()) {
		while (getline(f, line)) {	// 1문장씩 읽어오면서 sentence에 저장
			size_t temp_index = line.find(';');

			if (temp_index != std::string::npos) {	// 만약 문장 중간에 ; 가 있으면 2개의 문장으로 분리 후 저장
				string line1 = line.substr(0, temp_index) + ';';	// ; 이전 문장
				string line2 = line.substr(temp_index + 1);	// ; 이후 문장
				Sentence sentence1(line1);

				sentences.push_back(sentence1);	// 우선 ; 이전 문장을 저장

				if (!line2.empty()) {	// 만약 ; 이후 문장이 비어있지 않다면 저장
					Sentence sentence2(line2);
					sentences.push_back(sentence2);
				}
			}

			else {									// 문장 중간에 ; 가 없으면 그냥 저장
				Sentence sentence(line);
				sentences.push_back(sentence);
			}
		}

		for (Sentence sentence : sentences) {
			for (string token : sentence.lexemes) {
				tokenStream.push_back(token);
			}
		}

		program();

		f.close();
	}

	else cout << "Fail to open file" << endl;
}