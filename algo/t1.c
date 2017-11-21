#include "../include/common.h"
#include <limits.h>
struct _queue{
    struct _queue * next;
    int c;
    void * p;
};

int p_1_2()
{
    char str[] = "aaereradfaet123145";
    int i;
    struct _queue *q, *p;
    q=calloc(1, sizeof(*q));
    q->next = NULL;
    for (i=0; i<strlen(str); i++) {
        p = calloc(1, sizeof(*p));
        if (!p)
            exit(-1);
        p->c = str[i];
        p->next = q->next;
        q->next = p;
    }
    for (p=q->next;p; p=p->next) {
        printf("%c", p->c);
    }
    printf("\n");
    return 0;
}

int p_1_3()
{
    char str[] = "aaereradfaet123145";
    int len = strlen(str);
    int i=0, j=0;
    for (i=0; i<len; ) {
        for (j=0; j<i; j++) {
            if (str[j] == str[i])
                break;
        }
        if (i>0 && j != i) {
            if (len-i-1 >0)
                memmove(&str[i], &str[i+1], (len-i-1));
            len--;
        }
        else
            i++;
    }
    str[len] = 0;
    printf("%s\n", str);
}

struct _queue * get_sort(char *str1, int len1)
{
    int i, j;
    struct _queue *q, *p, *n;
    q=calloc(1, sizeof(*q));
    for (i=0; i<len1; i++) {
        int res=0;
        char c;
        if (str1[i] == ' ')
            continue;
        c = tolower(str1[i]);
        n = calloc(1, sizeof(*p));
        n->c=c;
        for (p=q->next; p; p=p->next) {
            if (p->c >= c) {
                res = 1;
                break;
            }
            else if (p->c < c && (p->next && p->next->c >= c)) {
                break;
            }
            else if (p->next==NULL) {
                res =2;
                break;
            }
        }
        if (res == 1 || !p) { //head
            n->next = q->next;
            q->next = n;
        }
        else if (res == 2) {//tail
            p->next = n;
        }
        else { //middle
            n->next = p->next;
            p->next = n;
        }
    }
    return q;
}
void print_queue(struct _queue *q)
{
    struct _queue *p1;
    for (p1=q->next; p1; p1=p1->next) {
        printf("%c", p1->c);
    }
    printf("\n");
}

int queue_equal(struct _queue *q1, struct _queue *q2)
{
    struct _queue *p1, *p2;
    for (p1=q1->next, p2=q2->next; p1&&p2;
         p1=p1->next, p2=p2->next) {
        if (p1->c != p2->c)
            break;
    }
    return (!p1 && !p2);

}
int p_1_4(char * str1, char * str2)
{
    int i=0;
    struct _queue *q1, *q2, *p1, *p2;
    q1=get_sort(str1, strlen(str1));
    print_queue(q1);
    q2=get_sort(str2, strlen(str2));
    print_queue(q2);

    printf("%d\n", queue_equal(q1, q2));
    
}

int tail_queue()
{
    char str[] = "aaereradfaet123145";
    int i;
    struct _queue *q, *p, *tail;
    q=calloc(1, sizeof(*q));
    q->next = NULL;
    tail=calloc(1, sizeof(*tail));
    tail->next = NULL;
    for (i=0; i<strlen(str); i++) {
        p = calloc(1, sizeof(*p));
        if (!p)
            exit(-1);
        p->c = str[i];
        p->next = NULL;
        tail->next = p;
        tail = p;
        if (!i) 
            q->next = p;

    }
    for (p=q->next;p; p=p->next) {
        printf("%c", p->c);
    }
    printf("\n");
    return 0;
}

void p_1_5(char * str)
{
    int i, pos=0;
    char * buf;
    if (!str)
        return;
    buf = calloc(1, 3*strlen(str)+1);
    for (i=0; i<strlen(str); i++) {
        if (str[i] != ' ') {
            buf[pos++] = str[i];
        }
        else {
            memcpy(buf+pos, "%20", 3);
            pos += 3;
        }
    }
    printf("%s \n", buf);
}
#define M 5
#define N 6
struct _queue2{
    struct _queue2 * next;
    int i,j;
};

