#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include <string>

using namespace std;

struct rows {
    vector <int> minterms;
    vector <int> expression;
};


int lang = 0;
unsigned int numOfVar;
unsigned int numOfMinterm;
unsigned int numOfDontcare;
vector <int> minterm;
vector <int> dontcare;
vector<vector<rows>> termList;
vector<rows> totalrow;
vector<vector<int>> PIchart;
vector <rows> EPI;
vector <rows> PI;
vector <int> checkPI;
map <int, int> M;
int ansCount = (int)1e8;
int ansSizeSum = -1;
vector <int> ansCheck;

vector <vector <string>> msg = {
    {"Enter the number of variables: ", "변수의 개수를 입력해주세요: "},
    {"Enter the number of minterms: ", "minterm의 개수를 입력해주세요: "},
    {"Enter the minterms such as example.\n", "아래 예시처럼 minterm들을 입력해주세요."},
    {"Ex: 0 3 9 7\n", "예시: 0 3 9 7\n"},
    {"Enter: ", "입력: "},
    {"Enter the number of `don'tcares`: ", "don't care의 개수를 입력해주세요: "},
    {"Enter the number of `don't care`\n", "don't care에 해당하는 수들을 입력해주세요.\n"},
    {"NO EPI\n","EPI가 없습니다.\n"},
    {"NO PI\n", "PI가 없습니다.\n"},
};

void inputLanguage(); // input language number
void printLine(); // print "-" * 50
void printLine(int k); // print "-" * k

bool inputNumbers(); // input numbers
bool mintermValidation();
bool dontcareValidation();

void printTermList();
void deleteDuplicatedTermList();
void setTermList();
void tabularMethod();
int termDiff(vector <int> V1, vector<int> V2);
void grouping();
int countOne(vector<int> V);
// bool compare(vector<int> V1, vector<int> V2);
void makePIchart();
void printPIchart();
void findEPInPI();
void printEPInPI();
void findlogic();
void PIdfs(int idx);
void derivecheckedPI();

void printMinterm(vector <int> V);
void printExpression(vector <int> V);
void printfinalExpression(vector <int> V);
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
        M[input] = x;
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
            M[input] = -1;
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
    makePIchart();
    printPIchart();
    findEPInPI();
    printEPInPI();
    findlogic();
}

