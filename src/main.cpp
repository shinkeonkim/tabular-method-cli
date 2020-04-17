#include <iostream>
#include <algorithm>
#include <vector>
#include <string>

using namespace std;

struct PI {
    vector <int> minterms;
    vector <int> expression;
};


int lang = 0;
unsigned int numOfVar;
unsigned int numOfMinterm;
unsigned int numOfDontcare;
vector <int> minterm;
vector <int> dontcare;
vector<vector<PI>> termList;

vector <vector <string>> msg = {
    {"Enter the number of variables: ", "변수의 개수를 입력해주세요: "},
    {"Enter the number of minterms: ", "minterm의 개수를 입력해주세요: "},
    {"Enter the minterms such as example.\n", "아래 예시처럼 minterm들을 입력해주세요."},
    {"Ex: 0 3 9 7\n", "예시: 0 3 9 7\n"},
    {"Enter: ", "입력: "},
    {"Enter the number of `don'tcares`: ", "don't care의 개수를 입력해주세요: "},
    {"Enter the number of `don't care`\n", "don't care에 해당하는 수들을 입력해주세요.\n"},
};

void inputLanguage(); // input language number
void printLine(); // print "-" * 50
void printLine(int k); // print "-" * k

bool inputNumbers(); // input numbers
bool mintermValidation();
bool dontcareValidation();

void printTermList();
void setTermList();
void tabularMethod();
int termDiff(vector <int> V1, vector<int> V2);
vector<vector<int>> grouping();
int countOne(vector<int> V);
// bool compare(vector<int> V1, vector<int> V2);
void makePIchart();
void printPIchart();

void inputErrorMsg();
void testFunc();

int main() {
    // freopen("test.txt","r",stdin);
    // ios::sync_with_stdio(0);
    // cin.tie(0);
    // cout.tie(0);

    inputLanguage();
    if(!inputNumbers()) {
        inputErrorMsg();
        return 0;
    }

    setTermList();
    tabularMethod();

    testFunc();
}

void printLine() {
    printLine(50);
}
void printLine(int k) {
    for(int x=0; x<k; x++) cout << "-";
    cout<<"\n";
}

void inputLanguage() {
    printLine();
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
    printLine();
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

void setTermList() {
    termList.resize(1 << (numOfVar-1));
    for(int y=0; y<numOfMinterm; y++) {
        vector<int> tmp(numOfVar);
        int idx = numOfVar-1;
        unsigned int current = minterm[y];

        while(current > 0 && idx >=0) {
            tmp[idx] = current % 2;
            current/=2;
            idx--;
        }
        int cnt = countOne(tmp);
        termList[cnt].push_back({{minterm[y]},tmp});
    }
    for(int y=0; y<numOfDontcare; y++) {
        vector<int> tmp(numOfVar);
        int idx = numOfVar-1;
        unsigned int current = dontcare[y];

        while(current > 0 && idx >=0) {
            tmp[idx] = current % 2;
            current/=2;
            idx--;
        }
        int cnt = countOne(tmp);
        termList[cnt].push_back({{dontcare[y]},tmp});
    }
}


void tabularMethod() {
    grouping();
//    vector<vector<int>> groupingResult = grouping();
}

void printTermList() {
    for(int x=0; x<termList.size(); x++) {
        if(termList[x].size() == 0) {
            continue;
        }
        cout << x << "\n";
        printLine(20);
        for(int y=0; y<termList[x].size(); y++) {
            cout<<"(";
            for(int z1=0; z1<termList[x][y].minterms.size(); z1++) {
                cout <<termList[x][y].minterms[z1];
                if(z1 != termList[x][y].minterms.size() -1) cout << " ";
            }
            cout<<"): {";
            for(int z2=0; z2<termList[x][y].expression.size(); z2++) {
                cout <<termList[x][y].expression[z2];
                if(z2 != termList[x][y].expression.size()-1) cout << " ";
            }
            cout<<"} \n";
        }
        printLine(20);
    }
    cout<<"\n\n";
}

vector<vector<int>> grouping() {
    printLine();
    int groupingCount = 2;
    cout << "# grouping 1\n";
    printLine();
    printTermList();

    int grouping = 0;
    while(true) {
        vector<vector<PI>> currentTermList(numOfVar);
        vector<vector<bool>> check(termList.size());
        for(int y=0; y<termList.size(); y++) {
            for(int x=0; x<termList[y].size(); x++) check[y].push_back(true);
        }
        for(int y =0; y < termList.size(); y++) {
            if(termList[y].size() == 0) continue;
            if(y+1 < termList.size()) {
                for(int x1=0; x1<termList[y].size(); x1++) {
                    for(int x2=0; x2<termList[y+1].size(); x2++) {
                        int diff = termDiff(termList[y][x1].expression, termList[y+1][x2].expression);
                        // cout << y << " " << x1 << ": " << y+1 << " " << x2 << " " << diff <<"\n";
                        if(diff == 1) {
                            grouping++;
                            check[y][x1] = false;
                            check[y+1][x2] = false;
                            vector<int> e;
                            vector<int> t;
                            for(int z=0; z<termList[y][x1].expression.size(); z++) {
                                if(termList[y][x1].expression[z] != termList[y+1][x2].expression[z] && termList[y][x1].expression[z] + termList[y+1][x2].expression[z] ==1) {
                                    e.push_back(2);
                                }
                                else {
                                    e.push_back(termList[y][x1].expression[z]);
                                }
                            }

                            for(int z=0; z<termList[y][x1].minterms.size(); z++) {
                                t.push_back(termList[y][x1].minterms[z]);
                            }

                            for(int z2=0; z2 < termList[y+1][x2].minterms.size(); z2++) {
                                int check = true;
                                for(int z=0; z<termList[y][x1].minterms.size(); z++) {                                
                                    if(termList[y][x1].minterms[z] == termList[y+1][x2].minterms[z2]) {
                                        check= false;
                                    }
                                }
                                if(check) {
                                    t.push_back(termList[y+1][x2].minterms[z2]);
                                }
                            }

                            int oneCount = countOne(e);
                            currentTermList[oneCount].push_back({t,e});
                        }
                    }
                    if(check[y][x1]) {
                        currentTermList[y].push_back(termList[y][x1]);
                    }
                }
            }
            else {
                for(int x=0; x<termList[y].size(); x++) {
                    if(check[y][x]) {
                        int count = countOne(termList[y][x].expression);
                        currentTermList[count].push_back({termList[y][x].minterms,termList[y][x].expression});
                    }
                }
            }

        }

        termList = currentTermList;

        printLine();    
        cout << "# grouping "<<groupingCount <<"\n";    
        printLine();
        
        if(grouping == 0) {
            cout << "No more grouping\n";
            break;
        }
    
        printTermList();
        grouping = 0;
        groupingCount++;
    }


    
}


int termDiff(vector<int> V1, vector<int> V2) {
    int k = 0;
    for(int x=0; x<V1.size(); x++) {
        if(V1[x] != V2[x] && V1[x] + V2[x] == 1) k++;
    }
    return k;
}

int countOne(vector<int> V) {
    int ret = 0;
    for(auto i : V) {
        if(i == 1) ret++;
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
    cout<<"\n";
    printLine(30);
    cout<<"test Func Print\n";
    printLine(30);

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