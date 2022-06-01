#include <bits/stdc++.h>

using namespace std;

int global_idCnt = 0;
int min_support;

map<int,int> inputCnt;

int srt(int a, int b) {
	return inputCnt[a] > inputCnt[b];
}

struct node {
	map<int, node*> child;
	node *parent;
	int id, cnt, val;


	node() {
		id = ++global_idCnt;
		cnt = 0;
		val = -1;
		parent = NULL;
	}

	node(int _val) {
		id = ++global_idCnt;
		cnt = 0;
		val = _val;
		parent = NULL;
	}

};

node *root, *subRoot;
map< int, vector<node*> > appear;

map< vector<int>, int > setCnt;

vector<int> countSetVec;
vector<int> countSetPath;
void countSet( int cnt, int r ) {
	if( r == (int)(countSetPath.size()) ) {
		setCnt[countSetVec] += cnt;
		return ;
	}

	if(r!=(int)(countSetPath.size())-1) countSet( cnt, r+1 );
	countSetVec.push_back(countSetPath[r]);
	countSet( cnt, r+1 );
	countSetVec.pop_back();
}

void mining(int x) {
	map<int, int> subCnt;
	subRoot = new node();
	vector< node* > leafList = appear[x];

	for( int i=0; i<(int)(leafList.size()); i++ ) {
		node *leaf = leafList[i];
		node *p = leaf;

		while(p->val != -1) {
			subCnt[p->val] += leaf->cnt;
			p = p -> parent;
		}
	}

	for( int i=0; i<(int)(leafList.size()); i++ ) {
		node *leaf = leafList[i];
		node *p = leaf;

		vector<int> path;

		while(p->val != -1) {
			if(subCnt[p->val] >= min_support) {
				path.push_back(p->val);
			}
			p = p -> parent;
		}

		

		reverse(path.begin(), path.end()); // 效果應該等同sort

		countSetPath = path;
		countSet( leaf->cnt, 0 );

	}
}

int srt2( pair<vector<int>, double> a, pair<vector<int>, double> b ) {
	if(a.first.size() != b.first.size()) {
		return a.first.size() < b.first.size();
	}
	for(int i=0;i<(int)(a.first.size());i++) {
		if(a.first[i] != b.first[i]) return a.first[i] < b.first[i];
	}
	return 0;
}

int main(int argc, char *argv[]) {

	if (argc != 4) {
		cout << "./hw1 [min support] [input.txt] [output.txt]" << endl;
		exit(0);
	} 
	// else cout << argv[1] << " " << argv[2] << endl;

	float aa = atof(argv[1]);
	// cout << aa << endl;
	freopen(argv[2], "r", stdin);
	freopen(argv[3], "w", stdout);

	vector< vector<int> > allV;
	int transCnt;

	string s;
	while( cin>>s ){
		
		if(s=="XXX") break;

		int nowNumber = 0;
		vector<int> nowV;

		for(int i=0;i<(int)(s.length());i++) {
			if(s[i]>='0' && s[i]<='9') nowNumber = nowNumber * 10 + (s[i] - '0');
			if( s[i] == ',' || i == (int)(s.length()) - 1 ) {
				nowV.push_back(nowNumber);
				inputCnt[nowNumber] +=1;
				nowNumber = 0;
			}
		}

		allV.push_back(nowV);
	}

	transCnt = allV.size();

	// int thres = (transCnt * 2+9) / 10; // 這樣要 >=
	// min_support = (transCnt+1) / 2 ; // 這樣要 >=
	float a = 1e-7;
	min_support = (int)(aa * transCnt + a);
	// (transCnt * 2 + 9) / 10;

	root = new node();

	vector<int> miningSeq;

	for(int i=0;i<transCnt;i++) {
		vector<int> v = allV[i];
		sort(v.begin(), v.end(), srt);

		node *p = root;

		for(auto x:v) {
			if(inputCnt[x] >= min_support) {
				if(appear.find(x) == appear.end()) 
					miningSeq.push_back(x);

				if(p->child.find(x) == p->child.end()) {
					p -> child[x] = new node(x);
					p -> child[x] -> parent = p;
					appear[x].push_back(p->child[x]);
				}
				p = p->child[x];
				p->cnt++;
			}
			else break;
		}
	}

	sort(miningSeq.begin(), miningSeq.end(), srt);

	for( int i = (int)(miningSeq.size()) - 1; i >= 0; i-- ) 
		mining(miningSeq[i]);

	vector< pair< vector<int>, double > > results;

	for( auto f:setCnt ) {
		if(f.first.size() == 0 || setCnt[f.first] < min_support) {
			continue;
		}
		results.push_back( make_pair(f.first, (double)setCnt[f.first]/(double)transCnt) );

		// for( int i = 0; i < (int)f.first.size(); i++ ) {
		// 	if( i > 0 ) cout<<", ";
		// 	cout<<f.first[i];
		// }


		// printf(":%.04f\n", (double)setCnt[f.first]/(double)transCnt);
	}

	sort(results.begin(), results.end(), srt2);
	
	for( auto p:results ) {

		sort(p.first.begin(), p.first.end());
		for(int i=0;i<(int)p.first.size();i++) {
			if(i>0) cout<<",";
			cout<<p.first[i];
		}

		printf(":%.04f\n", p.second);
	}

	return 0;
}

/*
5,9,10
0,1,4,6,8,10
0,1,10
5
0,1,3,8,10
9
0,2,4,5,6,9
3
0,4,6,7,9,10
0,6,8,10
0,1,5,7,8,9
0,2,4,9
1,2,3,5,7,9,10
0,4,7,9
0,2,7
0,2,3,6,7,8,9
5,7,10
8
0,1,4,8,9,10
0,1,2,9,10
XXX
*/