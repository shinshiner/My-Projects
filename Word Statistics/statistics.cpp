//本程序利用vector及c++11新特性实现了英文文章词频统计及c++程序关键字统计
//made by shin
//——2017.1.2
//——v6.0

#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <io.h>

using namespace std;

const int MAXLEN = 100000;
const int MAXFILENAME = 100;
class openFileFail{};		//文件打开失败
class wrongMode{};			//模式选择不合法

#define route string(filename).append("\\").append(fileInfo.name)	//批处理路径
#define findroute_txt string(filename).append("\\").append("*.txt").c_str()	//txt文件路径
#define findroute_cpp string(filename).append("\\").append("*.cpp").c_str()	//cpp文件路径

//判断是否为字母
bool isA(char a) { return ((a >= 'a'&&a <= 'z') || (a >= 'A'&&a <= 'Z')); }

//判断是否为数字
bool isNum(char a) { return (a >= '0'&&a <= '9'); }

//储存单词及词频
struct words{
	string data;
	int num;
	words() { num = 0; }
	words(string str, int n = 1) { data = str; num = n; }
	~words() {}
};

//判断单词相同
bool equal(string w1, string w2, int sense){
	if (sense == 1) return w1 == w2;	//大小写敏感
	if (sense == 0){					//大小写不敏感
		if (w1.size() != w2.size()) return false;
		for (int i = 0; i<w1.size(); ++i)
			if (w1[i] != w2[i] && w1[i] != (w2[i] - 'a' + 'A') && w1[i] != (w2[i] - 'A' + 'a'))
				return false;
		return true;
	}
}

//词频统计函数
void count(vector<words> &lib, ifstream &in, int sense, string text){
	string word;
	int i, j, k;
	bool flag{ false };  //初始化列表

	auto n = text.size();  //auto

	for (i = 0; i < n;){
		while ((isA(text[i]))||(i>0&&i+1<n&&text[i]=='-'&&isA(text[i-1])&&isA(text[i+1]))||
			(i>0 && i + 1<n&&text[i] == '-'&&isA(text[i - 1]) && isA(text[i + 1])))
				word.push_back(text[i++]);

		if (word.size() != 0){ //word为单词
			flag = false;
			word.push_back('\0');
			for (k = 0; k < lib.size(); ++k)	//判断单词是否已出现过
				if (equal(lib[k].data, word, sense))
				{
					++lib[k].num;
					flag = true;
					break;
				}
			if (!flag)		//未出现过则加入lib
				lib.emplace_back(word);  //emplace
			word.clear();
		}
		else ++i;
		continue;
	}
}

//输出函数
void show(vector<words> &lib, int choose, ofstream &out){
	//使用Lambda函数
	switch (choose)
	{
	case 0:  //频率从大到小
		out << "频率从大到小输出的结果：\n";
		sort(lib.begin(), lib.end(), [](const words &w1, const words &w2)
		{
			return w1.num > w2.num;
		});
		break;
	case 1:  //频率从小到大
		out << "频率从小到大输出的结果：\n";
		sort(lib.begin(), lib.end(), [](const words &w1, const words &w2)
		{
			return w1.num < w2.num;
		});
		break;
	case 2:  //字典序从小到大
		out << "字典序从小到大输出的结果：\n";
		sort(lib.begin(), lib.end(), [](const words &w1, const words &w2)
		{
			return w1.data < w2.data;
		});
		break;
	case 3:  //字典序从大到小
		out << "字典序从大到小输出的结果：\n";
		sort(lib.begin(), lib.end(), [](const words &w1, const words &w2)
		{
			return w1.data > w2.data;
		});
		break;
	default:
		break;
	}
	
	//输出lib
	for (auto p : lib)
		out << '<' << p.num << " ," << p.data << '>' << '\n';
	out << '\n';
}

//读文件建立c++关键词库
void makeLib(vector<words> &keyWords, ifstream &in){
	if (keyWords.size() == 0) {		//第一次构建
		string tmp;
		while (in >> tmp)
			keyWords.push_back(words(tmp, 0));
	}
	else    //第n(n>1)次构建
		for (int i = 0; i < keyWords.size(); ++i)
			keyWords[i].num = 0;
}

//统计c++关键词
void countKeyWords(vector<words> &lib, vector<words> &keyWords, ifstream &in, string text){
	string word;
	int i, j, k;
	auto flag = false;		//auto
	auto n = text.size();	//auto

	for (i = 0; i < n;){
		//过滤注释
		if (text[i] == '/'&&i + 1 < n&&text[i + 1] == '/'){	//“//”型
			i += 2;
			while (i < n&&text[i] != '\n') ++i;
			continue;
		}
		if (text[i] == '/'&&i + 1 < n&&text[i + 1] == '*'){	//“/*”型
			i += 2;
			while (i + 1 < n&&(!(text[i] == '*'&&text[i + 1] == '/'))) ++i;
			continue;
		}

		//过滤引号
		if (text[i] == '"'){	//双引号
			++i;
			while (text[i] != '"')
				if (text[i] == '\\') i += 2;
				else ++i;
			++i;
			continue;
		}
		if (text[i] == '\''){	//单引号
			++i;
			while (text[i] != '\'')
				if (text[i] == '\\') i += 2;
				else ++i;
			++i;
			continue;
		}

		while (isA(text[i])||text[i]=='_'||isNum(text[i]))	//读入一个单词
			word.push_back(text[i++]);

		if (word.size() != 0){ //word为单词
			flag = false;
			for (k = 0; k < keyWords.size(); ++k)		//匹配关键字
				if (equal(keyWords[k].data, word, 1)){
					++keyWords[k].num;
					break;
				}
			word.clear();
		}
		else ++i;
		continue;
	}

	//若关键词出现次数不为0，则加入lib
	for (auto p : keyWords)
		if (p.num != 0) lib.emplace_back(p);  //emplace
}

