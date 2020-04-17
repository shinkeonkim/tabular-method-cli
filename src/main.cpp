#include <iostream>
#include <algorithm>
#include <vector>
#include <string>

using namespace std;

int lang = 0;
unsigned int numOfVar;
unsigned int numOfMinterm;
unsigned int numOfDontcare;
vector <int> minterm;
vector <int> dontcare;

vector <vector <string>> msg = {
    {"Enter the number of variables.: ", "변수의 개수를 입력해주세요.: "},
    {"Enter the number of minterms.: ", "minterm의 개수를 입력해주세요.: "},
    {"Enter the minterms such as example.\n", "아래 예시처럼 minterm들을 입력해주세요."},
    {"Ex: 0 3 9 7\n", "예시: 0 3 9 7\n"},
    {"Enter: ", "입력: "},
    {"Enter the number of `don'tcares`.: ", "don't care의 개수를 입력해주세요.: "},
    {"Enter the number of `don't care`\n", "don't care에 해당하는 수들을 입력해주세요.\n"},
};

void inputLanguage(); // input language number
void printLine(); // print "-" * 50
void printLine(int k); // print "-" * k

bool inputNumbers(); // input numbers
bool mintermValidation();
bool dontcareValidation();

void tabularMethod();
vector<vector<int>> grouping();
bool compare(vector<int> V1, vector<int> V2);
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

    if(!mintermValidation()) return false;
    cout << msg[5][lang];
    cin >> numOfDontcare;
    if(numOfDontcare > 0) {
        cout << msg[6][lang];
        cout << msg[4][lang];

        for(int x=0; x<numOfDontcare; x++) {
            cin >> input;
            dontcare.push_back(input);
        }

        if(!dontcareValidation()) return false;
    }
    return true;
}

void tabularMethod() {
   vector<vector<int>> groupingResult = grouping();
   for(int x=0; x<groupingResult.size(); x++) {
       for(int y=0; y<groupingResult[x].size(); y++) {
           cout <<groupingResult[x][y] << " ";
       }
       cout<<"\n";
   }
}

bool compare(vector<int> V1, vector<int> V2) {
    unsigned int c1 = 0,cnt1=0;
    unsigned int c2 = 0,cnt2=0;
    for(int x=0; x<V1.size(); x++) {
        if(V1[x] == 1) cnt1++;
        c1*=2;
        c1 += (V1[x]%2);
    }
    for(int x=0; x<V2.size(); x++) {
        if(V2[x] == 1) cnt2++;
        c2*=2;
        c2 += (V2[x]%2);
    }
    if(cnt1 < cnt2) {
        return true;
    }
    else if(cnt1 == cnt2) {
        if(c1 < c2) return true;
    }
    return false;
}

vector<vector<int>> grouping() {
    printLine();
    vector<vector<int>> ret;
    for(int x=0; x<numOfMinterm; x++) {
        vector<int> I;
        for(int x=0; x<numOfVar; x++) I.push_back(0);
        ret.push_back(I);
    }

    for(int y=0; y<numOfMinterm; y++) {
        unsigned int current = minterm[y];
        int idx = numOfVar-1;
        while(current > 0 && idx >=0) {
            ret[y][idx] = current % 2;
            current/=2;
            idx--;
        }
    }

    sort(ret.begin(), ret.end(), compare);

    printLine();
    
    int count = 0;
    while(true) {


        printLine();    
        if(count==0) break;
    }

    return ret;
}

void makePIchart() {
    
}

void printPIchart() {

}

bool mintermValidation() {
    unsigned int checkNumber = 1 << numOfVar;
    for(auto i : minterm) {
        if(i >= checkNumber) return false;
    }
    for(int x=0; x<minterm.size(); x++) {
        for(int y=x+1; y<minterm.size(); y++) {
            if(minterm[x] == minterm[y]) return false;
        }
    }
    return true;
}
bool dontcareValidation() {
    unsigned int checkNumber = 1 << numOfVar;
    for(auto i : dontcare) {
        if(i >= checkNumber) return false;
    }
    
    for(int x=0; x<dontcare.size(); x++) {
        for(int y=x+1; y<dontcare.size(); y++) {
            if(dontcare[x] == dontcare[y]) return false;
        }
    }

    for(auto i : dontcare) {
        for(auto j : minterm) {
            if(i == j) return false;
        }
    }

    return true;
}

void testFunc() {
    cout << "numOfVar: " <<numOfVar << "\nnumOfMinterm: " <<numOfMinterm << "\nnumOfDontcare:  " << numOfDontcare <<"\nminterm: ";
    for(auto i : minterm) {
        cout << i << " ";
    } 
    cout << "\ndontcare: ";
    for(auto i : dontcare) {
        cout << i << " ";
    }
}

void inputErrorMsg() {
    string s[] = {"your input content is not valid.\n", "당신의 입력은 올바르지 않습니다.\n"};
    cout << s[lang];
}