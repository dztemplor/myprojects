#include <iostream>
#include <vector>
#include <list>
#include <stack>
#include <algorithm>
#include <unordered_map>
#include <string.h>

using namespace std;
void print_vec(vector<int> &v)
{
  for (int i=0; i<v.size(); i++)
    cout<< v[i]<< " ";
  cout<<endl;
}
void print_vec2(vector<int> &v, int len)
{
  for (int i=0; i<len; i++)
    cout<< v[i]<< " ";
  cout<<endl;
}


void print_list(list<int> &l)
{
  for (auto p=l.begin(); p!=l.end(); p++)
    cout<<*p <<" ";
  cout<<endl;
}

void test1()
{
  vector<int> arr;

  arr.push_back(1);
  arr.push_back(2);
  arr.push_back(3);
  arr.push_back(4);

  {
    auto p = arr.begin();
    for (; p!=arr.end(); ++p) {
      if (*p == 1)
        break;
    }
    if (p !=arr.end())
      arr.erase(p);
    else
      cout<< "p is null\n";
  
    print_vec(arr);
  }
  list<int> l;
  l.push_back(1);
  l.push_back(2);
  l.push_back(3);
  l.push_back(4);
  auto p = l.begin();
  for (; p!=l.end(); ++p) {
    if (*p == 3)
      break;
  }
  if (p !=l.end())
    l.erase(p);
  else
    cout<< "p is null\n";
  print_list(l);

  char str[]="1231231";
  cout<< "strlen"<< strlen(str)<<endl;
}
struct node {
  struct node *left, *right;
  int c;
  node(int c=0): c(c) {};
};

void tree_preorder(node *root)
{
  vector<int> result;
  stack<node*> s;
  if (root)
    s.push(root);
  while (!s.empty()) {
    node *p= s.top();
    s.pop();
    result.push_back(p->c);

    if (p->right)
      s.push(p->right);
    if (p->left)
      s.push(p->left);
  }
  print_vec(result);
}
void tree_levelorder(vector<node *> v, vector<int> &result)
{
  vector<node *> tmp;
  
  for (int i=0; i<v.size(); i++) {
    node *p = v[i];
    result.push_back(p->c);
    if (p->left)
      tmp.push_back(p->left);
    if (p->right)
      tmp.push_back(p->right);
  }
  if (tmp.size())
    tree_levelorder(tmp, result);
}
node * init_tree2(int *arr, int len, int index)
{
  if (arr[index] == -1) {
    return NULL;
  }
  node *root = new node(arr[index]);
  
  if (2*index+1<len) {
    root->left = init_tree2(arr, len, 2*index+1);
  }
  if (2*index+2<len)
    root->right = init_tree2(arr, len, 2*index+2);
  
  return root;
  
}


void test2()
{
  int arr[]= {1,2,3,4,5,-1,7};
  int n = sizeof(arr)/sizeof(arr[0]);
  node *root;
  
  root = init_tree2(arr, n, 0);
  tree_preorder(root);

}
void test3()
{
  int arr[]= {1,2,3,4,5,-1,7};
  int n = sizeof(arr)/sizeof(arr[0]);
  node *root;
  vector<node *> v;
  vector<int> ret;
  
  root = init_tree2(arr, n, 0);
  v.push_back(root);
  tree_levelorder(v, ret);
  //reverse(ret.begin(), ret.end());
  print_vec(ret);
  
}
void test4()
{
  vector<int> v;
  v.insert(v.begin(), 1);
  v.push_back(2);
  v.push_back(3);
  v.insert(v.end()-1, 4);
  cout<< *(v.end()) <<endl;
  print_vec(v);

  list<int> l;
  list<int>::iterator p;
  l.push_back(1);
  l.push_back(2);
  l.push_back(3);
  print_list(l);
  
}
void print_list2(list<node *> &l)
{
  for (auto p=l.begin(); p!=l.end(); p++)
    cout<<(*p)->c <<" ";
  cout<<endl;
}
void print_flatten(node *root)
{
  for (node *tmp=root; tmp; tmp=tmp->right)
    cout<< tmp->c<<" ";
  cout <<endl;

}
void flatten(node *root)
{
  if (!root)
    return;
  flatten(root->left);
  flatten(root->right);
  if (root->left) {
    node * tmp;
    for (tmp = root->left; tmp->right; tmp=tmp->right)
      ;
    tmp->right = root->right;
    root->right = root->left;
    root->left = NULL;
  }
  //print_flatten(root);
}
void test5()
{
  int arr[]= {1,2,3,4,5,-1,7};
  int n = sizeof(arr)/sizeof(arr[0]);
  node *root;
  list<node *>l;
  root = init_tree2(arr, n, 0);
  flatten(root);
  for (node *tmp=root; tmp; tmp=tmp->right)
    cout<< tmp->c<<" ";
  cout <<endl;
  
}