void grouping() {
    printLine();
    int groupingCount = 2;
    cout << "# grouping 1\n";
    printLine();
    printTermList();

    int grouping = 0;
    while(true) {
        vector<vector<rows>> currentTermList(numOfVar);
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
                        if(diff == 1) {
                            grouping++;
                            check[y][x1] = false;
                            check[y+1][x2] = false;
                            vector<int> e;
                            vector<int> t;
                            for(int z=0; z<termList[y][x1].expression.size(); z++) {
                                if(termList[y][x1].expression[z] != termList[y+1][x2].expression[z] && termList[y][x1].expression[z] + termList[y+1][x2].expression[z] == 1) {
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
                        check[y][x1] = false;
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

        deleteDuplicatedTermList();

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

void makePIchart() {
    for(int y =0; y< termList.size(); y++) {
        for(int x=0; x <termList[y].size(); x++) {
            totalrow.push_back(termList[y][x]);
        }
    }
    PIchart.resize(totalrow.size());

    for(int y=0; y<totalrow.size(); y++) {
        for(int x=0; x<numOfMinterm; x++) {
            int currentTerm = minterm[x];
            bool check = false;
            for(auto i : totalrow[y].minterms) {
                if(i == currentTerm) {
                    check = true;
                }
            }
            if(check) {
                PIchart[y].push_back(1);
            }
            else {
                PIchart[y].push_back(0);
            }
        }
    }
}

void printPIchart() {
    cout<<"\n";
    printLine();
    cout << "PIchart\n";
    cout<<"\n";
    cout << "\t";
    for(int x = 0; x<minterm.size(); x++) cout <<minterm[x]<<"\t";
    cout<<"\n";
    printLine(120);
    
    for(int y=0; y<PIchart.size(); y++) {
        cout << "{";
        for(int x=0; x<totalrow[y].minterms.size(); x++) {
            cout << totalrow[y].minterms[x];
            if(x != totalrow[y].minterms.size()-1) cout << " ";
        }
        cout << "}";
        cout << "\t";

        for(int x=0; x<PIchart[y].size(); x++ ) {
            if(PIchart[y][x] == 1) {
                cout << "X\t";
            }
            else {
                cout <<" \t";
            }
        }
        cout<<"\n";
    }
}


void findEPInPI() {
    checkPI.resize(PIchart.size());
    vector <bool> check(PIchart.size());
    for(int x = 0; x < numOfMinterm; x++) {
        int count = 0;
        int pos = 0;
        for(int y=0; y<PIchart.size(); y++) {
            if(PIchart[y][x] == 1) {
                count++;
                pos = y;
            }
        }
        if(count == 1) {
            check[pos] = 1;
        }
    }
    for(int x=0; x<PIchart.size(); x++) {
        if(check[x] == 1) {
            EPI.push_back(totalrow[x]);
            checkPI[x] = 2;
        }
        else {
            PI.push_back(totalrow[x]);
        }
    }

}

void printEPInPI() {
    printLine();
    cout << "EPI\n" ;
    printLine();
    if(EPI.size() == 0) {
        cout << msg[7][lang];
    }
    else {
        for(int x=0; x<EPI.size(); x++) {
            printMinterm(EPI[x].minterms);
            cout << " ";
            printExpression(EPI[x].expression);
            cout<<"\n";
        }
    }
    printLine();
    cout<<"\n";
    printLine();
    cout << "PI\n" ;
    printLine();
    if(PI.size() == 0) {
        cout << msg[8][lang];
    }
    else {
        for(int x=0; x<PI.size(); x++) {
            printMinterm(PI[x].minterms);
            cout << " ";
            printExpression(PI[x].expression);
            cout<<"\n";
        }
    }
    printLine();
}

void findlogic() {
    PIdfs(0);
    int last = 0;
    for(int x =0; x<ansCheck.size(); x++) {
        if(ansCheck[x] != 0) {
            last = x;
        }
    }
    cout << "final logic: ";
    for(int x =0; x<ansCheck.size(); x++) {
        if(ansCheck[x] != 0) {
            if(last != x) {
                printfinalExpression(totalrow[x].expression);
                cout <<"+";
            }
            else {
                printfinalExpression(totalrow[x].expression);
            }
        }
    }
    
}

void derivecheckedPI() {
    vector <bool> mintermCheck(numOfMinterm);
    int count = 0;
    int sizeSum = 0;
    for(int y=0; y< checkPI.size(); y++) {
        if(checkPI[y] != 0) {
            count++;
            sizeSum += totalrow[y].minterms.size();
            for(int x = 0; x< totalrow[y].minterms.size(); x++) {
                if(M[totalrow[y].minterms[x]] != -1)
                    mintermCheck[M[totalrow[y].minterms[x]]] = true;
            }
        }
    }

    for(int x=0; x<numOfMinterm; x++) {
        if(!mintermCheck[x]) return;
    }
    
    if(ansCount > count) {
        if(ansSizeSum <= sizeSum) {
            ansCount = count;
            ansSizeSum = sizeSum;
            ansCheck.clear();
            for(auto i : checkPI) {
                ansCheck.push_back(i);
            }
        }
    }
}

void PIdfs(int idx) {
    if(idx == PIchart.size()) {
        if(checkPI[idx] == 2) {
            derivecheckedPI();
        }
        else {
            for(int x=0; x<2; x++) {
                checkPI[idx] = x;
                derivecheckedPI();
            }
        }
    }
    else {
        if(checkPI[idx] == 2) {
            PIdfs(idx+1);
        }
        else {
            for(int x=0; x<2; x++) {
                checkPI[idx] = x;
                PIdfs(idx+1);
            }
        }

    }
}

void printTermList() {
    for(int x=0; x<termList.size(); x++) {
        if(termList[x].size() == 0) {
            continue;
        }
        cout << x << "\n";
        printLine(20);
        for(int y=0; y<termList[x].size(); y++) {
            printMinterm(termList[x][y].minterms);
            cout << " ";
            printExpression(termList[x][y].expression);
            cout<< "\n";
        }
        printLine(20);
    }
    cout<<"\n\n";
}

int vectorDiff(vector <int> V1, vector<int> V2) {
    if(V1.size() != V2.size()) return -1;
    int ret = 0;
    for(int x=0; x<V1.size(); x++) {
        if(V1[x] != V2[x]) {
            ret++;
        }
    }
    return ret;
}

void deleteDuplicatedTermList() {
    for(int y =0; y<termList.size(); y++) {
        for(int x=0; x< termList[y].size(); x++) {
            vector<int> Z = termList[y][x].minterms;
            sort(Z.begin(), Z.end());
            vector<int> Z2;
            Z2.push_back(Z[0]);
            for(int x=1; x<Z.size(); x++) {
                if(Z[x-1] != Z[x]) Z2.push_back(Z[x]);
            }
            termList[y][x].minterms = Z2;
        }
    }

    vector<vector<rows>> ret;
    for(int y=0; y<termList.size(); y++) {
        vector<rows> partialRet;
        for(int x1=0; x1<termList[y].size(); x1++) {
            int sameCount = 0;
            for(int x2=x1+1; x2<termList[y].size(); x2++) {
                if(vectorDiff(termList[y][x1].minterms,termList[y][x2].minterms) == 0) {
                    sameCount++;
                }
            }
            if(sameCount ==0) {
                partialRet.push_back(termList[y][x1]);
            }
        } 
        ret.push_back(partialRet);
    }
    termList = ret;
}


int termDiff(vector<int> V1, vector<int> V2) {
    if(V1.size() != V2.size()) return 1000;
    int k = 0;
    for(int x=0; x<V1.size(); x++) {
        if(V1[x] != V2[x]) k++;
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

void printMinterm(vector <int> V) {
    cout << "(";
    for(int x=0; x<V.size(); x++) {
        cout << V[x];
        if(x!= V.size() -1) cout << " ";
    }
    cout << ")";
}
void printExpression(vector <int> V) {
    for(int x=0; x<V.size(); x++) {
        if(V[x] == 0) {
            cout << (char)(x+97) << "'";
        }
        else if(V[x] == 1) {
            cout << (char)(x+97);
        }
        else {
            cout<<"-";
        }
    }
}
void printfinalExpression(vector <int> V) {
    for(int x=0; x<V.size(); x++) {
        if(V[x] == 0) {
            cout << (char)(x+97) << "'";
        }
        else if(V[x] == 1) {
            cout << (char)(x+97);
        }
    }
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