void p_1_7(int matrix[M][N])
{
    int i=0,j=0;
    struct _queue2 *p, *q, *tmp;
    q=calloc(1, sizeof(*q));
    
    for (i=0; i<M; i++) {
        for (j=0; j<N; j++) {
            if (!matrix[i][j]) {
                p = calloc(1, sizeof(*p));
                if (!p)
                    return;
                p->i=i; p->j=j;

                p->next = q->next;
                q->next = p;
            }
        }
    }
    for(p=q->next; p; p=p->next) {
        for (j=0;j<N; j++)
            matrix[p->i][j] = 0;
        for (i=0; i<M; i++)
            matrix[i][p->j] = 0;
    }
    for (i=0; i<M; i++) {
        for (j=0; j<N; j++)
            printf("%d ", matrix[i][j]);
        printf("\n");
    }
    for (p=q->next; p; p=tmp) {
        tmp = p->next;
        free(p);
    }
    free(q);
}

int p_1_8(char *str1, char *str2)
{
    int i, len;
    char * buf;
    if ( (len=strlen(str1)) != strlen(str2))
        return 0;
    buf = calloc(1, len);
    if (!buf)
        return 0;
    for (i=0; i<len-1; i++) {
        memcpy(buf, str1+i+1, len-i-1);
        memcpy(buf+len-i-1, str1, i+1);
        if (!strcmp(buf, str2))
            break;
    }
    return (i != len-1);
}

int p_2_2(struct _queue *q, int n)
{
    struct _queue *p1, *p2;
    int i=0;
    for (p1=q->next; p1; p1=p1->next) {
        if (i++ == n+1)
            break;
    }
    if (!p1)
        return -1;
    for (p2=q->next; p2&&p1; p1=p1->next, p2=p2->next)
        ;
    if (!p2)
        return -1;
    return p2->c;
}

void p_2_3(struct _queue *q, int c)
{
    struct _queue *p, *tmp;
    int i;
    for (p=q->next ;p&&p->next; p=p->next) {
        if (p->next->c == c) {
            p->next = p->next->next;
        }
    }

    for (p=q->next ;p; p=p->next)
        printf("%c", p->c);
    printf("\n");
    
}

void create_queue_from_str(struct _queue **pq, char *str)
{
    struct _queue *n, *p;
    int i;

    *pq = calloc(1, sizeof(struct _queue));

    for (i=0; i<strlen(str); i++) {
        n = calloc(1, sizeof(*n));
        n->c = str[i];

        for (p=(*pq)->next; p &&p->next; p=p->next)
            ;
        if (!p) { //head
            (*pq)->next = n;
        }
        else {
            p->next = n;
        }
    }
    for (p=(*pq)->next; p ; p=p->next)
        printf("%c", p->c);
    printf("\n");

}

void create_queue_from_str_reverse(struct _queue **pq, char *str)
{
    struct _queue *n, *p, *q;
    int i;

    *pq = calloc(1, sizeof(struct _queue));
    q = *pq;
    for (i=0; i<strlen(str); i++) {
        n = calloc(1, sizeof(*n));
        n->c = str[i];

        n->next = q->next;
        q->next = n;
    }
    for (p=(*pq)->next; p ; p=p->next)
        printf("%c", p->c);
    printf("\n");

}
int _pow(int base, int n)
{
    int i, r=1;
    for (i=0; i<n; i++)
        r=r*base;
    return r;
}
int _to_num(struct _queue *q)
{
    int i=0, j=0;
    struct _queue *p;
    for (p=q->next; p; p=p->next) {
        i += (p->c - '0') * (_pow(10, j++));
    }
    return i;
}