int min_total(vector<vector<int> >&tri)
{
  for (int i=tri.size()-2; i>=0; i--)
    for (int j=0; j<i+1; j++)
      tri[i][j] += min(tri[i+1][j], tri[i+1][j+1]);
  return tri[0][0];
}
int min_total2(vector<vector<int> >&tri)
{
  for (int i=1; i<= tri.size()-1; i++) {
    for (int j=0; j<i+1; j++) {
      if (j==0)
        tri[i][j] += tri[i-1][j];
      else if (j == i)
        tri[i][j] += tri[i-1][j-1];
      else 
        tri[i][j] += min(tri[i-1][j-1], tri[i-1][j]);
      
      //tri[i+1][j] += tri[i][j];
      cout<< tri[i][j]<< " " ;
    }
    cout<<endl;
  }
  return tri[0][0];

}
void test6()
{
  vector<vector<int> >tri{{2}, {3,4}, {6,5,7}, {4,1,8,3}};
  //cout<< min_total(tri)<<endl;
  min_total2(tri);
  print_vec(tri[3]);
}

int remove_dup2(vector<int> &arr)
{
  int index=0, can_dup=1;
  for (int i=0; i<arr.size(); i++) {
    if (arr[i] != arr[index]) {
      arr[++index]  = arr[i];
      can_dup=1;
    }
    else if (arr[i] == arr[index] && can_dup) {
      arr[++index] = arr[i];
      can_dup = 0;
    }
  }
  return index+1;
}
void test7()
{
  vector<int> arr{1,1,1,2,2,2,3,3,4,5,6,6,6};
  int len = remove_dup2(arr);
  print_vec2(arr, len);
}

void test8()
{
  int a= 4, b=5;
  int &r1=a, &r2=a;
  r2= 10;
  cout<<a<<endl;
  r1= b;
  cout<<a<< endl;
}
int g_n =4;
int &setvalue() {return g_n;}
void test9()
{
  setvalue()=10;
  cout<<g_n<<endl;
  int n =100, m;
  const int &r=n;
  //r = 200; //can't compile

  const int *p=&n;
  //*p=5;  //can't compile
  p =&m;

  const int *p1;
  int *p2;
  p1= p2;
  //p2=p1; //can't compile
  p2 = (int *)p1;

  p2 = new int;
  delete p2;
  
}
void my_test(const char *p)
{
  //strcpy(p, "this");  //can't compile
  printf("%s", p);
}
int my_max(int a=1, int b=2)
{
  if (a>b)
    return a;
  return b;
}
struct class1{
  int i;
  class1(int i=0): i(i) {};
  void hello() {cout<< "hello" <<endl;};
};
void test10()
{
  class1 o;
  //  o.val();
}

int median_2sort(vector<int>&v1, vector<int>&v2)
{
  int i1, i2, i=0;
  int mid= (v1.size()+v2.size())/2, ret;
  for (i1=0, i2=0; i1<v1.size() && i2< v2.size(); ) {
    if (v1[i1] <v2[i2]) {
      ret = v1[i1++];
    }
    else
      ret = v2[i2++];

    if (++i == mid)
      return ret;
  }
  if (i1 == v1.size())
    while (i2<v2.size() && i++<mid)
      ret = v2[i2++];
  if (i2 == v2.size())
    while (i2<v1.size() && i++<mid)
      ret = v1[i1++];
    
  return ret;
}
void test11()
{
  vector<int> v1{1,2,3};
  vector<int> v2{4,5,6,7,8};
  cout << median_2sort(v1, v2) <<endl;
}

int longest_consec(vector<int> &nums)
{
  unordered_map<int, bool> used;
  for (auto i:nums) {
    used[i] = false;
    //cout<<i<<endl;
  }
  int longest =0;
  for (auto i:nums) {
    //cout<<i<<endl;
    if (used[i])
      continue;
    int length =1;
    used[i] = true;
    for (int j=i+1; used.find(j)!= used.end(); j++) {
      used[j] = true;
      length++;
    }
    for (int j=i-1; used.find(j)!= used.end(); j--) {
      used[j] = true;
      length++;
    }
    longest = max(longest, length);
  }
  return longest;
}
void test12()
{
  vector<int> v{100, 4, 200, 1,3 ,2};
  int ret = longest_consec(v);
  cout <<ret;

  class1 *p = NULL;
  p->hello();
}

