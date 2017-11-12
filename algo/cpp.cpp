#include <iostream>
#include <list>
#include <iterator>
#include <vector>
#include <queue>
#include <stack>
#include <limits.h>
#include <algorithm>
#include <string>
#include <string.h>
#include <bitset>
using namespace std;

void test1(void)
{
  vector<int>  g1;
  vector<int>::iterator i;
  vector<int>::reverse_iterator ir;
  
  for (int i=0; i<5; i++)
    g1.push_back(i);

  cout << "begin: ";
  for(i= g1.begin(); i!= g1.end(); i++)
    cout << *i <<'\t';

  cout <<endl << endl;
  cout << "out of rbegin: ";
  for (ir = g1.rbegin(); ir!= g1.rend(); ir++)
    cout << "\t" << *ir;
  cout<< "\n";
}

void test2(void)
{
  vector<int> g1;
  for (int i=0; i<5; i++)
    g1.push_back(i);
  cout<< "size: "<<g1.size() << "\ncap: "<< g1.capacity() << "\nmax" << g1.max_size();
}


void showlist(list<int> g)
{
  list<int>::iterator it;
  for (it= g.begin(); it!= g.end(); it++)
    cout<< " "<< *it;
  cout << "\n";
}

void test3()
{
  list<int> glist1, glist2;
  
  for (int i=0; i<10; i++) {
    glist1.push_back(i*2);
    glist2.push_front(i*3);
  }
  cout << "\n list1 is :" ; showlist(glist1);
  cout << "\n list2 is :" ; showlist(glist2);
  cout << "\n1 front: " << glist1.front();
  cout << "\n1 back: "<< glist1.back();
  cout << "\n1 pop_front";
  glist1.pop_front();
  showlist(glist1);

  cout << "\n 2 pop_back";
  glist2.pop_back();
  showlist(glist2);

  cout << "\n 1 reverse";
  glist1.reverse();
  showlist(glist1);

  cout << "\n 2 sort";
  glist2.sort();
  showlist(glist2);


}

void showq(queue<int>  q)
{
  queue<int> tmp =q;
  while(!tmp.empty()) {
    cout << " "<< tmp.front();
    tmp.pop();
  }
  
  cout <<endl;
}
void test4()
{
  queue <int> q;
  q.push(10);
  q.push(20);
  showq(q);
  cout << q.size() <<", " << q.front() <<", " <<q.back();
  cout << "\n pop:";
  q.pop();
  showq(q);

}

void showstack(stack <int> s)
{
  stack<int>  tmp= s;
  while (!tmp.empty()) {
    cout << ", " <<tmp.top();
    tmp.pop();
  }
  cout <<endl;
}

void test5()
{
  stack<int> s;
  s.push(10);
  s.push(30);
  s.push(20);
  s.push(5);
  s.push(1);

  showstack(s);
  cout << "\n size " <<s.size();
  cout << "\n top" <<s.top();

  cout << "\n pop: ";
  s.pop();
  showstack(s);
}

void showvector(vector<int> v)
{
  for (int i=0; i<v.size(); i++)
    cout << ", " <<v[i];
  cout << endl;
}


void showlist2(list<vector<int> > l)
{
  list<vector<int> >::iterator it;
  for (it= l.begin(); it!= l.end(); it++)
    showvector(*it);
  if (l.size())
    l.pop_back();
}

void test6()
{
  list<vector<int> >  g;
  vector<int> *v;
  v= new vector<int>;
  v->push_back(1); v->push_back(2); v->push_back(3);
  g.push_back(*v);
  
  showlist2(g);
  showlist2(g);

}

struct node_t {
  int val, val2;
  node_t (int val):val(val) {};

  node_t(int a=0, int b=0): val(a), val2(b) {};
};
void showlist2(list<node_t > &l)
{

  for (auto it=l.begin(); it!=l.end(); it++)
    cout << it->val;
  cout<< endl;
}
void flush_list2(list<node_t> &l)
{
  while (!l.empty()) {
    delete &l.front(); 
    l.pop_front();
  }
}

void test7()
{
  list<node_t> l;
  node_t n;
  n.val = 1;
  l.push_back(n);

  n.val=2;
  l.push_back(n);

  n.val =3;
  l.push_back(n);

  showlist2(l);


  node_t n1(), n2(1,2);

}

