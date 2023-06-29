// Morse_Code.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//


#include<stdlib.h>
#include<string.h>
#include<iostream>
#include<fstream>
#include<string>

#define MAX 6
using namespace std;

typedef struct tree {
	char data;
	char morse[MAX];
	struct tree* lchild;
	struct tree* rchild;
	tree()
	{
		lchild = NULL;
		rchild = NULL;
	}
}Bitree;

Bitree* Create_Tree(int n);
void Enter(Bitree* t, string morse, char data);
Bitree* Init_Tree(Bitree* t);
void Porder(Bitree* t);
std::string Morse_Code_Korean_To_Symbol(std::string korean_morse = "");
std::string Morse_Code_Decoder(std::string morse);
std::string Morse_Code_Decoder(Bitree* t, std::string morse);
std::string Morse_Code_Eecoder(Bitree* t, std::string encry);

int main()
{
    std::string ifile_name = "ref.txt";
    std::string ofile_name = "morse.txt";
    std::ifstream infile(ifile_name.c_str(), std::ios::in);
    std::ofstream outfile(ofile_name.c_str(), std::ios::out);
    if (!infile || !outfile)
    {
        std::cerr << "infile outfile" << std::endl;
        exit(-1);
    }

	Bitree* t;
	t = Create_Tree(MAX);
	t = Init_Tree(t);
	
    std::string text_line;
    std::string morse;
	std::string encry;
    while (getline(infile, text_line, '\n'))
    {
		morse = Morse_Code_Korean_To_Symbol(text_line);
		std::cout << morse << std::endl;
		outfile << morse << std::endl;
		//1.BF 2.tree limited depth
		//encry = Morse_Code_Decoder(morse);
		encry = Morse_Code_Decoder(t, morse);
		std::cout << encry << std::endl;
        outfile << encry << std::endl;
    }
    std::cout << "Hello World!\n";
    return 0;
}



std::string Morse_Code_Korean_To_Symbol(std::string korean_morse)
{
	std::string morse = "";
	for (int i = 0; i < korean_morse.length();)
	{
		unsigned char c = korean_morse[i];
		if (c > 0 && c < 128)//第一个字节0xxxxxxx表示1个字节: 0xxxxxxx
		{
			std::cout << "utf8-1!\n";
			i += 1;
		}
		else if (c > 191 && c < 224) //第一个字节110xxxxx表示2个字节: 110xxxxx 10xxxxxx
		{
			std::cout << "utf8-2!\n";
			i += 2;
		}
		else if (c > 223 && c < 240)//第一个字节为1110xxxx表示3个字节: 1110xxxx 10xxxxxx 10xxxxxx
		{
			std::cout << "utf8-3!\n";
			unsigned char c1 = korean_morse[i + 1];
			unsigned char c2 = korean_morse[i + 2];
			if (c1 > 127 && c1 < 192 && c2 > 127 && c2 < 192) //sure to be 3-UTF8
			{
				if (c == 235 && c1 == 139 && c2 == 168) //단(tan)
					morse += ".";
				else if (c == 236 && c1 == 182 && c2 == 148) //추(chu)
					morse += "_";
				else//other,do sub
					std::cout << "warning isn't ._ , Do sub!\n";
			}
			else
			{
				std::cerr << "utf8-3" << std::endl;
			}
			i += 3;
		}
		else//utf8其他长度字符
		{

		}
	}
	return morse;
}



//BF,没有完成
std::string Morse_Code_Decoder(std::string morse)
{
    int i = 0;
    bool b12345 = true;
    std::string encry;
    std::string decry;
    decry += Morse_Code_Decoder(morse);
}



//tree
Bitree* Create_Tree(int n)
{
	if (n)
	{
		Bitree* t;
		t = (Bitree*)malloc(sizeof(Bitree));
		t->data = '*';
		t->morse[0] = '\0';
		t->lchild = Create_Tree(n - 1);
		t->rchild = Create_Tree(n - 1);
		return t;
	}
	return NULL;
}

void Enter(Bitree* t, string morse, char s)
{
	Bitree* p;
	p = t;
	for (int i = 0; i < morse.length(); i++)
	{
		if (morse[i] == '_')
		{
			p = p->lchild;
		}
		else
		{
			p = p->rchild;
		}
	}
	p->data = s;
	strcpy_s(p->morse, MAX, morse.c_str());
}