void p_2_4(struct _queue *q1, struct _queue *q2)
{
    int i1, i2;
    int n = _to_num(q1)+_to_num(q2);
    printf("n %d %d :%d\n", _to_num(q1), _to_num(q2), n);
    struct _queue *q;
    char buf[128]= "";
    snprintf(buf, sizeof(buf), "%d", n);
    create_queue_from_str_reverse(&q, buf);
}
struct _queue * find_node(struct _queue *q, int c)
{
    struct _queue *p;
    for (p=q->next; p; p=p->next) {
        if (p->c == c)
            break;
    }
    return p;
}

void p_2_5(struct _queue *q)
{
    struct _queue *p1, *p2;
    for (p1=q->next, p2=q->next;
         p1&&p2;  p1=p1->next) {

        //omit the first one
        if (p1 != q->next) {
            if (p1 == p2)
                break;
        }
        
        p2 = p2->next;
        if (!p2)
            break;
        else
            p2 = p2->next;
    }
    if (!p1 || !p2) {
        printf("list is not corrupt\n");
        return;
    }

    for (p1=q->next; p1&&p2 && (p1!=p2);
         p1=p1->next, p2=p2->next)
        ;
    printf("loop point is %c\n", p1->c);
}

struct my_queue {
    struct _queue *head, *tail;
};
void print_queue4(struct my_queue *q)
{
    struct _queue *p1;
    for (p1=q->head; p1; p1=p1->next) {
        printf("%c", p1->c);
    }
    printf("\n");
}

int enqueue(struct my_queue *q, int c)
{
    struct _queue *p;

    p = calloc(1, sizeof(*p));
    if (!p)
        return -1;
    p->c = c;
    if (!q->head) { //first
        q->head = calloc(1, sizeof(struct _queue));
        q->head->next = p;
        q->tail = p;
    }
    else {
        q->tail->next = p;
        q->tail = p;
    }
    return 0;
}
int enqueue2(struct my_queue *q, void * d)
{
    struct _queue *p;

    p = calloc(1, sizeof(*p));
    if (!p)
        return -1;
    p->p = d;
    if (!q->head) { //first
        q->head = calloc(1, sizeof(struct _queue));
        q->head->next = p;
        q->tail = p;
    }
    else {
        q->tail->next = p;
        q->tail = p;
    }
    return 0;
}
int enqueue3(struct my_queue *q, int c)
{
    struct _queue *p;

    p = calloc(1, sizeof(*p));
    if (!p)
        return -1;
    p->c = c;
    if (!q->head) { //first
        q->head = p;
        q->tail = p;
    }
    else {
        q->tail->next = p;
        q->tail = p;
    }
    return 0;
}

int dequeue(struct my_queue *q)
{
    struct _queue *p;
    if (!q->head)
        return -1;
    p = q->head->next;
    q->head->next = p->next;
    
    return p->c;
}
struct my_stack {
    int array[128];
    int size;
    int top;
    char name[32];
};
int stack_init(struct my_stack *s)
{
    memset(s, 0, sizeof(*s));
    s->size = 128;
}
    
int stack_push(struct my_stack *s, int i)
{
    if (s->top== s->size-1)
        return -1;
    s->array[s->top++]=i;
    return 0;
}
int stack_pop(struct my_stack *s)
{
    if (!s->top)
        return -1;
    return s->array[--s->top];
}
int stack_empty(struct my_stack *s)
{
    return !(s->top);
}
int stack_peek(struct my_stack *s)
{
    if (!s->top)
        return -1;
    else
        return s->array[s->top-1];
}