struct node {
  int c;
  node *left, *right;
  node(int c=0): c(c) {};
  node(int c, vector<int>path): c(c) {this->path = path;};
  
  vector<int> path;
};

node * init_tree4(int *arr, int len, int start)
{
  node *root = NULL;
  int left, right;
     
  if (start < len)  {
    root= new node(arr[start]);
          
    left = start*2+1; 
    right = start*2+2;
    if (left <len && arr[left]!= -1)
      root->left = init_tree4(arr, len ,left);
    if (right<len && arr[right]!= -1 )
      root->right = init_tree4(arr, len, right);
  }
  return root;
}


int max_path_sum(struct node *root, int * max_sum, vector<int> &path)
{
  int ret;
  int l, r, sum;
  int l_bigger = 0;
  if (!root)
    return 0;
     
  l = max_path_sum(root->left, max_sum, path);
  r = max_path_sum(root->right, max_sum, path);
     
  sum = root->c;
  if (l>0)  {
    sum += l;
  }
  if (r>0) {
    sum +=r;
  }
     
  if (l>r && l>0) {
    l_bigger = 1;
    for (int i=0; i<root->left->path.size(); i++)
      root->path.push_back(root->left->path[i]);
  }
  else if (r>=l && r>0) {
    for (int i=0; i<root->right->path.size(); i++)
      root->path.push_back(root->right->path[i]);
  }
          

  if (max(r,l)>0 ) {
    ret = max(r,l)+root->c;
  }
  else  {
    ret = root->c;
  }
  root->path.push_back(root->c);

  if (*max_sum < sum) {
    *max_sum = sum;
    path.clear();
    for (int i=0; i<root->path.size(); i++)
      path.push_back(root->path[i]);
    
    if (l_bigger && r>0 ) { 
      for (int i=0; i<root->right->path.size(); i++)
        path.push_back(root->right->path[i]);
    }
    else if (!l_bigger && l>0) {
      for (int i=0; i<root->left->path.size(); i++)
        path.push_back(root->left->path[i]);
    }
    
  }
  
  return ret;
}

void test_tree5()
{
  struct node *root;
int arr[]= {-100,2,3,4,5,6, 7};
int n = sizeof(arr)/sizeof(arr[0]);
int max_sum = INT_MIN;
vector<int> path(n);

root = init_tree4(arr, n, 0);

max_path_sum(root, &max_sum, path);
cout<< path.size() <<"\n";
for (int i=0; i<path.size(); i++)
  cout<< path[i]<< " ";
cout<<endl;


}

void test8()
{
list<node *> l;
node *n = new node(1);
l.push_back(n);

n= new node(2);
l.push_back(n);

n = new node(3);
l.push_back(n);

for (list<node *>::iterator p = l.begin(); p!= l.end(); ++p) {
cout << (*p)->c <<",";
delete *p;
}
}


void tree_dfs(node *root, vector<int> &path, vector<vector<int> >&all_paths)
{
  if (!root) {
    return ;
  }
  
  path.push_back(root->c);
  if (root->left) {
    tree_dfs(root->left, path, all_paths);
    path.pop_back();
  }
  if (root->right) {
    tree_dfs(root->right, path, all_paths);
    path.pop_back();
  }
  
  if (!root->left && !root->right) {
    all_paths.push_back(path);
  }
}


void test_tree6()
{
  node *root;
  int arr[]= {-100,2,3,4,5,6, 7};
  int n = sizeof(arr)/sizeof(arr[0]);
  int max_sum = INT_MIN;
  vector<int> path;


  vector<vector<int> > all_paths;

  root = init_tree4(arr, n, 0);



  tree_dfs(root, path, all_paths);


  cout << "all_paths size:"<<all_paths.size() <<endl;
  for (int i=0; i<all_paths.size(); i++) {
    vector<int> tmp = all_paths[i];
    for (int j=0; j<tmp.size(); j++)
      cout<<tmp[j]<<" ";
    cout<<endl;
  }


}
void print_vec(vector<int>&v)
{
  for (int i=0; i<v.size(); i++)
    cout << v[i]<< " ";
  cout<<endl;
}
void test_1()
{
  vector<int> path(8); //don't use this.
  print_vec(path);

  node n(1, path);
  print_vec(n.path);
}


