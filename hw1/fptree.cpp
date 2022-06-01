#include <bits/stdc++.h>
using namespace std;

int idCnt = 0;

struct node {
	map< int, node* > child;
	node *nxt, *parent;
	int val, id, cnt; // val 是他的值，cnt是他在growth過程中被加的權重

	node() {
		nxt = NULL;
		val = -1;
		cnt = 0;
		id = idCnt++;
		parent = NULL;
	}

	node(int _val) {
		nxt = NULL;
		val = _val;
		cnt = 0;
		id = idCnt++;
		parent = NULL;
	}	
};


map<int, node*> lastAppear, firstAppear; // 紀錄每個值最後一次在樹出現的位置

map<int, int> cnt, mineCnt;

int min_support ;

int srt(int a, int b){
	return cnt[a] > cnt[b];
}

void goUp(node *p,int leafCnt) {
	mineCnt[p->val] += leafCnt;
	// cout<<"goUp "<<p->val<<' '<<mineCnt[p->val]<<endl;
	if(p->parent) goUp(p->parent, leafCnt);
}

vector<int> mineV, dfsV;

map< vector<int>, int > setCnt;

void dfs(int depth, int x, int nowMin) {
	if(depth == int(mineV.size())){ // reach end
		// cout<<"\tdfs end"<<endl;
		// cout<<"\t";
		for(auto f:dfsV) cout<<f<<' ';
		cout<<endl;
		setCnt[dfsV] = setCnt[dfsV]+nowMin;
		return;
	}

	if(mineV[depth] != x) dfs(depth+1, x, nowMin);
	dfsV.push_back(mineV[depth]);
	dfs(depth+1, x, min(nowMin, mineCnt[mineV[depth]]));
	dfsV.pop_back();
}

void mining(int x) {
	// cout<<"mining "<<x<<endl;
	node* p;
	p = firstAppear[x];
	if(p == NULL) {
		cout<<"NULL first appear, error!"<<endl;
		return;
	}

	mineCnt.clear();
	while(1) {
		// cout<<p->id<<endl;
		goUp(p, p->cnt);
		if(p->nxt == NULL) break;
		p = p->nxt;
	}

	mineV.clear();
	dfsV.clear();

	for(auto p:mineCnt) {
		if(p.first!=-1 && p.second >= min_support) {
			mineV.push_back(p.first);
		}
	}

	sort(mineV.begin(), mineV.end(), srt);

	dfs(0, x, 1e9);
}
void prt(node *p,int depth) {
	cout<<(char)(p->val+'A'-1)<<' '<<depth<<' '<<p->id<<endl;
	if(p->nxt) {
		cout<<p->nxt->id<<endl;
	}
	for(auto c:p->child) {
		prt(c.second, depth+1);
	}
}


int main() {
	string s;
	stringstream ss;
	vector< vector<int> > vv;
	int transCnt = 0;

	while(cin>>s) {
		if(s=="XXX"){
			 break;
		}
		int now = 0;
		vector<int> v;
		transCnt ++;
		for(int i=0;i<s.length();i++) {
			char c = s[i];
			if(c != ',') {
				now = now * 10 + (c-'0');
			}
			if( c== ',' || i == s.length()-1) {
				v.push_back(now);
				cnt[now] ++;
				now = 0;
			}
 		}

		vv.push_back(v);
	}

	// while(getline(cin, s)) {
	// 	if(s=="XXX"){
	// 		break;
	// 	}
	// 	transCnt++;
	// 	ss.clear();
	// 	ss<<s;
	// 	int x;
	// 	vector<int> v;
	// 	while(ss>>x) {
	// 		v.push_back(x);
	// 		cnt[x] ++;
	// 	}
	// 	vv.push_back(v);
	// }

	for(auto v:vv){
		for(auto x:v){
			cout<<x<<' ';
		}
		cout<<endl;
	}
	int thres = (transCnt * 2+9) / 10; // 這樣要 >=
	// min_support = (transCnt+1) / 2 ; // 這樣要 >=
	min_support = 4;
	node *root = new node();
	node *p;

	for(int i=0;i<transCnt;i++) {

		vector<int> v = vv[i];
		sort(v.begin(), v.end(), srt);

		p = root;
		// cout<<"trans"<<endl;
		for(auto x:v) {
			if(cnt[x]>=thres){
				// cout<<"insert "<<x<<" to "<<p->val<<endl;
				// 會用到
				if(p->child.find(x) == p->child.end()){ // 目前節點的child中沒有X
					// cout<<"orphan"<<endl;
					p->child[x] = new node(x);
					p->child[x]->parent = p;
					p = p->child[x];
					if(lastAppear.find(x) == lastAppear.end()){ // x第一次出現
						// cout<<"first "<<p->id<<' '<<p->val<<endl;
						firstAppear[x] = p;
						lastAppear[x] = p;
					}
					else{
						// cout<<"nxt "<<lastAppear[x]->id<<" => "<<p->id<<' '<<p->val<<endl;
						lastAppear[x]->nxt = p;
						lastAppear[x] = p;
					}
				}
				else p = p->child[x];
				
				if(1) {
					cout<<"hello2 "<<p->val<<' '<<p->id<<endl;
				}
				else{
					cout<<"hello "<<p->val<<' '<<p->child[x]->parent->id<<' '<<p->id<<endl;
				}
				p->cnt ++;


			}
			else break;
		}
	}

	// prt(root, 0);


	set<int> mk; // 要mining, 要從出現次數最少的數字開始一組一組拔
	vector<int> cntSeq;

	for(auto f:cnt) {
		if(cnt[f.first]>=thres && mk.find(f.first) == mk.end()) 
			cntSeq.push_back(f.first);
	}
	// 此時cntSeq 應包含所有出現在樹中的節點	

	sort(cntSeq.begin(), cntSeq.end(), srt); // 出現次數大到小

	for(int i=(int)(cntSeq.size()) - 1; i>=0;i--) {
		mining( cntSeq[i] );
	}

	for(auto f:setCnt) {
		if(f.first.size() == 0) {
			continue;
		}
		for(int i=0;i<(int)f.first.size();i++) {
			if(i>0) cout<<", ";
			cout<<f.first[i];
		}

		printf(":%.04f\n", (double)setCnt[f.first]/(double)transCnt);
		// cout<<":"<<f.second<<endl;
	}



	return 0;
}