void print_stack(struct my_stack *s)
{
    int i;
    printf("name: %s, ", s->name);
    for (i=0;i<s->top;i++)
        printf("%d ", s->array[i]);
    printf("\n");
}
void hanoi_move(int n, struct my_stack *s_src, struct my_stack *s_targ, struct my_stack *s_spare )
{
    if (n>0) {
        hanoi_move(n-1, s_src, s_spare, s_targ);
        stack_push(s_targ, stack_pop(s_src));
        printf("##### n:%d \n", n);
        print_stack(s_src);
        print_stack(s_spare);
        print_stack(s_targ);
        printf("oper from %s to %s, %d\n", s_src->name, s_targ->name, s_targ->array[s_targ->top-1]);
        
        hanoi_move(n-1, s_spare, s_targ, s_src);
    }
}
// see  https://en.wikipedia.org/wiki/Tower_of_Hanoi for the algorithm
void p_3_4()
{
    struct my_stack s_src, s_spare, s_targ;
    stack_init(&s_src);
    strcpy(s_src.name, "a");
    stack_init(&s_spare);
    strcpy(s_spare.name, "b");
    stack_init(&s_targ);
    strcpy(s_targ.name, "c");
    stack_push(&s_src, 5);
    stack_push(&s_src, 4);
    stack_push(&s_src, 3);
    stack_push(&s_src, 2);
    stack_push(&s_src, 1);
    hanoi_move(5, &s_src, &s_targ, &s_spare);
}

struct my_stack p_3_6(struct my_stack *s)
{
    struct my_stack r;
    int i;
    stack_init(&r);

    while (!stack_empty(s)) {
        i = stack_pop(s);
        while (!stack_empty(&r) && stack_peek(&r)>i) {
            stack_push(s, stack_pop(&r));
        }
        stack_push(&r, i);
    }
    return r;
}

void sort_list(struct _queue *q)
{
    struct _queue *r, *p1, *p2, *tmp, *tmp2;
    int ret;
    r = calloc(1, sizeof(*r));


    for (p1=q->next; p1; p1=tmp2) {
        tmp = NULL;
        for (p2=r->next; p2; p2=p2->next) {
            if (p2->c < p1->c)
                tmp = p2;
            else
                break;
        }
        tmp2 = p1->next;
        if (!tmp) { //head
            p1->next = r->next;
            r->next = p1;
        } else {
            p1->next = tmp->next;
            tmp->next = p1;
        }
    }
    print_queue(r);
}

struct _node {
    struct _node *left, *right, *parent;
    int c;
};
void tree_print1(struct _node *root)
{
    if (!root) {
        printf("null\n");
        return;
    }
    printf("%d\n", root->c);
    tree_print1(root->left);
    tree_print1(root->right);
}
void print_queue2(struct _queue *q)
{
    struct _queue *p;
    for (p=q; p; p=p->next) {
        if (!p->p)
            printf("null\n");
        else
            printf("%d\n", ((struct _node*)p->p)->c);
    }

}
void print_queue3(struct _queue *q)
{
    struct _queue *p;
    for (p=q->next; p; p=p->next) {
        if (!p->p)
            printf("null\n");
        else
            printf("%d\n", ((struct _node*)p->p)->c);
    }

}

void tree_print2(struct _node *root)
{
    struct _queue *p, *head = NULL, *head1, *p1;
    p = calloc(1, sizeof(*p));
    p->p = root;
    head = p;

    while (head) {
        print_queue2(head);
        printf("#######\n");
        head1 = NULL;
        for (p=head; p; p=p->next) {
            struct _node *n = p->p;
            if (!n)
                break;
            if (!head1) {
                p1 = calloc(1, sizeof(*p));
                p1->p = n->left;
                head1 = p1;

                p1 = calloc(1, sizeof(*p));
                p1->p = n->right;
                p1->next = head1;
                head1 = p1;


            }
            else {
                p1 = calloc(1, sizeof(*p));
                p1->p = n->left;
                p1->next = head1;
                head1 = p1;

                p1 = calloc(1, sizeof(*p));
                p1->p = n->right;
                p1->next = head1;
                head1 = p1;
            }
        }
        //print_queue2(head1);
        head = head1;
    }

}

void tree_print3(struct _node *root)
{
    struct _queue *p,  *p1;
    struct my_queue q, q1;
    memset(&q, 0, sizeof(q));
    memset(&q1, 0, sizeof(q1));
    enqueue2(&q, root);

    while (q.head->next) {
        print_queue3(q.head);
        printf("#######\n");
        q1.head = NULL;
            
        for (p=q.head->next; p; p=p->next) {
            struct _node *n = (struct _node *)p->p;
            if (!n)
                break;
            enqueue2(&q1, n->left);
            enqueue2(&q1, n->right);
        }
        //print_queue2(head1);
        if (q1.head)
            q.head->next = q1.head->next;
        else
            q.head->next = NULL;
    }
}