void test_2()
{
  vector<int> v{1,2,3};
  print_vec(v);
}


bool binary_search(vector<int> &arr, int val)
{
  int mid, left=0, right= arr.size()-1;
  print_vec(arr);
  while (left<=right) {
    mid = (left+right)/2;
    if (val< arr[mid]) 
      right = mid-1;
    else if (val == arr[mid])
      return true;
    else 
      left = mid+1;
    
  }
  return false;
}

bool binary_search2(vector<vector<int> >&matrix, int val)
{
  int mid, left=0, right= matrix.size()-1;
  bool ret =false;
  while(left <= right) {
    mid =(left+right)/2;
    if (val< matrix[mid][0]) {
      cout << "< "<< matrix[mid][0] <<endl;
      right = mid-1;
    }
    else if (val > matrix[mid][matrix[mid].size()-1]) {
      cout << "> "<< matrix[mid][matrix[mid].size()-1] <<endl;
      left = mid+1;
    }
    else  {
      cout<< "mid "<<mid <<endl;
      ret = binary_search(matrix[mid], val);
      return ret;
    }
  }
  return ret;
}

void test_3()
{
  vector<vector<int> >matrix {{1,3,5,7}, {10,11,16,20}, {23,30,34,50}};
  cout<< binary_search2(matrix, 3) <<endl;
}
#if 0
void subsets2(vector<int> &s, vector<int>&path, int step, vector<vector<int> >&result)
{
  if (step == s.size()) {
    result.push_back(path);
    print_vec(path);
    return;
  }
  subsets2(s, path, step+1, result);
  path.push_back(s[step]);
  subsets2(s, path, step+1, result);
  path.pop_back();
}

vector<vector<int> >subsets(vector<int> &s) 
{
  sort(s.begin(), s.end());
  vector<vector<int> >result;
  vector<int> path;
  subsets2(s, path, 0, result);
  return result;
}
#endif
void subsets2(vector<int>&s, vector<bool>&selected, int step, vector<vector<int> >&result)
{
  if (step == s.size()) {
    vector<int> subset;
    for (int i=0; i<s.size(); i++)
      if (selected[i])
        subset.push_back(s[i]);
    print_vec(subset);
    result.push_back(subset);
    return;
  }
  selected[step] = true;
  subsets2(s, selected, step+1, result);
  selected[step] = false;
  subsets2(s, selected, step+1, result);
}
vector<vector<int> > subsets(vector<int> &s)
{
  sort(s.begin(), s.end());
  vector<vector<int> >result;
  
  vector<bool> selected(s.size(), false);
  subsets2(s, selected, 0, result);
  return result;

}

void test_4()
{
  vector<int> s{1,2,3};
  subsets(s);
}


void permute(vector<int>&s, vector<int>&path, vector<bool> &used)
{
     if (path.size() == s.size()) {
          print_vec(path);
          return;
     }
     for (int i=0; i<s.size(); i++) {
          if (used[i])
               continue;
          used[i] = true;
          path.push_back(s[i]);
          permute(s, path, used );
          path.pop_back();
          used[i]=false;
     }
}
void test_5()
{
  vector<int>s{1,2,3};
  vector<int>path;
  vector<bool>used(s.size(), false);
  permute(s, path, used);
}

bool find_val(vector<int>&arr, int val)
{
  int i;
  for ( i=0; i<arr.size(); i++) 
    if (arr[i] == val)
      break;
  if (i== arr.size())
    return false;
  else
    return true;
}
#if 0
void permute2(vector<int>&s, vector<int>&path, vector<bool> &used, vector<vector<int> >&used_arr)
{
  vector<int> used_ele ;
  int step = path.size();

  if (path.size() == s.size()) {
    print_vec(path);
          return;
     }
  used_ele = used_arr[step];
  for (int i=0; i<s.size(); i++) {
    if (used[i])
             continue;
          
          if (find_val(used_ele, s[i])) 
            continue;
            used_ele.push_back(s[i]);

          used[i] = true;
          path.push_back(s[i]);
          permute2(s, path, used, used_arr );
          path.pop_back();
          used[i]=false;
     }
}

void test_6()
{
  vector<int>s{1,1,1,1};
  vector<int>path;

  vector<bool>used(s.size(), false);
  vector<vector<int> >used_arr;
  
  for (int i=0; i<s.size(); i++) {
    vector<int> tmp;
    used_arr.push_back(tmp);
  }
  permute2(s, path, used, used_arr);

}
#endif