list<int> remove_dup(list<int> &l)
{
  int last = -1, poped=0;
  list<int> l2;
  for (auto p=l.begin(); p!= l.end(); ++p) {
    if (*p != last) {
      l2.push_back(*p);
      last = *p;
      poped= 0;
    }
    else {
      if (!poped) {
        l2.pop_back();
        poped = 1;
      }
    }
  }
  return l2;
}
list<int> remove_dup2(list<int> &l)
{
  int last = -1;
  list<int> l2;
  for (auto p=l.begin(); p!= l.end(); ++p) {
    if (*p != last) {
      l2.push_back(*p);
      last = *p;
    }
    else
      ;
  }
  return l2;
}

void test13()
{
  list<int> l{1,2,2,2,3,4,4,4,5};
  list<int> l2;
  l2 = remove_dup(l);
  print_list(l2);
  l2 = remove_dup2(l);
  print_list(l2);
  
}

/*
struct spin_node{
  int val;
  int x,y;
}
generate n*n array of spin_node
  when iterate, the same direction node mumber goes:
    n,n-1,n-1,n-2,n-2,n-3,n-3,... until n-k is 0.
  the direction goes:
    r,d,l,u,r,...
  tmp=n; val=1; dir=0;
  x=0; y=0; twice =true;
  while(tmp)   {
    for (i=0;i<tmp; i++) {
       spin_node node(val++, x, y);
       arr.push_back(node);
       next_cord(dir, x, y);
    }
    next_dir(dir);
    if (twice)  
     tmp--; 
    twice = !twice;
  }


from the array generate the matrix.
  for each node in spin_node_arr, 
    find [i,j] where i=0~n, j=0~n. and print
*/
struct spin_node{
  int val;
  int x,y;
  spin_node(int x,int y, int val):x(x),y(y),val(val) {};
};
void next_node(int dir, int &x, int &y)
{
  if (dir ==0) //r
    y++;
  else if (dir ==1)
    x++;
  else if (dir == 2)
    y--;
  else if (dir == 3)
    x--;
}
void generate_node_arr(vector<spin_node>&v, int n)
{
  int tmp=n, val= 1, dir=0;
  bool twice =true;
  int x=0, y=0;
  while(tmp) {
    for (int i=0; i<tmp; i++) {
      spin_node node(x, y, val++);
      v.push_back(node);
      if (i < tmp-1) {
        next_node(dir, x,y);
      }
      else {
        if (++dir == 4)
          dir = 0;
        next_node(dir, x,y);
      }
    }
    //cout<<tmp<<endl;
    if (twice)
      tmp--;
    twice = !twice;
  }
}
void print_vec3(vector<spin_node> &v)
{
  for (int i=0; i<v.size(); i++)
    cout<< v[i].x<< ","<<v[i].y <<" ";
  cout<<endl;
}
int find_spin_node(int x, int y, vector<spin_node>&v)
{
  for (int i=0; i<v.size();i++)
    if (v[i].x==x && v[i].y==y)
      return v[i].val;
  return 0;
}

void test14()
{
  vector<spin_node>v;
  int n=5;
  generate_node_arr(v, n);
  print_vec3(v);
  for (int i=0; i<n; i++)
    for (int j=0; j<n; j++) {
      cout<< find_spin_node(i,j,v) <<" ";
    }
  cout<<endl;
}

class Complex {
	public:
		 double real, imag;
		  Complex( int i ) { //
			  cout << "int constru" << endl;
			  real = i; imag = 0;
			   }
		   Complex( double r, double i )
			    { real = r; imag = i; }
};
class Demo {
	int id;
	public:
	Demo( int i )
	{
		id = i;
		cout << "int cons: "<< i << endl;
	}
	~Demo()
	{
		cout <<  "int destruc"<< id << endl;
	}
};
void Func(){
	static Demo d2(2);
	Demo d3(3);
	cout << "func"<< endl;
}

class CMyclass {
	 int n;
	 static int s;
	  };


int main()
{
	cout<< sizeof(CMyclass) <<endl;
}
