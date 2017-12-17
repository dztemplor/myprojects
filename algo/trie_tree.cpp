#include <iostream>
#include <vector>
#include <list>
#include <stack>
#include <algorithm>
#include <unordered_map>
#include <map>
#include <string.h>
#include <sstream>


#include <string>

using namespace std;


const int ALPHABET_SIZE = 128;


// trie node
struct TrieNode
{
    struct TrieNode *children[ALPHABET_SIZE];

    // isEndOfWord is true if the node represents
    // end of a word
    bool isEndOfWord;
  int num;
  static int count;
  TrieNode():num(0) {
    isEndOfWord = false;
    for (int i = 0; i < ALPHABET_SIZE; i++)
      children[i] = NULL;
  };
  void insert(string key) {
    struct TrieNode *pCrawl = this;
        
    for (int i = 0; i < key.length(); i++)
    {
        int index = key[i] ;
        if (!pCrawl->children[index])
          pCrawl->children[index] = new TrieNode();
 
        pCrawl = pCrawl->children[index];
    }
    // mark last node as leaf
    pCrawl->isEndOfWord = true;
    if (!pCrawl->num++ )
      TrieNode::count++;
  };
  bool search(string key) {
    struct TrieNode *pCrawl = this;

    for (int i = 0; i < key.length(); i++)
    {
        int index = key[i];
        if (!pCrawl->children[index])
            return false;

        pCrawl = pCrawl->children[index];
    }

    return (pCrawl != NULL && pCrawl->isEndOfWord);
    
  };
  void save_num(vector<int> &v) {
    /*print tree's leaf's num*/
    
    if (isEndOfWord) {
      v.push_back(num);
      return;
    }
    for (int i = 0; i < ALPHABET_SIZE; i++) {
      if (children[i])
        children[i]->save_num(v);
    }
  }
};
int TrieNode::count= 0;
void test_1()
{
   string keys[] = {"the", "a", "there",
                    "answer", "any", "by",
                     "bye", "their" };
    int n = sizeof(keys)/sizeof(keys[0]);

    struct TrieNode *root = new TrieNode();

    // Construct trie
    for (int i = 0; i < n; i++)
      root->insert(keys[i]);

    // Search for different keys
    root->search("the")? cout << "Yes\n" :
                         cout << "No\n";
    root->search("these")? cout << "Yes\n" :
                           cout << "No\n";
    
}

void sort_words()
{
  /*
use set, when add a new word, c
*/
  char *fn= "controls.xml";
  FILE *fp = fopen(fn,"r");
  char buf[1024] = {0};
  TrieNode *root = new TrieNode();
  if (!fp)
    return;
  int num=0;
  while (fgets(buf, sizeof(buf)-1, fp)) {
    char *str;
    for (str = strtok(buf, " \n\t"); str; str=strtok(NULL, " \n\t")) {
        root->insert(string(str));
    }
    memset(buf, 0, sizeof(buf));
  }
  cout<< TrieNode::count <<" "<<root->isEndOfWord <<endl;
  vector<int> v;
  root->save_num(v);
  sort(v.begin(), v.end(), greater<int>());
#if 1
  for (int i=0; i<10; i++)
    printf("%d ", v[i]);
  printf("\n");
#endif
  //printf("%lu num %d\n", mm.size(), num);
}

void test_2()
{
  //for (int i=0;i <10; i++)
    sort_words();
}

int main()
{
  test_2();
  return 0;
}
