#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
#include<vector>
#include"Parser.h"
#include"lexical_analyzer.h"
using namespace std;

extern vector<string> tokenStream;

int main(int argc, char* argv[]) {
	ifstream f;
	f.open(argv[1]);
	vector<Sentence> sentences;	// ������� �����ϴ� sentences vector

	string line;
	int ID = 0, CONST = 0, OP = 0;

	if (f.is_open()) {
		while (getline(f, line)) {	// 1���徿 �о���鼭 sentence�� ����
			size_t temp_index = line.find(';');

			if (temp_index != std::string::npos) {	// ���� ���� �߰��� ; �� ������ 2���� �������� �и� �� ����
				string line1 = line.substr(0, temp_index) + ';';	// ; ���� ����
				string line2 = line.substr(temp_index + 1);	// ; ���� ����
				Sentence sentence1(line1);

				sentences.push_back(sentence1);	// �켱 ; ���� ������ ����

				if (!line2.empty()) {	// ���� ; ���� ������ ������� �ʴٸ� ����
					Sentence sentence2(line2);
					sentences.push_back(sentence2);
				}
			}

			else {									// ���� �߰��� ; �� ������ �׳� ����
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