//if arr[i]==-1, means node is null.
void tree_build(struct _node **root, int * arr, int len)
{
    struct _node *n, *parent;
    struct _node **n_arr;
    int i, left;
    
    n_arr = calloc(1, len*sizeof(struct _node *));
    for (i=0; i<len; i++) {
        if (arr[i] == -1) {
            n = NULL;
        }
        else  {
            n = calloc(1, sizeof(*n));
            n->c = arr[i];
        }
        n_arr[i] = n;
        if (!i)
            *root = n;
        else {
            parent = n_arr[(i+1)/2-1];
            left = (i+1)%2 ? 0:1;
            if (left) {
                parent->left = n;
                n->parent = parent;
            }
            else {
                parent->right = n;
                n->parent = parent;
            }
        }
    }
}
struct _node * tree_find(struct _node *root, int c)
{
    struct _node *n;
    if (!root)
        return NULL;
    if (n = tree_find(root->left, c))
        return n;
    if (root->c == c)
        return root;
    if (n = tree_find(root->right, c))
        return n;
}
void p_4_6(struct _node *root, struct _node *n1, struct _node *n2)
{
    struct _node *p1, *p2;

    for (p1=n1->parent; p1; p1=p1->parent) {
        for (p2=n2->parent; p2; p2=p2->parent) {
            if (p1 == p2)
                goto out;
        }
    }
out:    
    if (p1)
        printf("parent is %d\n", p1->c);
    else
        printf("can't find parent\n");
}

int ** p_16_10(int rows, int columns)
{
    int **ret;
    int size ;
    int **p;
    int i;
    size = sizeof(int *)*rows+ rows*columns*sizeof(int);
    ret = calloc(1, size);
    p = ret;
    for (i=0; i<rows; i++) {
        p[i]= (int *)(ret +rows)+i*columns;
    }
    return ret;
    
}

int p_19_7(int * arr, int len)
{
    int max=0;
    int i, j,k, sum;
    for (i=1; i<=len; i++) {

        for (j=0; j<len-i; j++) {
            sum=0;
            for (k=j; k<j+i; k++)
                sum+= arr[k];
            if (max < sum)
                max = sum;
        }

    }
    return max;
}

void p_503(int *arr, int len)
{
    int i, j, k;
    int * ret;
    ret = calloc(1, sizeof(int)*len);
    for (i=0;i<len; i++) {
        int r = -1;
        for (j=i+1; j!=i ; ) {
            if (arr[j] > arr[i]) {
                r = arr[j];
                break;
            }
            if (j+1 >=len)
                j=0;
            else
                j=j+1;
        }
        ret[i] = r;
    }
    for (i=0;i<len;i++)
        printf("%d ", ret[i]);
    printf("\n");
}
struct _queue * find_queue(struct my_queue *q, int c)
{
    struct _queue *p;
    for (p=q->head; p ; p=p->next) {
        if (p->c == c)
            return p;
    }
    return NULL;
}

void remove_queue(struct my_queue *q, struct _queue *n)
{
    struct _queue *p;
    int update_tail = 0;
    if (!n)
        return;
    if (n == q->tail)
        update_tail = 1;
    
    if (n == q->head) {
        q->head = n->next;
    }
    else {
        for (p=q->head; p; p=p->next) {
            if (p->next == n)
                break;
        }
        p->next = n->next;
    }

    if (update_tail) {
        for (p=q->head; p; p=p->next) {
            if (!p->next)
                break;
        }
        q->tail = p;
        
    }
}
void swap2(int *a, int *b)
{
	*a = *a^*b;
	*b = *a^*b;
	*a = *a^*b;
}
void print_arr(int *arr, int len)
{
	int i;
	for (i=0; i<len; i++)
		printf("%d ", arr[i]);
	printf("\n");
}
void get_paren_arr(int *arr, int left, int right)
{
	if (left == 0 && right == 0)
		print_arr(arr, 8);

	if (left == right) {
		arr[8-left-right] = '(';
		get_paren_arr(arr, left-1, right);
	}
	else if (left < right) {
		arr[8-left-right] = '(';
		if (left >=1)
		get_paren_arr(arr, left-1, right);
		arr[8-left-right] = ')';
		if (right >=1)
		get_paren_arr(arr, left, right-1);
	}
	else
		printf("wrong\n");
}