int main()
{
	vector<words> lib, keyWords;
	string text, textkey, fileName;
	ifstream in, in_words("keyWordsLib.in");
	ofstream out("result.txt");
	char ch, filename[MAXFILENAME];
	int choose, sense, mode;

	//输入文件路径
	cout << "请输入文件路径：";
	cin >> filename;
	_finddata_t fileInfo;
	long handle = _findfirst(filename, &fileInfo);

	while (handle == -1){	//错误处理
		cout << "文件路径错误！请重新输入：\n";
		cin >> filename;
		handle = _findfirst(filename, &fileInfo);
	}

	cout << "请输入统计词频或统计关键词（0为词频，1为关键词, -1退出）：";
	cin >> mode;
	while (mode != 0 && mode != 1 && mode != -1){	//错误处理
		cout << "模式输入错误，请重新输入！";
		cin >> mode;
	}

	//功能主循环
	while (mode != -1) {
		if (mode == 0) {
			cout << "  请选择大小写是否敏感（敏感为1，不敏感为0,-1返回上一级）：";
			cin >> sense;
			while (!(sense >= -1 && sense <= 1)){	//敏感选择输入不合法
				cout << "模式输入错误，请重新输入！";
				cin >> sense;
			}

			while (sense != -1) {
				cout << "    请选择功能：\n";
				cout << "    0 - 词频由大到小输出\n    1 - 词频由小到大输出\n    2 - 单词字典序从小到大输出\n"
					<< "    3 - 单词字典序从大到小输出\n"
					<< "    -1 - 回到上一级\n";
				cin >> choose;
				while (!(choose >= -1 && choose <= 3)){		//输出模式选择不合法
					cout << "模式输入错误，请重新输入！";
					cin >> choose;
				}

				//按用户指定方式输出的循环
				while (choose != -1){
					handle = _findfirst(findroute_txt, &fileInfo);
					do{
						in.open(route);
						while ((ch = in.get()) != EOF)		//读入文件内容
							text.push_back(ch);

						count(lib, in, sense, text);
						out << fileInfo.name << "的词频统计：\n";
						show(lib, choose, out);

						//统计完一个文件，清理相关存储区
						in.close();
						lib.clear();
						text.clear();
					} while (_findnext(handle, &fileInfo) == 0);
					
					cout << "Done!\n";
					cin >> choose;
					while (!(choose >= -1 && choose <= 3)){
						cout << "模式输入错误，请重新输入！";
						cin >> choose;
					}
				}
				lib.clear();

				//继续下一次循环
				cout << "  请选择大小写是否敏感（敏感为1，不敏感为0, -1返回上一级）：";
				cin >> sense;
				while (sense != -1 && sense != 0 && sense != 1){		//敏感选择输入不合法
					cout << "模式输入错误，请重新输入！";
					cin >> sense;
				}
			}
		}
		if (mode == 1){
			handle = _findfirst(findroute_cpp, &fileInfo);

			cout << "    请选择功能：\n";
			cout << "    0 - 词频由大到小输出\n    1 - 词频由小到大输出\n    2 - 单词字典序从小到大输出\n"
				<< "    3 - 单词字典序从大到小输出\n"
				<< "    -1 - 回到上一级\n";
			cin >> choose;
			while (!(choose >= -1 && choose <= 3)){		//输出模式选择不合法
				cout << "模式输入错误，请重新输入！";
				cin >> choose;
			}

			while (choose != -1) {
				//按用户指定方式输出的循环
				while (choose != -1){
					handle = _findfirst(findroute_cpp, &fileInfo);
					do{
						makeLib(keyWords, in_words);
						in.open(route);
						while ((ch = in.get()) != EOF)		//读入文件内容
							textkey.push_back(ch);

						countKeyWords(lib, keyWords, in, textkey);
						out << fileInfo.name << "的关键词统计：\n";
						show(lib, choose, out);

						//统计完一个文件，清理相关存储区
						in.close();
						lib.clear();
						textkey.clear();
					} while (_findnext(handle, &fileInfo) == 0);

					cout << "Done!\n";
					cin >> choose;
					while (!(choose >= -1 && choose <= 3)){
						cout << "模式输入错误，请重新输入！";
						cin >> choose;
					}
				}
			}
		}

		//继续下一次循环
		cout << "请输入统计词频或统计关键词（0为词频，1为关键词, -1退出）：";
		cin >> mode;
		while (mode != 0 && mode != 1 && mode != -1){	//模式选择不合法
			cout << "模式输入错误，请重新输入！";
			cin >> mode;
		}
	}
	cout << "谢谢使用！\n";

	in.close();
	out.close();

	return 0;
}