void permute2(vector<int>&s, vector<int>&path, vector<bool> &used, vector<vector<int> >&used_arr)
{
  vector<int> used_ele ;
  int step = path.size();

  if (path.size() == s.size()) {
    print_vec(path);
          return;
     }

  for (int i=0; i<s.size(); i++) {
    if (used[i])
             continue;
          
          if (find_val(used_ele, s[i])) 
            continue;
            used_ele.push_back(s[i]);

          used[i] = true;
          path.push_back(s[i]);
          permute2(s, path, used, used_arr );
          path.pop_back();
          used[i]=false;
     }
}

void test_6()
{
  vector<int>s{1,1,2,2};
  vector<int>path;

  vector<bool>used(s.size(), false);
  vector<vector<int> >used_arr;
  
  for (int i=0; i<s.size(); i++) {
    vector<int> tmp;
    used_arr.push_back(tmp);
  }
  permute2(s, path, used, used_arr);

}
void print_list(list<int> l)
{
  for (auto p=l.begin(); p!=l.end(); p++)
    cout<<*p<<" ";
  cout <<endl;
}
void del_list(list<int>&l)
{
  for (auto p=l.begin(); p!=l.end();) {
    l.erase(p++);

  }
  
}

void test_7()
{
  list<int> l{1,2,3,4};
  //  l.push_back(1);
  del_list(l);
  print_list(l);
}

void test_8()
{
  char s1[] ="aaaeeee";
  char s2[128] = {0};
  strcpy(s2, s1);
  cout <<s1<<endl;
  cout<<s2<<endl;

}
void test_9()
{
  list<int> l{1,2,3,4};
  if (find(l.begin(), l.end(), 5) != l.end())
    cout<<"found"<<endl;
}

/*
if x[i] is even, x[i+1]=x[i]/2;
if x[i] is odd, x[i+1]=3*x[i]+1;
so the sequence is:/
1 4 2 1
2 1
3 10 5 16 8 4 2 1
4 2 1
5 16 8 4 2 1

*/




#define USE_BITS


struct bitmap {
  char *arr;
  int size;
  bitmap(int size=0):size(size) {
    if (size) {
      arr= new char[size/8+1];
    }
    else
      arr= NULL;
  };
  ~bitmap() {
    if (arr)
      delete [] arr;
  };
  void set(int x){
    if (x>size || x<=0)
      return;
    x--;
    arr[x/8] |= (1<<(x%8));
    //cout<<hex<<(unsigned int)(tmp)<<endl;
  };
  bool test(int x) {
    if (x>size || x<=0)
      return false;
    x--;
 
   if (arr[x/8] & (1<<(x%8)))
      return true;
    else
      return false;
  };

};
int gen_arr(long long x, bitmap &bits)
{
  int num=1; 
  long long old_x = x;
  while (x) {
    if (x%2 == 0)
      x= x/2;
    else
      x = 3*x+1;
    //cout<<x<<" ";
#ifdef USE_BITS
    if (x< 5000000  && x>old_x)
      bits.set(x);
#endif
    num++;
    if (x == 1 )
      break;
  }
  //cout <<  " "<< num<<endl;

  return num;
}
int find_max_chain(long long n)
{
  int max_num = 1;
  long long i;
  long long max_ele= -1;
  bitmap bits(n);

  for(i=1; i<=n; i++) {
    if (i%2 != 0)
      continue;
#ifdef USE_BITS
 if (bits.test(i)) {
      max_ele++;
      continue;
    }
#endif
    //if (max_ele >= i)
    // continue;
    max_num = max(max_num, gen_arr(i, bits));
  }
  cout<<max_ele<<endl;
  return max_num;
}
void test_10()
{
  //cout<<gen_arr(113383)<< endl;
  cout<<find_max_chain(5000000) <<endl;
}


void test_11()
{
  bitmap bits(1000);
  bits.set(1);
  cout<< bits.test(1)<<endl;
  bits.set(8);
  cout<< bits.test(8)<<endl;
  
}

int main()
{
  //test7();
  //test_tree5();
  //test8();
  //test_tree6();
  test_10();
  
 return 0;
}