void test_1()
{
	int arr[8] = {0};
	get_paren_arr(arr, 4, 4);
}

void sort_2_arr(int *a, int *b, int lena, int lenb)
{
	int index = lena+lenb-1, i;
	int ia =lena-1, ib=lenb-1;
	while(ia>=0 && ib>=0) {
		a[index--] = a[ia]>b[ib] ? a[ia--]:b[ib--];
	}
	while(ib>=0)
		a[index--] =b[ib--];
}
void test_2()
{
	int a[20] ={1,2,7,8,9};
	int b[]= {3,4,5,6};
	sort_2_arr(a,b, 5,4);
	print_arr(a, 9);
}

int my_sqrt(int x)
{
	int left = 1, right= x/2;
	int last_mid;

	if (x<2) return x;
	while(left <=right) {
		int mid = (left+right)/2;
		if (x/mid >mid ) {
			left = mid+1;
			last_mid =mid;
		}else if (x/mid < mid) {
			right =mid-1;
		}
		else
			return mid;
	}
	return last_mid;
}


void test_3()
{
    printf("%d %d %d %d\n", my_sqrt(100), my_sqrt(81), my_sqrt(64), my_sqrt(1024));
}

int mypower(int x, int n)
{
    if (!n)
        return 1;
    int v = mypower(x, n/2);
    if (n%2 == 0)
        return v*v;
    else
        return v*v*x;
}
void test_4()
{
    printf("%d %d %d %d\n", mypower(2,10), mypower(3,5), mypower(4,6 ), mypower(7,8));
}

struct node2 {
    int c;
    struct node2 *next;
};

struct node2* init_list(int *arr, int len)
{
    int i;
    struct node2 *n, *ret, *n1;
    for (i=0; i<len ;i++) {
        if (!i) {
            ret = calloc(1, sizeof(*n));
            ret->c = arr[0];
            n = ret;
        }
        else {
            n1 = calloc(1, sizeof(*n1));
            n1->c = arr[i];
            n->next = n1;
            n = n1;
        }
    }
    return ret;
}
struct node2 * find_pos(struct node2 *l, int c)
{
    struct node2 *p;
    for (p= l; p; p=p->next) {
        if (!p->next)
            break;
        if (p->c <= c && p->next->c > c)
            break;
    }
    return p;
}
struct node2* insert_sort(struct node2 *l)
{
    struct node2 tmp;
    struct node2 *p, *p2, *p1;
    memset(&tmp, 0, sizeof(tmp));
    tmp.c = INT_MIN;
    p = &tmp;

    for (p=l; p; ) {
        p2 = p->next;
        p1 = find_pos(&tmp, p->c);
        p->next = p1->next;
        p1->next = p;
        p = p2;
    }
    return tmp.next;
}

void print_list(struct node2 *l)
{
    struct node2 *p = l;
    for (p=l; p; p=p->next)
        printf("%d ", p->c);
    printf("\n");
}
void test_5()
{
    int arr[] = {2,1,3,5,4,6};
    struct node2 *l;
    int n = sizeof(arr)/sizeof(arr[0]);
    l = init_list(arr, n);
    print_list(l);
    print_list(insert_sort(l));
}

void list_push(struct node2 *head, struct node2 *n)
{
    struct node2 *p = head;
    for (p=head; p && p->next; p=p->next)
        ;
    p->next = n;
    n->next = NULL;
}
void test_6()
{
    int arr[]= {1,2,3};
    struct node2 *l, *n;
    l = init_list(arr, sizeof(arr)/sizeof(arr[0]));
    print_list(l);
    n = calloc(1, sizeof(*n));
    n->c = 4;
    list_push(l, n);
    print_list(l);
}

