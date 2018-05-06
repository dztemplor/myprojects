#include <iostream>
#include <vector>
#include <list>
#include <queue>
#include <stack>
#include <algorithm>
#include <unordered_map>
#include <map>
#include <unordered_map>
#include <string.h>
#include <sstream>
 #include <unistd.h>


#include <string>

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
  struct node *left, *right, *next, *parent;
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
void tree_inorder(node *root)
{
  vector<int> result;
  stack<node*> s;
  node *p = root;
  while (!s.empty() || p) {
    if (p) {
      s.push(p);
      p = p->left;
    }
    else  {
      p = s.top();
      s.pop();
      result.push_back(p->c);
      p = p->right;
    }
  }
  print_vec(result);
}
void tree_levelorder(vector<node *> &v, vector<int> &result)
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
    if (root->left)
      root->left->parent = root;
  }
  if (2*index+2<len) {
    root->right = init_tree2(arr, len, 2*index+2);
    if (root->right)
      root->right->parent = root;
  }
  return root;
  
}


void test2()
{
  int arr[]= {1,2,3,4,5,-1,7};
  int n = sizeof(arr)/sizeof(arr[0]);
  node *root;
  
  root = init_tree2(arr, n, 0);
  //tree_preorder(root);
  tree_inorder(root);
  
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
#if 0
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
#endif
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
/*
lru_cache contains get(), set()
has a map and list obj.
list head contains latest element, tail contains oldest.
get: find the node in list, put the node in list head, update map
set: if exceed list size, erase the tail node, insert at list head,
    update 2 nodes in map
    if find the node, just update value info
*/
struct lru_cache {
  struct node {
    int k,v;
    node(int k=0, int v=0): k(k),v(v){};
  };
  int size;
  list<node> l;
  map<int, list<node>::iterator>h;
  lru_cache(int size =0): size(size) {};
  int get(int key) {
    if (h.find(key) == h.end())
      return -1;
    l.splice(l.begin(), l, h[key]);
    h[key] = l.begin();
    return h[key]->v;
  };
  void set(int key, int value) {
    if (h.find(key) == h.end()) {
      if (l.size() == size) {
        h.erase(l.back().k);
        l.pop_back();
      }
      l.push_front(node(key, value));
      h[key] = l.begin();
    }
    else {
      h[key]->v= value;
      l.splice(l.begin(), l, h[key]);
      h[key] = l.begin();
    }
  };
  void print() {
    for (auto p=l.begin(); p!=l.end(); p++) {
      cout<<p->k<<","<<p->v <<" ";
    }
    cout <<endl;
  };
};


void test15()
{
  lru_cache cache(10);
  cout << cache.get(1)<<endl;
  for (int i=0; i<12; i++)
    cache.set(i, i+100);
  //cache.print();
}

string int2roman(int num)
{
  const int radix[] = {1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1};
  const string symbol[] ={"M", "CM", "D", "CD", "C", "XC",
                          "L", "XL", "X", "IX", "V", "IV", "I"};

  string roman;
  for (int i=0; num>0; i++) {
    int count = num/radix[i];
    num %= radix[i];
    for (; count>0; --count) roman+= symbol[i];
  }
  return roman;
}

void test16()
{
  cout << int2roman(3100)<<endl;
  cout << int2roman(3990)<<endl;
  cout << int2roman(590)<<endl;
  cout << int2roman(190)<<endl;
  cout << int2roman(59)<<endl;
  cout << int2roman(9)<<endl;
}
int roman2int(char * str)
{
/*
for each char in s,
change char to number, watch:
 C: need check CM,CD,C
 X: need check XC,XL,X
 I: need check IX, IV, I

add these numbers, return 
*/  
  const int radix[] = {1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1};
  const string symbol[] ={"M", "CM", "D", "CD", "C", "XC",
                          "L", "XL", "X", "IX", "V", "IV", "I"};

  int num = 0;

  int n = sizeof(radix)/sizeof(radix[0]);
  for (int i=0; i<strlen(str);) {
    int index = -1;
    int j=0;
    for (j=0; j<n; j++) {
      if (str[i]== 'C' || str[i]== 'X' || str[i]=='I') {
        const char *tmp = symbol[j].c_str();
        if ( str[i+1] && (str[i]==tmp[0] && str[i+1]==tmp[1])) {
          i+=2;
          break;
        }
      }
      if (str[i] == symbol[j].at(0)) {
        i++;
        break;
      }
    }
    index = j;
    if (index <0)
      return 0;
    num += radix[index];
  }
  return num;
}

void test17()
{
#if 0
  cout<<roman2int("MMMC")<<endl ;
  cout<<roman2int("MMMCMXC")<<endl;
  cout<<roman2int("DXC")<<endl;
  cout<<roman2int("CXC")<<endl;
  cout<<roman2int("LIX")<<endl;
                          cout<<roman2int("IX")<<endl;
#endif                                                 
};
bool get_token(string::iterator &p, string *token, string &path)
{
  int len;
  p++;
  auto j = find(p, path.end(), '/');
  *token = string(p, j);
  p = j;
  return j != path.end();
}
string simplify_path(string &path)
{
/*
list<char *> l
for each str between /,
   if str == .., if l.size(),  pop_back
   if str == . or empty, continue
   else l.push_back
get_token(char *p, &token)
  return 0 when end
  /home/: token is home, p goes to next /
according to l, construct string and return
  start with /
  for each str in l, add str/
*/
  string ret, token;
  auto p=path.begin();
  list<string >l;
  while (1) {
    int cont;
    cont = get_token(p, &token, path);

    if (token == "..") {
      if (l.size())
        l.pop_back();
    }
    else if (token=="." || token.empty())
      ;
    else
      l.push_back(token);
    if (!cont)
      break;
  }

  stringstream out;
  if (!l.size())
    out <<"/";
  else
    for (auto str=l.begin(); str!=l.end(); str++) {
      out<<'/'<<*str;
    }
  
  return out.str();
}

void test_18()
{
    int i;
    string s;
    s = "/a/./b/../../c/";
    cout << simplify_path(s)<<endl;
    s = "/a/./b/../c";
    cout << simplify_path(s)<<endl;
    s = "/../";
    cout << simplify_path(s)<<endl;
    s = "/home///foo/";
    cout << simplify_path(s)<<endl;
}

void tree_preorder2(node *root)
{
/*
use a stack to store node, 
first put root in stack
for each node, print top, pop node, push right, left, 
while( !s.empty())
*/
  stack<node *>s;
  node *tmp;
  if (!root)
    return;
  s.push(root);
  while(!s.empty()) {
    tmp = s.top();
    cout<< tmp->c<<" ";
    s.pop();
    if (tmp->right)
      s.push(tmp->right);
    if (tmp->left)
      s.push(tmp->left);
  }
  cout<<endl;
}

void test_19()
{
  int arr[]= {1,2,3,-1,5,4,-1};
  int n = sizeof(arr)/sizeof(arr[0]);
  node *root;
  
  root = init_tree2(arr, n, 0);
  tree_preorder2(root);

}
struct node3 {
  struct node3 *left, *right;
  int i,j;
  node3(int i=0, int j=0): i(i),j(j) {};
};

void robot_walk(node3 *p, int m, int n)
{
  struct node3 r;
  
}
int uniquePaths(int r, int c,int m, int n) {
  if (r>m || c>n) return 0; 
  if (r==m || c==n) return 1;
  return uniquePaths(r+1, c, m , n) + uniquePaths(r, c+1, m, n);
}

void test_20()
{
  cout<< uniquePaths(0, 0, 3,5)<<endl;
}
int tree_height(node *root)
{
  /*
if root is null, return 0;
else return bigger height of left and right, plus 1.
*/  
  if (!root)
    return 0;
  //cout<< "c "<<root->c ;
  int left, right;
  
  left = tree_height(root->left);
  right = tree_height(root->right);
  if (left<0 || right<0 || abs(left-right)>1)
    return -1;
  return max(left, right)+1;
}
bool tree_balance(node *root)
{
  /*
recursive, find height of root->left and root->right, check if diff >1
to get height
*/
  if (!root)
    return true;
  return tree_height(root)>=0;
}
void test_22()
{
  int arr[]= {1,-1,2,-1,-1,-1,3};
  int n = sizeof(arr)/sizeof(arr[0]);
  node *root;
  bool ret;
  
  root = init_tree2(arr, n, 0);
  //ret = tree_balance(root);
  //cout<<ret<<endl;
  
  int arr2[]= {1,2,3,4,-1,-1,5,6,-1};
  n = sizeof(arr2)/sizeof(arr2[0]);
  root = init_tree2(arr2, n, 0);
  ret = tree_balance(root);
  cout<<ret<<endl;
}
void flatten(node *root)
{
  if (!root)
    return;
  flatten(root->left);
  flatten(root->right);
  if (root->left) {
    node *tmp;
    for (tmp=root->left; tmp->right; tmp=tmp->right)
      ;
    tmp->right = root->right;
    root->right = root->left;
    root->left = NULL;
  }
}
void test_23()
{
  int arr[]= {1,2,3,4,5,-1,7};
  int n = sizeof(arr)/sizeof(arr[0]);
  node *root, *tmp;
  list<node *>l;
  root = init_tree2(arr, n, 0);
  flatten(root);
  for (tmp=root; tmp->right; tmp=tmp->right)
    cout<< tmp->c<<" ";
  cout<<tmp->c <<" ";
  cout <<endl;
  
}
void popu_next(vector<node *>&v)
{
/*
v contains level-order nodes.
for each node in v,
  node[i]->next = node[i+1]; and end node's next is NULL

*/
  if (!v.size())
    return;
  vector<node *>tmp;
  for (int i=0; i<v.size(); i++)   {
    if (i== v.size()-1)
      v[i]->next = NULL;
    else
      v[i]->next = v[i+1];
  }
  for (int i=0; i<v.size(); i++) {
    if (v[i]->left)
      tmp.push_back(v[i]->left);
    if (v[i]->right)
      tmp.push_back(v[i]->right);
  }
  popu_next(tmp);
}
void popu_next2(node *root)
{
  /*
from root, create a list to link left and right. 
send the list head to recur, and creat list to link left and right
*/
  node l, *p, *p2, *p3;
  if (!root)
    return;
  l.next = NULL;
  for (p=root, p3=&l; p; p=p2) {
    p2 = p->next;
    if (p->left) {
      p3->next = p->left;
      p3= p3->next;
    }
    if (p->right) {
      p3->next = p->right;
      p3=p3->next;
    }
  }
  p3->next = NULL;
  popu_next2(l.next);
}
void print_tree_next(node * root)
{
  if (!root)
    return;
  cout<<endl;
  cout<< root->c << " ";
                    if (root->next)  cout<<root->next->c;
                          print_tree_next(root->left);
                    print_tree_next(root->right);
}
void test_24()
{
  int arr[]= {1,2,3,4,5,-1,7};
  int n = sizeof(arr)/sizeof(arr[0]);
  node *root, *tmp;
  vector<node *>v;
  
  root = init_tree2(arr, n, 0);
  v.push_back(root);
  popu_next2( root);
  print_tree_next(root);
  cout<<endl;
}

void print_preorder(node *root)
{
  if (!root)
    return;
  cout<< root->c<<" ";
  print_preorder(root->left);
  print_preorder(root->right);
}
void print_inorder(node *root)
{
  if (!root)
    return;
  print_inorder(root->left);
  cout<< root->c <<" ";
  print_inorder(root->right);
}
void print_postorder(node *root)
{
  if (!root)
    return;
  print_postorder(root->left);
  print_postorder(root->right);
  cout<< root->c <<" ";
}

int node_find(int *arr, int val, int len)
{
  int i;
  for (i=0; i<len; i++) {
    if (arr[i] == val)
      break;
  }
  if (i>len)
    printf("not found\n");
  return i;
}
int times;
node * construct_tree(int *preorder, int * inorder, int len)
{
  /*
preorder's first is root node, find index in inorder,
i1 is start of preorder, i2 is start of inorder
in inorder, the left part is left tree (len1), right part is right tree(len2).
in preorder, left part starts from 2nd node with len1, right part starts from end+1 with len2
watch out len1 is 0 or len2 is 0
*/
  node *root;
  int index, len1, len2;
  if (!preorder || !inorder)
    return NULL;
  root = new node(preorder[0]);
  index = node_find(inorder, root->c, len);
  
  len1 = index;

  //printf("%d %d %d len1 %d\n", root->c, len ,index, len1);

  if (len1<=0)
    root->left = NULL;
  else 
    root->left = construct_tree(preorder+1, inorder, len1);

  len2 = len-1-index;
  if (len2<=0)
    root->right = NULL;
  else 
    root->right = construct_tree(preorder+len1+1, inorder+index+1, len2);

  return root;
}

node * init_tree3(int *preorder, int *inorder, int len)
{
  return construct_tree(preorder, inorder, len);
}
void test_25()
{
  int arr1[]= {1,2,4,5,3,7};
  int arr2[]= {4,2,5,1,3,7};
  int n = sizeof(arr1)/sizeof(arr1[0]);
  node *root;
  
  root = construct_tree(arr1, arr2, n);
  print_preorder(root);
  cout<<endl;
  print_inorder(root);
  cout<<endl;
}
node * construct_tree2(int *inorder, int *postorder, int len)
{
  /*
arr1 is inorder, arr2 is postorder, len is arr length
arr2[len-1] is root node.
find index in inorder, get left tree size and right tree size, 
adjust arr1 and arr2, then recurse
   */  
  node *root;
  int index, len1, len2;
  if (len<=0)
    return NULL;
  root = new node(postorder[len-1]);

  index = node_find(inorder, root->c, len);
  len1 = index;
  if (len1<=0)
    root->left = NULL;
  else
    root->left = construct_tree2(inorder, postorder, len1);
  len2 = len-index-1;
  if (len2<=0)
    root->right = NULL;
  else
    root->right = construct_tree2(inorder+index+1, postorder+len1, len2);
  
  return root;
}
void test_26()
{
  int arr1[]= {4,2,5,1,3,7}; //inorder
  int arr2[]= {4,5,2,7,3,1}; //postorder
  int n = sizeof(arr1)/sizeof(arr1[0]);
  node *root;
  
  root = construct_tree2(arr1, arr2, n);
  print_postorder(root);
  cout<<endl;
  print_inorder(root);
  cout<<endl;
}
int find_in_level(int *inorder, int *level, int len)
{
  /*
for each item in inorder, find pos in level, 
return the first pos in level.
*/
  int i,j, m=10000;
  for (i=0; i<len; i++) {
    for (j=0; ; j++) {
      if (inorder[i] == level[j]) {
        m = min(m, j);
        break;
      }
    }
  }
  return m;
}
node * construct_tree3(int *inorder, int *level, int len, int first)
{
  /*
first is tell the root is at level[0]
if not first, find all items in inorder in level, the first one is root.
after get root, find it in inorder, get index, 
then recurse, inorder split to 2 parts, level need proceed one. len only works on inorder
*/
  node *root;
  int index, index2, len1, len2;
  if (first) {
    root= new node(level[0]);
    index2 = 0;
    first = 1;
  }
  else {
    index2 = find_in_level(inorder, level, len);
    root = new node(level[index2]);
  }
  index = node_find(inorder, root->c, len);
  
  len1 = index;
  if (len1<=0)
    root->left = NULL;
  else
    root->left = construct_tree3(inorder, level+index2+1, len1, 0);

  len2 = len-index-1;
  if (len2<=0)
    root->right = NULL;
  else
    root->right = construct_tree3(inorder+index+1, level+index2+1, len2, 0);


  return root;
}
void test_28()
{
  int arr1[]= {4,2,5,1,3,7}; //inorder
  int arr2[]= {1,2,3,4,5,7}; //levelorder
  int n = sizeof(arr1)/sizeof(arr1[0]);
  node *root;
  
  root = construct_tree3(arr1, arr2, n, 1);
  print_postorder(root);
  cout<<endl;
  print_inorder(root);
  cout<<endl;  
  unordered_map<char, int> m;
  cout<< sizeof(m) <<"dz" <<endl;
}
#if 0
template <class T> struct my_comp {
  bool operator() (const T& x, const T& y) const {return strcmp(x, y);}
};
#endif
#if 0
struct my_comp {
  bool operator() (const char * x, const char * y) const {
    //printf(" cmp %s %s\n", x,y);
    return strcmp(x,y);};
};
#endif
struct word {
  string str;
  int num;
  word(char * in):num(0) {str = string(in);};
  
};
//why char * not work?
void sort_words()
{
  /*
use set, when add a new word, c
*/
  const char *fn= "controls.xml";
  FILE *fp = fopen(fn,"r");
  char buf[1024] = {0};
  map<string, int> mm;
  multimap<int, string, greater<int>> mm2;
  if (!fp)
    return;

  while (fgets(buf, sizeof(buf)-1, fp)) {
    char *str;
    string word;
    for (str = strtok(buf, " \n\t"); str; str=strtok(NULL, " \n\t")) {
        word = string(str);
        auto ret = mm.insert(pair<string, int>(word, 1));
        if (!ret.second) 
          ++ret.first->second;
    }

    memset(buf, 0, sizeof(buf));
  }
  printf("%lu \n", mm.size());
  int i=0;
  //printf("dz %d\n", mm.count("a"));
  for (auto p=mm.begin(); p!=mm.end()  ; i++, ++p ) {
    //cout<< p->first <<" "<< p->second <<endl;
    mm2.insert({p->second, p->first});
    //cout<< (*p).first << " "<< mm.count((*p).first)<<endl;
    //printf("%s %lu\n", (*p).first, mm.count((*p).first));
  }
  i = 0;
#if 0
  for (auto p=mm2.begin(); p!=mm2.end() && i<50; i++, ++p)
    cout<< p->first << " " << p->second <<endl; 
#endif
  //sort(mm.begin(), mm.end());
}

void test_27()
{
  //  for (int i=0; i<10; i++)
    sort_words();
}
bool sudoku_valid(vector<vector<int> >&board, int i, int j)
{
  /*
board[i][j] is newly added, valid if all below meets:
 row no repeat
 column no repeat
 3*3 square no repeat
*/
  int s1, s2;
  for (int k=0; k<9; k++) {
    if (k!=j && board[i][k] == board[i][j])
      return false;
  }
  for (int k=0; k<9; k++) 
    if (k!=i && board[k][j] == board[i][j])
      return false;
  s1= i/3*3;
  s2= j/3*3;
  for (int k1=s1; k1<3+s1; k1++)
    for (int k2=s2; k2<3+s2; k2++)
      if ( (k1!=i || k2!=j) && board[k1][k2]==board[i][j])
        return false;
 
  return true;
}
bool resolve_sudoku(vector<vector<int> >&board)
{
  /*recursive, 
for each empty node, try it and recurse .*/
  for (int i=0; i<9; i++)
    for (int j=0; j<9; j++){
      if (board[i][j])
        continue;

      for (int k=1; k<=9; k++) {
        board[i][j] = k;
        if (sudoku_valid(board, i, j) && resolve_sudoku(board))
          return true;
        board[i][j]= 0;
      }
      return false;
    }
  return true;
}
struct sudoku_step {
  int i,j,val;
  sudoku_step(int i=0, int j=0, int val=0):i(i),j(j),val(val) {};
};
void find_next_pos(vector<vector<int> >&board, int &nexti, int &nextj)
{
  int i,j;
  for (i=0; i<9; i++) {
    for (j=0; j<9; j++)
      if (!board[i][j])
        break;
      if (j<9)
        break;
  }
  nexti = i;
  nextj = j;
}
bool resolve_sudoku2(vector<vector<int> >&board)
{
  /*non recursive,
maintain stack<sudoku_step>
each time finds a possible step, stack push
each time all tries failed, recover board, pop the stack and retry next val
  if the poped item has no next val, recover board, and pop again.
loop until stack size is (empty_node.size),
how to detect no solution?

*/
  int empty_size = 0;
  for (int i=0; i<9; i++)
    for (int j=0; j<9; j++)
      if (!board[i][j])
        empty_size++;
  
  stack<sudoku_step> s;
  sudoku_step step, last;
  int time=0, revert=0;
  {
  int i,j;
  for (i=0; i<9; i++) {
    for (j=0; j<9; j++)
      if (!board[i][j])
        break;
      if (j<9)
        break;
  }
  sudoku_step step(i,j);
  s.push(step);
  } 
  //todo: i,j can start from s.top(); k starts from last's val
  /*
  if revert,stay on top item, and val++
  else, find next empty item.
*/ 
 while (s.size()!= empty_size) {
    int i,j,k;
    sudoku_step last = s.top();

    
    i= last.i; 
    j=last.j;
    
    int valid=0;
    for (k=last.val+1; k<=9; k++) {
      board[i][j] = k;
      if (sudoku_valid(board, i, j)) {
        
        sudoku_step &step = s.top();
        step.val = k;
        //s.push(step);//?
        valid =1;
        //printf("step %d %d %d\n", i,j,k);
        break;
      }
    }
    if (!valid) { 
      if (!s.size())
        return false;
      last = s.top();
      board[last.i][last.j] = 0;
      s.pop();
    }
    else {
      int nexti, nextj;
      find_next_pos(board, nexti, nextj);
      sudoku_step step(nexti, nextj);
      s.push(step);
    }

    last = s.top();
    //printf("last %d %d %d\n", last.i, last.j, last.val);
  }
 
  return true;
}

void print_sudoku(vector<vector<int> >&board)
{
  for (int i=0; i<9; i++) {
    for (int j=0; j<9; j++)
      printf("%d ", board[i][j]);
    printf("\n");
  }
  printf("\n");
}
void test_29()
{
  vector<vector<int> >board = {{5,3,0,0,7,0,0,0,0},
                               {6,0,0,1,9,5,0,0,0},
                               {0,9,8,0,0,0,0,6,0},
                               {8,0,0,0,6,0,0,0,3},
                               {4,0,0,8,0,3,0,0,1},
                               {7,0,0,0,2,0,0,0,6},
                               {0,6,0,0,0,0,2,8,0},
                               {0,0,0,4,1,9,0,0,5},
                               {0,0,0,0,8,0,0,7,9}};
  
  resolve_sudoku(board) ;
  print_sudoku(board);
  board = {{5,3,0,0,7,0,0,0,0},
                               {6,0,0,1,9,5,0,0,0},
                               {0,9,8,0,0,0,0,6,0},
                               {8,0,0,0,6,0,0,0,3},
                               {4,0,0,8,0,3,0,0,1},
                               {7,0,0,0,2,0,0,0,6},
                               {0,6,0,0,0,0,2,8,0},
                               {0,0,0,4,1,9,0,0,5},
                               {0,0,0,0,8,0,0,7,9}};
  
  resolve_sudoku2(board) ;
  print_sudoku(board); 

}

int get_remove(stack<int>  &s)
{
  int result = s.top();
  s.pop();
  if (s.empty())
    return result;
  else {
    int last= get_remove(s);
    s.push(result);
    return last;
  }
    
}

void stack_reverse(stack<int> &s)
{
  if (s.empty())
    return;
  int i = get_remove(s);
  stack_reverse(s);
  s.push(i);
}
void test_30()
{
  stack<int> s;
  s.push(3);
  s.push(2);
  s.push(1);
  stack_reverse(s);
  while (!s.empty()) {
    cout<<s.top() <<" ";
    s.pop();
  }
  cout<<endl;
}
struct node2 {
  int c;
  struct node2 *prev, *next;
  node2(int c=0):c(c),prev(NULL),next(NULL) {};
};
void tree2list(node *root, node2 * &l)
{
  /*
root is BST.
recurse root inorder, put each node at l's tail
*/
  if (!root)
    return;
  tree2list(root->left, l);
  struct node2 *n = new node2(root->c);
  l->next = n;
  n->prev = l;
  l = l->next;

  tree2list(root->right, l);
}
void print_list(node2 *l)
{
  node2 *p;
  for (p=l; p->next; p=p->next)
    cout<< p->c <<" ";
  cout<<p->c << " ";
  cout<<endl;
  
  cout<< "reverse"<<endl;
  for (; p!=l; p=p->prev)
    cout<<p->c<< " ";
  cout<<p->c <<" ";
  cout<<endl;
  
}

void test_31()
{
  int arr[]= {6,4,7,2,5,-1,9};
  int n = sizeof(arr)/sizeof(arr[0]);
  node *root;
  node2 *l, *tmp, *l2;
  root = init_tree2(arr, n, 0);

  l = new node2(0);
  l2 = l;
  tree2list(root, l);
  print_list(l2->next);
}



void sort_words2()
{
  const char *fn= "controls.xml";
  FILE *fp = fopen(fn,"r");
  char buf[1024] = {0};
  unordered_map<string, int> mm;
  if (!fp)
    return;
  int num=0;
  while (fgets(buf, sizeof(buf)-1, fp)) {
    char *str;
    string word;
    for (str = strtok(buf, " \n\t"); str; str=strtok(NULL, " \n\t")) {
        word = string(str);
        ++mm[word];
        //mm.insert(pair<string, int>(word, num));
    }

    memset(buf, 0, sizeof(buf));
  }
  vector<int> v;
  printf("%lu num %d\n", mm.size(), num);
  int i=0;
  for (auto p=mm.begin(); p!=mm.end() ;  ++p)
    v.push_back(p->second);
  sort(v.begin(), v.end(), greater<int>());
  for (i=0; i<10; i++)
    cout<< v[i]<<endl;
    //cout<< p->first<<  " " <<p->second <<endl;
}
void test_28_1()
{
  sort_words2();
}



void test_29_1()
{
  int arr[]={2,4,5,1,3,8,1};
  priority_queue<int, vector<int> > q(arr, arr+6);
  while (!q.empty()) {
    cout<< q.top()<<" ";
    q.pop();
  }
  cout<<endl;
}

void morris_inorder(node *root)
{
  node *cur=root;
  while (cur) {
    if (!cur->left) {
      cout<< cur->c <<" ";
      cur= cur->right;
    }
    else {
      node *n = cur->left;
      while (n->right && n->right!=cur)
        n = n->right;
      if (!n->right) {
        n->right = cur;
        cur= cur->left;
      }
      else {
        cout<<cur->c <<" ";
        n->right = NULL;
        cur = cur->right;
      }
    }
  }
}
                   
void morris_preorder(node *root)
{
  node *cur=root;
  while (cur) {
    if (!cur->left) {
       cout<< cur->c <<", ";
      cur= cur->right;
    }
    else {
      node *n = cur->left;
      while (n->right && n->right!=cur)
        n = n->right;
      if (!n->right) {
        cout<<cur->c<< ": ";
        n->right = cur;
        cur= cur->left;
      }
      else {
        n->right = NULL;
        cur = cur->right;
      }
    }
  }
}

void test_30_1()
{
  int arr[]= {1,2,3,4,5,6,7};
  int n = sizeof(arr)/sizeof(arr[0]);
  node *root;
  
  root = init_tree2(arr, n, 0);
  //tree_preorder(root);
  morris_inorder(root);
  cout<<endl;
  morris_preorder(root);
  cout<<endl;
}

void test_32()
{
  node2 l, *l2;
  cout << &l <<" "<<l2<<endl;
#if 1
  node2 * &&p = &l; 
  //p = l2;
  cout << &l<<" " << p<< endl;
#endif  
}
void print_nodevec(vector<node *>v)
{
  for (int i=0; i<v.size(); i++)
    printf("%d ", v[i]->c);
  printf("\n");
}
void tree_find_bound(vector<node *>l_level, vector<node *>r_level, vector<int> &left, vector<int> &right)
{
  /*
l_level and r_level contains left and right level nodes
generate left: for each node in l_level, 
   push leaf; if not leaf but is leftmost, push it
generate right: for each node in r_level
   push leaf; if not leaf but rightmost, push it
*/
  vector<node *> l_tmp, r_tmp;
  
  if (!l_level.size() && !r_level.size())
    return;
#if 1 
  if (l_level.size() == 1 && r_level.size()==1 && l_level[0]==r_level[0]) {
    node *root = l_level[0];
    if (root->left) {
      l_tmp.push_back(root->left);
      left.push_back(root->left->c);
    }
    if (root->right) {
      r_tmp.push_back(root->right);
      right.push_back(root->right->c);
    }
    return tree_find_bound(l_tmp, r_tmp, left, right);
  }
#endif  

  for (int i=0; i<l_level.size(); i++) {
    node *p = l_level[i];
    if (p->left)
      l_tmp.push_back(p->left);
    if (p->right)
      l_tmp.push_back(p->right);
  }


  for (int i=0; i<l_tmp.size(); i++) {
    node *p = l_tmp[i];
    if (!i)
      left.push_back(p->c);
    else if (!p->left && !p->right)
      left.push_back(p->c);
  }

  for (int i=0; i<r_level.size(); i++) {
    node *p = r_level[i];
    if (p->left)
      r_tmp.push_back(p->left);
    if (p->right)
      r_tmp.push_back(p->right);
  }
  for (int i=0; i<r_tmp.size(); i++) {
    node *p = r_tmp[i];
    if (i == r_tmp.size()-1)
      right.push_back(p->c);
    else if (!p->left && !p->right)
      right.push_back(p->c);
  }

  tree_find_bound(l_tmp, r_tmp, left, right);
}
void print_boundary(struct node *r)
{
  /*
question page 95
level traversal, vector<vector<int> >level
vector<int> left_bound, vector<right>right_bound
for each level, fill in left_bound and right_bound
print left_bound in normal order, and right_bound in reverse order
*/  
  vector<node *>l_level, r_level;
  vector<int>left_bound, right_bound;
  if (!r)
    return;
#if 0
  left_bound.push_back(r->left->c);
  right_bound.push_back(r->right->c);
  l_level.push_back(r->left);
  r_level.push_back(r->right);
#else
  l_level.push_back(r);
  r_level.push_back(r);
#endif
  
  tree_find_bound(l_level, r_level, left_bound, right_bound);
  print_vec(left_bound);
  print_vec(right_bound);
}
void test_33()
{
  int arr1[] = {1,2,4,7,8,11,13,14,3,5,9,12,15,16,10,6};
  int arr2[] = {2,7,4,8,13,11,14,1,15,12,16,9,5,10,3,6};
  int n= sizeof(arr1)/sizeof(arr1[0]);
  struct node *t;
  t = init_tree3(arr1, arr2, n);
#if 0
  print_preorder(t);
  cout<<endl;
  print_inorder(t);
#endif
  print_boundary(t);
}

void build_tree(char *str, vector<node *>&v)
{
  /*
for each node in v,
  creat a new node from string, put_back to vector, step str forward 
  assign node's left and right, note NULL condition.
  ensure str is adjusted, and recurse
*/
  vector<node *>tmp;
  node *n;
  if (!str || !str[0])
    return;
  
  for (int i=0; i<v.size(); i++) {
    node *parent = v[i];
    if (!strncmp(str, "#!", 2)) {
      parent->left = NULL;
    }
    else {
      n = new node(atoi(str));
      parent->left = n;
      tmp.push_back(n);
    }
    str+=2;
    if (!strncmp(str, "#!", 2)) {
      parent->right = NULL;
    }
    else {
      n = new node(atoi(str));
      parent->right = n;
      tmp.push_back(n);
    }
    str+= 2;
  }
  build_tree(str, tmp);
}

void test_34()
{
  char str[] = "1!2!3!4!#!#!5!#!#!#!#!";
  node root(1);
  vector<node *>v;
  v.push_back(&root);
  build_tree(str+2, v);
  print_preorder(&root);
  cout<<endl;
  print_inorder(&root);
  cout<<endl;
}


int get_sum(int *arr, int start, int end)
{
  int ret=0;
  for (int i=start; i<=end; i++)
    ret += arr[i];
  return ret;
}
void find_sum(int *arr, int len, int k, vector<int>& v)
{
  /*
int left, right, sum;
left from 0 to len-1,
  if right <left, right=left;
  calc sum arr[left..right], 
    if sum==k, update v, continue, 
    if sum>k, continue
    if sum<k, right++
*/
  int left=0, right=0, sum=0, sub_len;
  for (; left<len; left++) {
    //if (right<left)
    right = left;
    for (; (sum = get_sum(arr, left, right))<=k && right<len-1; right++ ) {
      if (sum == k) {
        sub_len = right+1-left;
        if (sub_len>v.size()) {
          v.clear();
          for (int i=left; i<=right; i++)
            v.push_back(arr[i]);
          print_vec(v);
        }
#if 0        
        else
          printf("%d %d \n", arr[left], arr[right]);
#endif        
        break;
      }
    }
  }
}

void test_33_2()
{
  int arr[] = {9,1,2,3,6,6,3,3,7,8,13};
  int len = sizeof(arr)/sizeof(arr[0]);
  vector<int> v;
  find_sum(arr, len, 12, v);
  print_vec(v);
}
int get_index(int *arr, int len, int key)
{
  /*
arr[0..end], find the first number >=key
*/
  int ret=-1, mid=0, start=0, end=len;

  while(start<= end) {
    mid = (start+end)/2;
    if (arr[mid]>= key) {
      ret = mid;
      end = mid-1;
    }
    else {
      start = mid+1;
    }
  }
  return ret;
}
void find_sum2(int *arr, int len, int k, vector<int> &v)
{
  /*
find consec subarray whose sum <=k
sum_arr[len+1]: sum of arr[0..i] index is arr[-1..i-1]
helper_arr[len+1]: adjust sum_arr[], the biggest sum with early index, sorted
iterate arr, index is i.
sum(j) >= sum(i)-k, find the earliest index of j, put arr[j+1..i] in v
*/
  int sum_arr[len+1], helper_arr[len+1];
  int i, j;
  sum_arr[0]=0;
  for (i=1; i<len+1; i++)
    sum_arr[i] = arr[i-1]+sum_arr[i-1];
  helper_arr[0]=0;
  for (i=1; i<len+1; i++)
    helper_arr[i] = (sum_arr[i]>helper_arr[i-1])? sum_arr[i]:helper_arr[i-1];

  for (i=0; i<len; i++) {
    int sum, j, sublen;
    sum = sum_arr[i+1]-k;
    j = get_index(helper_arr, i+1, sum)-1;
    if (j != -2) { //get_index() return -1 when can't find an index.
      sublen = i-j;
      if (sublen > v.size()) {
        v.clear();
        for (int i1=j+1; i1<=i; i1++)
          v.push_back(arr[i1]);
        //print_vec(v);
      }
    }
  }
}

void test_34_2()
{
  int arr[] = {3,-2,-4,0,3,100,6,-2,-4,1,1,2,-1};
  int len = sizeof(arr)/sizeof(arr[0]);
  vector<int> v;
  find_sum2(arr, len, -4, v);
  print_vec(v);
  
}


void find_long_consec(int *arr, int k, int len, unordered_map<int, int> &map, vector<int> &v)
{
  /*
arr[j+1..i] is the consec we want, 
map : key is sum, value is earlest index, or j
iterate arr from 0.
  get sum until i, check if sum-k is in map, 
    if so check if len in bigger than v.size(), then update it
  store sum in map   
*/
  int i,sum = 0, j, sublen;
  for (i=0; i<len; i++) {
    sum+= arr[i];
    if (map.find(sum-k) != map.end()) {
      j = map[sum-k];

      sublen = i-j;
      if (sublen > v.size()) {
        v.clear();
        for (int i1=j+1; i1<=i; i1++)
          v.push_back(arr[i1]);
        //print_vec(v);
      }
    }
    map.insert({sum, i});
  }
}

void test_35()
{
  int arr[] = {9,1,2,3,6,-6,3,2,1,3,3,8,-5,1,1,1};
  int len = sizeof(arr)/sizeof(arr[0]);
  unordered_map<int, int> map;
  vector<int> v;
  map[-1]=0;
  find_long_consec(arr, 12, len, map, v);
  print_vec(v);
}
#if 0
void find_long_path(node *root, int k, vector<int> &v, vector<int> &result, int sum)
{
  /*
find path which starts from root.
sum is sum of all nodes along the path
dfs until leaf, if sum == k, 
  print v;
  v.pop_back(); 
 
*/
  if (!root)
    return;
  v.push_back(root->c);
  sum += root->c;
  if (sum == k) {
    print_vec(v);
    if (v.size() > result.size()) {
      result.clear();
      for (auto ele:v)
        result.push_back(ele);
    
    }
  }
  find_long_path(root->left, k, v, result, sum);
  find_long_path(root->right, k, v, result, sum);
  v.pop_back();
}

void find_path2(node *root, int k, vector<int> &v, vector<int> &result)
{
  /*
preorder traversal each node,
find_long_path() for each node, sum is 0.
*/
  if (!root)
    return;
  find_long_path(root, k, v, result, 0);
  find_path2(root->left, k, v, result);
  find_path2(root->right, k, v, result);
}
#endif
void print_map(unordered_map<int, int> &map)
{
  printf("print_map: ");
  for (auto i :map) {
    printf("key, val %d %d", i.first, i.second);
  } 
  printf("\n");
}
int find_long_path(node *root, int k, int presum, int level, int maxlen, unordered_map<int, int> &map, 
                   vector<node *> &v_node, vector<int> &path)
{
  /*
for each node, put_back in v_node, before return, pop node
when update maxlen, store v_node[j+1..i] to path
*/
  int level_min;
  if (!root)
    return maxlen;
  int cursum = presum+ root->c;
  v_node.push_back(root);
  if (map.find(cursum) == map.end()) {
    map[cursum] = level;
    //printf("insert map %d %d\n", cursum, level);
  }
  auto it= map.find(cursum-k);
  if (map.find(cursum- k) != map.end()) {
    level_min = map[cursum-k];
    if (level - level_min  > maxlen ) {
      maxlen = level - level_min;
      path.clear();
      for (int i= level_min; i<= level-1; i++)
        path.push_back(v_node[i]->c);
      print_vec(path);
    }
  } 
  maxlen = find_long_path(root->left, k ,cursum, level+1, maxlen, map, v_node, path);
  maxlen = find_long_path(root->right, k, cursum, level+1, maxlen, map, v_node, path);
  if (map.find(cursum) != map.end() && level == map[cursum])
    map.erase(cursum);
  
  v_node.pop_back();
  return maxlen;
}

int find_path(node *root, int k) 
{
  unordered_map<int, int> map;
  map[0]=0;
  vector<node *> v_node;
  vector<int> path;

  int ret= find_long_path(root, k, 0, 1, 0, map, v_node, path);
  cout<<ret <<endl;
}


void test_36()
{
  int arr[]= {-3,3,9, 6,0, 2, 1, -1, -1, 2, 3, -2, -1, -1, -1};
  int n = sizeof(arr)/sizeof(arr[0]);
  node *root;
  vector<int> v;
  vector<int> v2;
  
  root = init_tree2(arr, n, 0);
  //tree_preorder(root);
  find_path(root, 6);
  //find_path(root, 10);
}
struct bst {
  int num;
  node *root;
  bst(int node_num=0, node *root=NULL):num(node_num), root(root) {};
};
static int g;
bool find_max_bst(node *&root, int &node_num,  int &biggest, int &smallest, bst &sub)
{
  /*
node_num: root's subtree node num
biggest: root subtree's biggest num
smallest: root subtree's smallest num
sub: root's biggest bst
return: true is root is bst

update :
node_num, left tree's node_num+ right tree's node_num +1
biggest: max(root->c, left tree's biggest, right tree's biggest)
smallest: min(root->c, left tree's smallest, right tree's smallest)


dfs, if root->left and root->right are bst, 
  and root->c > left tree's biggest and 
  root->c < right tree's smallest. 
  root is bst, if  node_num > sub.node_num, update sub to root 
  if root->left is null, root is bst ; if root->right is null, root is bst.
  
recurse finish:
  root is null: node_num 0
*/
  if (!root) {
    node_num = 0;
    return true;
  }
  biggest = smallest = root->c;
  int num1, big1, small1;
  int num2, big2, small2;
  bool is_bst1, is_bst2;
  is_bst1= find_max_bst(root->left, num1, big1, small1, sub);
  is_bst2= find_max_bst(root->right, num2, big2, small2, sub);
  

  if (is_bst1 && is_bst2 && ( (!num1 || root->c > big1) 
                              && (!num2 || root->c < small2)) ) {
    if (num2)
      biggest = big2;
    if (num1)
      smallest = small1;
    node_num = num1+num2+1;
    if (node_num > sub.num) {
      sub.num = node_num;
      sub.root = root;
      printf("%d %d\n", node_num, root->c);
    }
    return true;
  }
  else
    return false;
}

void test_37()
{
  int arr1[]= {0,1,3,6,2,4,5,10,11,14,15,12,16,13,20}; //inorder
  int arr2[]= {6,1,12,0,3,10,13,4,14,16,20,2,5,11,15}; //levelorder
  int n = sizeof(arr1)/sizeof(arr1[0]);
  node *root;
  
  root = construct_tree3(arr1, arr2, n, 1);
  print_preorder(root);
  cout<<endl;
  print_inorder(root);
  cout<<endl;  
  int node_num=0, is_bst=0, biggest= 0, smallest=0;
  bst sub;
  find_max_bst(root, node_num, biggest, smallest, sub);
  //unordered_map<char, int> m;
  //xcout<< sizeof(m) <<"dz" <<endl;
  printf("g %d\n", g);
}
bool is_subtree2(node *root, node *root1)
{
  /* calc if root1 is subtree of root
dfs for root,
for each node, if node->c == root1->c and is_subtree(node->left, root1->left)
  and is_subtree(node->right, root1->right)
if !root1, return true.
*/
  if (!root1)
    return true;
  else if (!root)
    return false;

  return (root->c == root1->c && is_subtree2(root->left, root1->left) && is_subtree2(root->right, root1->right));
  
}
bool is_subtree(node *root, node *root1)
{
  /*dfs*/
  if (is_subtree2(root, root1))
    return true;
  if (!root)
    return false;
  return is_subtree(root->left, root1) || is_subtree(root->right, root1);
  
}

void test_38()
{
  int arr[]= {1,2,3,4,5,6,7,8,9,10,-1,-1,-1,-1,-1};
  int n = sizeof(arr)/sizeof(arr[0]);
  node *root, *root1, *root2;
  vector<int> v;
  vector<int> v2;
  
  root = init_tree2(arr, n, 0);
  int arr1[] = {2,4,5,8,-1,-1,-1};
  n = sizeof(arr1)/sizeof(arr[0]);
  root1 = init_tree2(arr1, n, 0);
  int arr2[] = {2,4,5,9,-1,-1,-1};
  root2 = init_tree2(arr2, n, 0);
  
  int ret;
  ret= is_subtree(root, root1);
  cout<<ret <<endl;
  ret = is_subtree(root, root2);
  cout<<ret <<endl;

  //  tree_preorder(root);
  //find_path(root, 6);

}
int find_first_bigger(int *arr, int len, int root)
{
  int i;
  for (i=0; i<len-1 && arr[i]<root ;i++)
    ;
  return i;
}

bool post_is_bst(int *arr, int len)
{
  /*
find the arr's left and right subtree and judge if they are bst.
if len is 1 or 0, it's bst.
else split arr to 2 parts, note left or right part can be length of 0
how to find pivot?
  the first element that > root is right part.
  all right part should be > root
if left and right both are bst, root is bst
*/
  int len1, len2, index=0, root;
  if (!len || len ==1)
    return true;
  root = arr[len-1];
  index  = find_first_bigger(arr, len, root);
  len1 = index;
  len2 = len-index-1;
  for (int i=index; i<len-1; i++)
    if (arr[i] < root)
      return false;
  return post_is_bst(arr, len1) && post_is_bst(arr+len1, len2);
}

void test_39()
{
  int arr1[] = {3,7,6,14,12,10};
  int n = sizeof(arr1)/sizeof(arr1[0]);
  cout << post_is_bst(arr1, n);
  cout <<endl;
  int arr2[] = {3,11,6,14,12,10};
  n = sizeof(arr2)/sizeof(arr2[0]);
  cout << post_is_bst(arr2, n);
  cout <<endl; 
  int arr3[]= {3,7,6,18,11,12,10};
  n = sizeof(arr3)/sizeof(arr3[0]);
  cout << post_is_bst(arr3, n);
  cout <<endl;
}

void print_vec2(vector<node *> &v)
{
  for (auto n:v)
    cout <<n->c <<" ";
  cout<<endl;
}

bool is_complete(node *root )
{
  /*
v contians nodes, 
for each nodes, if
  node->right exist, but left not, return false
  node->right and node->left both exist, continue;
  else 
    all left nodes should be leaf nodes, otherwise return false.
return true
no need for recursive

generate next level vector
*/
  vector<node *>v , v2;
  int leaf_mode = 0;
  if (root)
    v.push_back(root);
  while (v.size()) {
    
    for (int i=0; i<v.size(); i++) {
       if (leaf_mode) {
        if (v[i]->left || v[i]->right)
          return false;
      }
      else {
        if (v[i]->right && !v[i]->left)
          return false;
        else if (!v[i]->left || !v[i]->right) 
          leaf_mode = 1;
      }
    }
    v2.clear();
    for (auto n: v) {
      if (n->left)
        v2.push_back(n->left);
      if (n->right)
        v2.push_back(n->right);
    }
    v.clear();
    v = v2;
    //print_vec2(v);
  }
  return true;
}
void test_40()
{
  int arr[]= {1,2,3,4,-1,-1,-1};
  int n = sizeof(arr)/sizeof(arr[0]);
  node *root, *root1, *root2;
  vector<int> v;
  vector<int> v2;
  
  root = init_tree2(arr, n, 0);
  cout << is_complete(root) <<endl;

  int arr1[] = {1,2,3,4,5,6,-1,7,8,-1,-1,-1,-1,-1,-1};
  n = sizeof(arr1)/sizeof(arr1[0]);
  root1 = init_tree2(arr1, n, 0);
  //tree_inorder(root1);
  //cout <<endl;
  cout << is_complete(root1)<<endl;

  int arr2[] = {1,2,3,4,-1,5,-1};
  n = sizeof(arr2)/sizeof(arr2[0]);
  root2 = init_tree2(arr2, n, 0);
  cout << is_complete(root2) <<endl;

}
node * tree_find(node *root, int val)
{
  node *n = NULL;
  if (!root)
    return NULL;
  if (root->c == val)
    return root;
  n = tree_find(root->left, val);
  if (!n)
    n = tree_find(root->right, val);
  return n;
}
node * find_next(node *n)
{

}
void test_41()
{
  int arr[]= {6,3,9,1,4,8,10, -1,2,-1,5,7,-1,-1,-1};
  int len = sizeof(arr)/sizeof(arr[0]);
  node *root, *root1, *n;
  vector<int> v;
  vector<int> v2;
  
  
  root = init_tree2(arr, len, 0);
  n = find_next(tree_find(root, 10));
  cout<< n->c <<endl;
  n = find_next(tree_find(root, 6));
  cout <<n->c <<endl;
  n = find_next(tree_find(root, 5));
  cout << n->c <<endl;
}

void print_postorder(int *pre, int *in, int len)
{
  /*
given preorder and inorder, print postorder
split the tree to left, right tree
print left tree, then right tree, then root.
*/
  if (!len)
    return;
  
  int index, len1, len2;
  
  index = node_find(in, pre[0], len);
  len1 = index;
  len2 = len-index-1;
  print_postorder(pre+1, in, len1);
  print_postorder(pre+1+len1, in+index+1, len2);
  printf("%d ", pre[0]);
}
void test_42()
{
  int arr[] = {1,2,4,5,3,7};
  int len = sizeof(arr)/sizeof(arr[0]);
  int arr1[] = {4,2,5,1,3,7};
  vector<int> v;
  print_postorder(arr, arr1, len);
  cout<<endl;
}
int get_hight(node *root)
{
  node *n;
  int ret=1;
  for (n = root; n->left ;n = n->left, ret++)
    ;
  return ret;
}
int get_tree_node_num(node *root, int l, int h)
{
  /* root is a complete tree
first get tree hight, by going to the leftmost node
for a tree, there are 2 conditions:
 a) right is complete
 b) left is complete
use bs(node, l, h)
*/

  if (!root)
    return 0;
  node *n;
  int h1 = l;
  if (root->right)
    h1++;

  for (n=root->right; n && n->left; n=n->left, h1++)
    ;
  //printf("h1, l ,val, %d %d %d\n", h1, l, root->c);

  
  if (h1 == h)  
    return (1<<(h-l))+get_tree_node_num(root->right, l+1, h);
  else
    return (1<<(h-l-1)) +get_tree_node_num(root->left, l+1, h);
}

void test_43()
{
  int arr[] = {1,2,3,4,5,6,7,8,9,10,-1,-1,-1,-1,-1};
  int len = sizeof(arr)/sizeof(arr[0]);
  int arr1[] = {1,2,3,4,5,6,7,8,9,10,11,12,-1,-1,-1};

  node *root, root1;
  int h;


  root = init_tree2(arr, len, 0);
  //tree_preorder(root);
  h =get_hight(root);
  //cout<<h<<endl;
  cout << get_tree_node_num(root, 1, h) <<endl;
  
  len = sizeof(arr1)/sizeof(arr1[0]);
  root = init_tree2(arr1, len, 0);
  h = get_hight(root);
  cout << get_tree_node_num(root, 1, h) << endl;
}


int min_coins(int *arr, int len, int aim, vector<vector<int>> &maps)
{
  if (!len)
    return (aim==0) ? 1:0 ;
  int i;
  int ret=0;
  if (maps[len][aim] == -1) {
    for (i=0; i*arr[0]<=aim; i++)
      ret += min_coins(arr+1, len-1, aim-(i*arr[0]), maps);
    
    maps[len][aim] = ret;
  }
  else
    ret = maps[len][aim];
  
  return ret;
}
void test_44()
{
  int arr[]= {5,10,25,1};
  int len = sizeof(arr)/sizeof(arr[0]);
  int aim=10000;
  vector<vector<int>> maps(len+1);
  for (int i=0;i<len+1; i++) {
    for (int j=0; j<aim+1; j++)
      maps[i].push_back(-1);
  }


  cout << min_coins(arr, len, aim, maps) <<endl;
}

int min_coins3(int *arr, int len, int aim)
{
  int dp[len][aim+1] ;
  for (int i=0; i<len ;i++)
    for (int j=0; j<aim+1; j++)
      dp[i][j] = 0;
  for (int i=0; i<len; i++)
    dp[i][0] = 1;
  for (int j=1; arr[0]*j<=aim; j++)
    dp[0][arr[0]*j] = 1;
  int num = 0;
  for (int i=1; i<len; i++) {
    for (int j=1; j<=aim;j++) {
      num = 0;
      for (int k=0; j-arr[i]*k>=0; k++) 
        num += dp[i-1][j-arr[i] *k];
      
      dp[i][j]= num;
    }
  }
  return dp[len-1][aim];
}
void test_45()
{
  int arr[]= {5,10,25,1};
  int len = sizeof(arr)/sizeof(arr[0]);
  int aim=10000;
  cout << min_coins3(arr, len, aim) <<endl;
}


#if 0
int backtrace(int r, int c, int m, int n, int mat[][6])
{
  if (r==m-1 && c ==n-1)
    return 1;
  if (r>=m || c>=n)
    return 0;
  if (mat[r+1][c]==-1)
    mat[r+1][c]=backtrace(r+1, c,m,n, mat);
  if (mat[r][c+1] == -1)
    mat[r][c+1] =backtrace(r, c+1, m, n, mat);
  return mat[r+1][c] +mat[r][c+1];
}

void test_21()
{
  int m=3, n=5;
  int[][] mat=new int[m+1][n+1];
  //int[][] mat = new int[m+1][n+1];
  for (int i=0;i<m+1; i++)
    for (int j=0; j<n+1; j++)
      mat[i][j]=-1;
  
  cout<< backtrace(0, 0, m, n, mat);
}
#endif
#if 0
string find_longparlin(string &s)
{
/*
  for each item i
*/
}
void test16()
{
  string s("abccba1deed23");
}
#endif
int main()
{
  //test3();
  test_45();
  //sort_words();
}