Bitree* Init_Tree(Bitree* t)
{
	char a[10][MAX];
	strcpy_s(a[0], MAX, "_");
	strcpy_s(a[1], MAX, "._");
	strcpy_s(a[2], MAX, ".._");
	strcpy_s(a[3], MAX, "..._");
	strcpy_s(a[4], MAX, "...._");
	strcpy_s(a[5], MAX, ".....");
	strcpy_s(a[6], MAX, "_....");
	strcpy_s(a[7], MAX, "_...");
	strcpy_s(a[8], MAX, "_..");
	strcpy_s(a[9], MAX, "_.");

	Enter(t, a[0], '0');
	Enter(t, a[1], '1');
	Enter(t, a[2], '2');
	Enter(t, a[3], '3');
	Enter(t, a[4], '4');
	Enter(t, a[5], '5');
	Enter(t, a[6], '6');
	Enter(t, a[7], '7');
	Enter(t, a[8], '8');
	Enter(t, a[9], '9');
	return t;
}

void Porder(Bitree* t) 
{
	if (t)//递归先序
	{
		printf("%c  ", t->data);
		Porder(t->lchild);
		Porder(t->rchild);
	}
	return;
}

std::string Morse_Code_Decoder(Bitree* t, std::string morse)
{
	Bitree* p;
	std::string decry(1024,'\0');
	int i = 0; int j = 0;
	do {
		p = t;
		char data = '*';
		int k = 0;
		int depth = 0;
		std::string tempS(1024, '\0');
		while (morse[i + k] != '\0' && p->lchild != NULL && p->rchild != NULL ) {
			if (morse[i + k] == '_') {
					p = p->lchild;
			}
			else {
					p = p->rchild;
			}
			tempS[k] = morse[i + k];
			tempS[k + 1] = '\0';
			k++;
			if (p->data != '*')
			{
				data = p->data;
				depth = k;
				std::cout << tempS << "(" << data << ")" << endl;
			}
		}
		if (data != '*')
		{
			decry[j] = data;
			j++;
		}
		i += depth;
	} while (morse[i] != '\0' && morse[i - 1] != '\0');
	decry[j] = '\0';
	return decry;
}

std::string Morse_Code_Eecoder(Bitree* t, std::string encry)
{
	char a[38][MAX];
	strcpy_s(a[0], MAX, ".-");
	strcpy_s(a[1], MAX, "-...");
	strcpy_s(a[2], MAX, "-.-.");
	strcpy_s(a[3], MAX, "-..");
	strcpy_s(a[4], MAX, ".");
	strcpy_s(a[5], MAX, "..-.");
	strcpy_s(a[6], MAX, "--.");
	strcpy_s(a[7], MAX, "....");
	strcpy_s(a[8], MAX, "..");
	strcpy_s(a[9], MAX, ".---");
	strcpy_s(a[10], MAX, "-.-");
	strcpy_s(a[11], MAX, ".-..");
	strcpy_s(a[12], MAX, "--");
	strcpy_s(a[13], MAX, "-.");
	strcpy_s(a[14], MAX, "---");
	strcpy_s(a[15], MAX, ".--.");
	strcpy_s(a[16], MAX, "--.-");
	strcpy_s(a[17], MAX, ".-.");
	strcpy_s(a[18], MAX, "...");
	strcpy_s(a[19], MAX, "-");
	strcpy_s(a[20], MAX, "..-");
	strcpy_s(a[21], MAX, "...-");
	strcpy_s(a[22], MAX, ".--");
	strcpy_s(a[23], MAX, "-..-");
	strcpy_s(a[24], MAX, "-.--");
	strcpy_s(a[25], MAX, "--..");
	strcpy_s(a[26], MAX, "-----");
	strcpy_s(a[27], MAX, ".----");
	strcpy_s(a[28], MAX, "..---");
	strcpy_s(a[29], MAX, "...--");
	strcpy_s(a[30], MAX, "....-");
	strcpy_s(a[31], MAX, ".....");
	strcpy_s(a[32], MAX, "-....");
	strcpy_s(a[33], MAX, "--...");
	strcpy_s(a[34], MAX, "---..");
	strcpy_s(a[35], MAX, "----.");
	strcpy_s(a[36], MAX, "-....-");
	strcpy_s(a[37], MAX, ".-.-.-");
	std::string mores;
	int i = 0;
	do {
		if (encry[i] >= 97 && encry[i] <= 122) {
			encry[i] -= 32;
		}
		if (encry[i] >= 48 && encry[i] <= 57) {
			cout << a[encry[i] - 22] << "  ";
			mores += a[encry[i] - 22];
			mores += " ";
		}
		else if (encry[i] >= 65 && encry[i] <= 90) {
			cout << a[encry[i] - 65] << "  ";
			mores += a[encry[i] - 65];
			mores += " ";
		}
		else if (encry[i] == '-') {
			cout << a[36] << "  ";
			mores += a[36];
			mores += " ";
		}
		else {
			cout << a[37] << "  ";
			mores += a[37];
			mores += " ";
		}
		i++;
	} while (encry[i] != '\0' && encry[i - 1] != '\0');
	cout << endl;
	return mores;
}



// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