int reverse_int(int n)
{
    int tmp, ret=0;
    while(n) {
        tmp = n%10;
        ret = ret*10+tmp;
        n = (n-tmp)/10;
    }
    return ret;
}

void test_7()
{
    printf("%d\n", reverse_int(321));
}

struct node2 * reverse_list(struct node2 *l)
{
    struct node2 *p, l2, *p2;
    l2.next =NULL;
    for (p=l; p; p=p2) {
        p2 = p->next;
        p->next = l2.next;
        l2.next = p;
    }
    return l2.next;
}
void test_9()
{
    struct node2 *l, *l2;
    int arr[]= {1,2,3};
    l = init_list(arr, sizeof(arr)/sizeof(arr[0]));
    l2 = reverse_list(l);
    print_list(l2);
}

struct node2* remove_dup(struct node2 *l)
{
    struct node2 *n, *prev=NULL;
    struct node2 head, *p;
    int push=0;
    p = &head;
    for (n=l; n; ) {
        if (!prev) {//head
            if (!n->next || n->next->c != n->c) {
                push =1;
            }
        }
        else if (!n->next) {
            if (prev->c != n->c) {
                push =1;
            }
        }
        else {
            if (prev->c != n->c && n->c != n->next->c) {
                push = 1;
            }
        }

        if (push) {
            //printf("%d\n", n->c);
            p->next = n;
            p =n;
            printf("%d\n", p->c);
        }
        push =0;
        prev =n;
        n= n->next;

    }
    p->next = NULL;
    //printf("%d\n", p->c);
    return head.next;
}



void test_8()
{
    int arr[]={1,2,2,3,4,4,1,5,5};
    struct node2 *l, *l2;
    l = init_list(arr, sizeof(arr)/sizeof(arr[0]));
    l2 = remove_dup(l);
    print_list(l2);
}
struct node2 * new_node(int c)
{
    struct node2 *n;
    n=calloc(1, sizeof(*n));
    if (n)
        n->c = c;
    return n;
}
struct node2 * add_list(struct node2 *l1, struct node2 *l2)
{
/*
for each node in list, add number and generate a new node.
   if (inc)
      need add one more;
   if (num >=10)
      num = num%10;
      set inc=1;
   else
     set inc=0
   iterate until both lists are empty.
   create a node, append to head
*/
    struct node2 *p1, *p2, *n, l, *p;
    int num=0, inc=0;
    memset(&l, 0, sizeof(l));
    p= &l;
    for(p1=l1, p2=l2; p1 ||p2; ) {
        num =0;
        if (p1)
            num += p1->c;
        if (p2)
            num += p2->c;
        if (inc)
            num++;
        if (num >=10) {
            num -= 10;
            inc = 1;
        }
        else
            inc = 0;
        n = new_node(num);
        p->next = n;
        p = p->next;
        if (p1)
            p1 = p1->next;
        if (p2)
            p2 = p2->next;
        //
    }
    return l.next;
}

void test_10()
{
    int arr1[]= {2,4,3};
    int arr2[]= {5,6,9,1};
    struct node2 *l1, *l2, *l3;
    l1= init_list(arr1, sizeof(arr1)/sizeof(int));
    l2= init_list(arr2, sizeof(arr2)/sizeof(int));
    l3 = add_list(l1,l2);
    print_list(l3);
}


