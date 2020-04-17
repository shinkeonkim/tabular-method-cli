#include <iostream>
#include <algorithm>
#include <vector>
#include <string>

using namespace std;

int lang = 0;
int numOfVar;
int numOfMinterm;
int numOfDontcare;
vector <int> minterm;
vector <int> dontcare;

vector <vector <string>> msg = {
    {"Enter the number of variables.: ", "변수의 개수를 입력해주세요.: "},
    {"Enter the number of minterms.: ", "minterm의 개수를 입력해주세요.: "},
    {"Enter the minterms such as example.\n", "아래 예시처럼 minterm들을 입력해주세요."},
    {"Ex: 0 3 9 7\n", "예시: 0 3 9 7\n"},
    {"Enter: ", "입력: "},
    {"Enter the number of don't care", "don't care에 해당하는 수들을 입력해주세요."},
};

void inputLanguage(); // input language number
void printLine(); // print "-" * 50
void printLine(int k); // print "-" * k

bool inputNumbers(); // input numbers
bool mintermValidation();
bool dontcareValidation();

void tabularMethod();
void grouping();
void makePIchart();
void printPIchart();

void inputErrorMsg();
void testFunc();

int main() {
    // ios::sync_with_stdio(0);
    // cin.tie(0);
    // cout.tie(0);

    inputLanguage();
    if(!inputNumbers()) {
        inputErrorMsg();
        return 0;
    }

    tabularMethod();

    testFunc();
}

void printLine() {
    for(int x=0; x<50; x++) cout << "-";
    cout<<"\n";
}
void printLine(int k) {
    for(int x=0; x<k; x++) cout << "-";
    cout<<"\n";
}

void inputLanguage() {
    cout << "please input your language number\n";
    cout << "ENG: 0\n";
    cout << "KOR: 1\n";
    cout << "other numbers will be setting ENG\n";
    cout << "LANG: ";
    int tmp;
    cin >> tmp;
    if(tmp == 1) lang = 1;
    printLine();
}

bool inputNumbers() {
    cout << msg[0][lang];
    cin >> numOfVar;
    cout << msg[1][lang];
    cin >> numOfMinterm;
    cout << msg[2][lang];
    cout << msg[3][lang];
    cout << msg[4][lang];
    int input;
    for(int x = 0; x<numOfMinterm; x++) {
        cin >> input;
        minterm.push_back(input);
    }

    mintermValidation();
    
    cout << msg[5][lang];
    cout << msg[4][lang];
    for(int x=0; x<numOfDontcare; x++) {
        cin >> input;
        dontcare.push_back(input);
    }

    dontcareValidation();

}



void tabularMethod() {

}

void grouping() {

}

void makePIchart() {
    
}

void printPIchart() {

}

bool mintermValidation() {
    return true;
}
bool dontcareValidation() {
    return true;
}

void testFunc() {
    cout << numOfVar << " " <<numOfMinterm << "\n"; 
}

void inputErrorMsg() {
    string s[] = {"your input content is not valid.\n", "당신의 입력은 올바르지 않습니다.\n"};
    cout << s[lang];
}