int main()
{
#if 0    
    p_1_4("The earthquakes","The queer shakes");
    p_1_4("Astronomer","Moon starre");

    tail_queue();

    p_1_5("The earth quake e");
#endif
#if 0
    int matrix[M][N] = { {1, 0, 2, 3,4 ,5},
                         {2,2,0, 2,2,2},
                         {3,3,3,3,3,3},
                         {4,4,4,4,4,4},
                         {5,5,5,5,5,0}};
    p_1_7(matrix);
#endif
#if 0    
    printf("%d\n", p_1_8("waterbottle", "erbottlewat"));
    printf("%d\n", p_1_8("waterbottle", "erbottlewae"));
#endif

#if 0    
    struct _queue *q;
    create_queue_from_str(&q, "aerqerqa1324123$r");
    
    printf("%c\n", p_2_2(q, 2));
    printf("%c\n", p_2_2(q, 3));
#endif    

#if 0    
    struct _queue *q;
    create_queue_from_str(&q, "aerqerqa1324123$r");
    p_2_3(q, 'r');
#endif    

#if 0    
    struct _queue *q1, *q2;
    create_queue_from_str_reverse(&q1, "123");
    create_queue_from_str_reverse(&q2, "345");
    p_2_4(q1, q2);
#endif

#if 0    
    struct _queue *q, *p1, *p2;
    //create corrupt link list
    create_queue_from_str(&q, "1234567890");
    p_2_5(q);
    p1 = find_node(q, '0');
    p2 = find_node(q, '5');
    p1->next = p2;
    p_2_5(q);
#endif

#if 0    
    struct my_queue q;
    memset(&q, 0, sizeof(q));
    enqueue(&q, '1');
    enqueue(&q, '2');
    enqueue(&q, '3');
    print_queue(q.head);
    printf("%c ", dequeue(&q));
    printf("%c ", dequeue(&q));
    printf("%c ", dequeue(&q));
    printf("\n");
#endif
#if 0    
    struct my_stack s;
    stack_init(&s);
    stack_push(&s, 1);
    stack_push(&s, 2);
    stack_push(&s, 3);
    printf("%d ", stack_pop(&s));
    printf("%d ", stack_pop(&s));
    printf("%d ", stack_pop(&s));
    printf("\n");
#endif

#if 0    
    p_3_4();
#endif

#if 0    
    struct my_stack s, r;
    stack_init(&s);
    stack_push(&s, 1);
    stack_push(&s, 5);
    stack_push(&s, 4);
    stack_push(&s, 3);
    stack_push(&s, 2);
    r = p_3_6(&s);
    print_stack(&r);
#endif

#if 0
    struct _queue *q;
    //create corrupt link list
    create_queue_from_str(&q, "1789236");
    sort_list(q);
#endif

#if 0    
    int arr[]= {1,2,3,4,5,6,7,8};
    struct _node *root;
    tree_build(&root, arr, 8);
    //tree_print1(root);
    //tree_print2(root);
    tree_print3(root);
#endif
#if 0    
    int arr[]= {1,2,3,4,5,6,7,8};
    struct _node *root, *n1, *n2;
    tree_build(&root, arr, 8);
    //tree_print1(root);
    //tree_print2(root);
    n1=tree_find(root, 5);
    n2=tree_find(root, 8);
    p_4_6(root, n1, n2);
#endif
#if 0    
    int arr[]= {1,2,3};
    printf("%p %p\n",  arr, &arr[-1]);
#endif
#if 0    
    int ** arr =NULL;
    arr = p_16_10(4,5);
#endif
#if 0    
    int arr[] = {2, -8,3,-2,4, -10};
    printf(" %d\n",p_19_7(arr, 6));
#endif
#if 0    
    int arr[] = {1 ,2, 3, 1};
    p_503(arr, 4);
#endif

#if 0    
    struct my_queue q;
    struct _queue *n;
    memset(&q, 0, sizeof(q));
    
    char str[] = "123";
    int i;

    for (i=0; i<strlen(str); i++)
        enqueue3(&q, str[i]);
    print_queue4(&q);
    n = find_queue(&q, '2');
    remove_queue(&q, n);
        print_queue4(&q);
    n = find_queue(&q, '3');
    remove_queue(&q, n);
        print_queue4(&q);
    n = find_queue(&q, '1');
    remove_queue(&q, n);
    print_queue4(&q);
#endif
#if 0	
	int i=20, j=30;
	swap2(&i, &j);
	printf("%d %d\n", i,j);
#endif
	//test_2();
	//test_3();
        //test_4();
        test_10();
}
