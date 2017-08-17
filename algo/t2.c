#include "../include/common.h"
#include "queue.h"
void p_6_1()
{
    int arra[10]= {2,3,4,5,6,10};
    int arrb[]= {1,3,7,8};

    int len_a = 6, len_b=4;
    int ia,ib, ic;

    for (ia=len_a-1, ib=len_b-1, ic=len_a+len_b-1;
         ia>=0 && ib>=0; ic--) {
        if (arrb[ib]>arra[ia]) {
            arra[ic] = arrb[ib];
            ib--;
        }
        else {
            arra[ic] = arra[ia];
            ia--;
        }
    }
    if (ib>=0) {
        for (ia=0; ia<=ib; ia++)
            arra[ia] = arrb[ia];
    }
    ;
}
struct entry {
    TAILQ_ENTRY(entry) entries;         /* Tail queue. */
    void *d;
};

TAILQ_HEAD(tailhead, entry) ;

void create_tailq(struct tailhead *l, int *arr, int len)
{
    int i;
    struct entry *n;
    TAILQ_INIT(l);
    for (i=0; i<len; i++) {
        n =calloc(1, sizeof(*n));
        n->d= (void *)arr[i];
        TAILQ_INSERT_TAIL(l, n, entries);
    }

}


void p_6_2()
{
    struct tailhead l1, l2, l3;
    int a1[]={2,4,5,6,10};
    int a2[]={1,3,7,8};
    struct entry *n1, *n2, *tmp1, *tmp2;
    
    create_tailq(&l1, a1, 5);
    create_tailq(&l2, a2, 4);
#if 0    
    TAILQ_FOREACH(n1, &l1, entries)
        printf("%d ", (int)n1->d);
    printf("\n");
    TAILQ_FOREACH(n1, &l2, entries)
        printf("%d ", (int)n1->d);
    printf("\n");
#endif

    
    TAILQ_INIT(&l3);
    for (n1=TAILQ_FIRST(&l1), n2=TAILQ_FIRST(&l2) ;!TAILQ_EMPTY(&l1) && !TAILQ_EMPTY(&l2) ;
         n1= tmp1, n2=tmp2) {
        tmp1 = n1; tmp2=n2;
        if (n1->d < n2->d) {
            tmp1= TAILQ_NEXT(n1, entries);
            TAILQ_REMOVE(&l1, n1, entries);
            TAILQ_INSERT_TAIL(&l3, n1, entries);
        }
        else {
            tmp2 = TAILQ_NEXT(n2, entries);
            TAILQ_REMOVE(&l2, n2, entries);
            TAILQ_INSERT_TAIL(&l3, n2, entries);
        }
    }

    if (!TAILQ_EMPTY(&l1))
        TAILQ_CONCAT(&l3, &l1, entries);
    else
        TAILQ_CONCAT(&l3, &l2, entries);

    TAILQ_FOREACH(n1, &l3, entries)
        printf("%d ", (int)n1->d);
    printf("\n");
}

void p_6_7(int *arr, int len)
{
    int counts[3] = {0};
    int i;
    int *data;
    for (i=0; i<len; i++) {
        if (!arr[i])
            counts[0]++;
        else if (arr[i] == 1)
            counts[1]++;
        else if (arr[i] == 2)
            counts[2]++;
    }
    counts[1]+= counts[0];
    counts[2]+= counts[1];
    data= calloc(1, len*sizeof(int));
    for (i=len-1; i>=0; i--) {
        data[counts[arr[i]]-1]=arr[i];
        counts[arr[i]]--;
    }
    for (i=0; i<len; i++)
        printf("%d ", data[i]);
    printf("\n");
    
}

int bisearch(int *arr, int len, int val, int lower_bound)
{
    int left, right, mid;
    left =0;
    right =len-1;
    while (left!= right) {
        mid = (left+right)/2;
        if (arr[mid]<=val)
            left = mid+1;
        else
            right  = mid;
    }
    
    return left;
}
void queue_del_last_str(struct tailhead *l)
{
    if (TAILQ_EMPTY(l))
        return;
    struct entry *n;
    n = TAILQ_LAST(l, tailhead);
    TAILQ_REMOVE(l, n, entries);
}

void queue_print_str(struct tailhead *l)
{
    struct entry *p;
    TAILQ_FOREACH(p, l, entries) {
        printf("%s, ", (char *)p->d);
    }
    printf("\n");
}

int is_palindrome(char *s, int start, int end)
{
    int i;
    while (start<=end) {
        if (s[start]!=s[end])
            return 0;
        start++;
        end--;
    }
    return 1;
}
void get_partition(char *s, int i, struct tailhead *l)
{
    int j, len;
    char *substr;
    struct entry *n;
    len= strlen(s);
    
    if (i>=len) {
        queue_print_str(l);
        //TAILQ_INIT(l);
    }
    for (j=i; j<len; j++) {
        if (is_palindrome(s, i, j)) {
            substr = calloc(1, (j-i+1+1));
            memcpy(substr, &s[i], j-i+1);
            n = calloc(1, sizeof(*n));
            n->d = substr;
            TAILQ_INSERT_TAIL(l, n, entries);
            get_partition(s, j+1, l);
            queue_del_last_str(l);
        }
    }
}
void p_10_1(char *s)
{
    struct tailhead l;
    TAILQ_INIT(&l);
    int i=0;

    get_partition(s, i, &l);
}
struct _path {
    int r[128];
    int c[128];
    int n;
    int a;
};
void path_print(struct _path *p)
{
    int i;
    for (i=0; i<p->n; i++)
        printf("%d %d,", p->r[i], p->c[i]);
    printf("\n");
}
void path_pop(struct _path *p)
{
    if (!p->n)
        return;
    p->n--;
}
void path_push(struct _path *p, int i, int j)
{
    if (p->n ==128)
        return;
    p->r[p->n] = i;
    p->c[p->n] = j;
    p->n++;
}

void robot_go(int i, int j, int r, int c, struct _path *p)
{
    if (i>r || j>c)
        return ;
    if (i== r && j==c) {
        //path_print(p);
        p->a++;
        return;
    }

    path_push(p, i, j);
    robot_go(i+1, j, r, c, p);
    robot_go(i, j+1, r, c, p);
    path_pop(p);
}

void p_10_2(int r, int c)
{
    int i,j;
    struct _path p;
    i=j=0;
    memset(&p, 0, sizeof(p));
    robot_go(i, j, r, c,&p);
    printf("%d\n", p.a);
    
}
int queen_can_place(struct _path *p, int r, int c)
{
    int i, j;
    for (i=0; i<p->n; i++) {
        if (p->r[i] == r || p->c[i]==c) {
                return 0;
        }
        if (p->c[i] == c+(r-p->r[i])  ||
            p->c[i] == c-(r-p->r[i]))
            return 0;
    }
    return 1;
        
}
void queen_place(int r, int n, struct _path *p)
{
    if (r == n) {
        path_print(p);
        p->a++;
        return;
    }
    int j;
    for (j=0; j<n; j++) {
        if (queen_can_place(p, r, j)) {
            path_push(p, r, j);
            queen_place(r+1, n, p);
            path_pop(p);
        }
    }
}

void p_10_4(int n)
{
    int i,j;
    struct _path p;
    i=j=0;
    memset(&p, 0, sizeof(p));
    queen_place(i, n, &p);
    printf("%d\n", p.a);
}
struct _path2 {
    int paren[128];
    int left_n;
    int right_n;
    int a;
};

void path2_print(struct _path2 *p)
{
    int i;
    for (i=0;i<p->left_n+p->right_n; i++)
        printf("%c", p->paren[i]);
    printf("\n");
}
void path2_push(struct _path2 *p, int c)
{
    p->paren[p->left_n+p->right_n] = c;
    if (c == '(') 
        p->left_n++;
    else
        p->right_n++;
}
void path2_pop(struct _path2 *p)
{
    if (p->paren[p->left_n+p->right_n-1] == '(' )
        p->left_n--;
    else
        p->right_n--;
}
void paren_place(int n, struct _path2 *p, int c)
{
    int left= p->left_n;
    int right= p->right_n;
    if (left>n || right >n)
        return;
    if (right > left)
        return;

    if (left == n && right == n) {
        path2_print(p);
        p->a++;
        return;
    }
    
    path2_push(p, c);
    paren_place(n, p, '(');
    if (p->left_n >p->right_n)
        paren_place(n, p, ')');
    path2_pop(p);
}    

struct _pos {
    int i,j;
};

void p_10_9(int n)
{
    int i,j;
    struct _path2 p;
    i=j=0;
    memset(&p, 0, sizeof(p));
    paren_place(n, &p, '(');
    printf("%d\n", p.a);
}
#define M 3
#define N 4
#define ADD_POS() do { \
pos[k].i= i; \
pos[k].j = j; \
k++; \
    } while(0);
int found_char(int arr[][N], int i, int j, int c)
{
    if (i<0 || i>=M)
        return 0;
    if (j<0 || j>=N)
        return 0;
    if (arr[i][j] == c)
        return 1;
    return 0;
}
int find_char(int arr[][N], int c, struct _path *path, struct _pos *pos)
{
    int i,j;
    int k=0;
    //first char needs search through arr
    if (!path->n) {
        for (i=0; i<M; i++) {
            for (j=0; j<N; j++){
                if (c == arr[i][j]) {
                    pos[k].i = i;
                    pos[k].j = j;
                    k++;
                }
            }
        }
    }
    else {
        struct _pos last_pos;
        last_pos.i = path->r[path->n-1];
        last_pos.j = path->c[path->n-1];
        i=last_pos.i-1;
        j=last_pos.j;
        if (found_char(arr, i,j, c)) ADD_POS();
        i=last_pos.i+1;
        j=last_pos.j;
        if (found_char(arr, i,j, c)) ADD_POS();
        i=last_pos.i;
        j=last_pos.j-1;
        if (found_char(arr, i,j, c)) ADD_POS();
        i=last_pos.i;
        j=last_pos.j+1;
        if (found_char(arr, i,j, c)) ADD_POS();
    }
        return k;
}
#undef ADD_POS
//todo, how to stop when first match finds, don't continue
void find_word(int arr[][N], char *str, struct _path *p, int index)
{
    int n, i;
    struct _pos pos[M*N];
    if (index==strlen(str)) {
        path_print(p);
        return;
    }

    if (!(n= find_char(arr, str[index], p, pos)))
        return;
    for (i=0; i<n; i++) {
        path_push(p, pos[i].i, pos[i].j);
        find_word(arr, str, p, index+1);
        path_pop(p);
    }
}
void p_10_11(int arr[][N], char *str)
{
    int i;
    struct _path p;
    memset(&p, 0, sizeof(p));
    find_word(arr, str, &p, 0);
}


void print_permut(char *str, char *buf, char *used)
{
    int len=0;
    int i;
    if (strlen(buf) == strlen(str))
        printf("%s\n", buf);
    
    for (i=0; i<strlen(str); i++) {
        if (used[i])
            continue;
        
        len= strlen(buf);
        buf[len] = str[i];
        used[i] = 1;
        print_permut(str, buf, used);
        used[i] = 0;
        buf[len] = 0;
    }
}

void print_str2(char *str, char *buf, char *used, int need)
{
    int len=0;
    int i;

    if (!need) {
        printf("%s\n", buf);
        return;
    }
            
    if (!str[0] ) {
        return;
    }
#if 0    
    if (strlen(buf) == need) {
        printf("%s\n", buf);
        return;
    }
#endif    
    for (i=0; i<strlen(str); i++) {
        len = strlen(buf);
        buf[len] = str[i];
        print_str2(str+i+1, buf, used, need-1);
        buf[len] = 0;

    }
}
void print_str(char *str, char *buf, char *used)
{
    int len=0;
    int i;
    for (i=0; i<strlen(str); i++)  {
        buf[0] = 0;
        memset(used, 0, strlen(str));
        print_str2(str, buf, used, i+1);
    }
}
int tel_code[3][3] = {
    {'a', 'b', 'c'},
    {'d', 'e', 'f'},
    {'g', 'h', 'i'}
};
int get_char(int tel,  int pos)
{
    return tel_code[tel-2][pos];
}
int g;
void print_tel(int *tel, char *buf)
{
    int i, j, k;
    int len2, c;
    
    if (strlen(buf) == 3) {
        printf("%s\n", buf);
        g++;
        return;
    }
    
    for (j=0; j<3; j++) {
            len2= strlen(buf);
            c =get_char(tel[0], j);
            buf[len2] = c;
            print_tel(tel+1, buf);
            buf[len2] = 0;
    }
        
}

int find_nonrepeat(char *str)
{
    int i, j, k;
    for (i=0;i<strlen(str); i++) {
        for (j=0; j<strlen(str) ; j++) {
            if (i!=j && str[j] == str[i] )
                break;
        }
        if (j == strlen(str))
            return str[i];
    }
    return 0;
}
void copy_str(char *dst, int *pos, char *src, int start, int end)
{
    memcpy(dst+*pos-(end-start+1), src+start, (end-start+1));
    *pos = *pos-(end-start+1);
}
void reverse_words(char * str)
{
    char buf[128] ={0};
    int i,j, start, end, pos;

    start = end=0;
    pos = strlen(str);
    for (i=0; i<strlen(str); i++) {
        if (str[i] == ' ' ) {
            end = i-1;
            copy_str(buf, &pos, str, start, end);
            buf[--pos]= ' ';

            start = i+1;
        }

        if (i == strlen(str)-1) {
            end = i;
            copy_str(buf, &pos, str, start, end);
        }
    }
    printf("%s\n", buf);
    
}

void str2int(char *str)
{
    int i, ret =0;
    for (i=0; i<strlen(str); i++) {
        ret = ret*10+ str[i]-'0';
    }
    printf("ret %d\n", ret);
}

void int2str(int n)
{
    char buf[128] = {0};
    int i, j,k, len;
    len = 0;
    while( n ) {
        i = n%10;
        buf[len++] = i+'0';
        n = n/10;
    }
    for (i=0; i<strlen(buf)/2; i++) {
        char tmp;
        tmp = buf[i];
        buf[i] = buf[strlen(buf)-1-i];
        buf[strlen(buf)-1-i] = tmp;
    }
    printf("%s\n", buf);
}

void test_bitf()
{
    struct {
        char i1:4, i2:2, i3:2;
    } s;
    unsigned char c;
    s.i1= 0x7;
    s.i2 = 0x1;
    s.i3= 0x2;

    memcpy(&c, &s, 1);
    printf("%x\n", c      );
        
}
void str_cli(FILE *fp, int sockfd)
{
    int maxfdp1;
    fd_set rset;
    char sendline[256], recvline[256];

    FD_ZERO(&rset);
    for (;;) {
        FD_SET(fileno(fp), &rset);
        FD_SET(sockfd, &rset);
        maxfdp1 = (sockfd> fileno(fp)? sockfd:fileno(fp))+1;
        select(maxfdp1,  &rset, NULL, NULL, NULL);
        if (FD_ISSET(sockfd, &rset)) {
            ;
        }
    }
    
}
void test_select(int argc, char **argv)
{
    int sockfd;
    struct sockaddr_in servaddr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(10000);
    inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
    connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    str_cli(stdin, sockfd);
    
}
void test_l2packet()
{
    int fd;
    struct ifreq ifr;
    struct sockaddr_ll ll;
    
    
    fd= socket(PF_PACKET, SOCK_DGRAM, htons(0x8086));
    memset(&ifr, 0, sizeof(ifr));
    strcpy(ifr.ifr_name, "eth0");
    if (ioctl(fd, SIOCGIFINDEX, &ifr)) {
        perror("ioctl1");
        return;
    }
    memset(&ll, 0, sizeof(ll));
    ll.sll_family = PF_PACKET;
    ll.sll_ifindex = ifr.ifr_ifindex;
    ll.sll_protocol = htons(0x8086);
    if (bind(fd, (struct sockaddr *)&ll, sizeof(ll))) {
        perror("bind");
        return;
    }

    //memset(&ll, 0, sizeof(ll));
    ll.sll_halen = 6;
    unsigned char dst_addr[6] = {0x18, 0x66,0xda, 0x11,0x22, 0x33};
    memcpy(ll.sll_addr, dst_addr, 6);
    char buf[32] = {0};
    memset(buf, 0xaa, 32);
    if (sendto(fd, buf, 32, 0, (struct sockaddr *)&ll, sizeof(ll))) {
        perror("sendto");
        return;
    }
    
}
#undef offsetof
#undef container_of
#define offsetof(type, member) ((long) &((type *) 0)->member)

#define container_of(ptr, type, member)					\
	({								\
		const typeof(((type *) NULL)->member) *__mptr = (ptr);	\
		(type *) ((char *) __mptr - offsetof(type, member));	\
	})



void test_ms()
{
    struct  _s {
        int i,j;
        char c;
    }m;
    printf("offset %d\n", offsetof(struct _s, c));
    printf("%p %p\n", &m, container_of(&m.c, struct _s, c));
}

int main()  
{
    //p_6_2();
#if 0    
    int arr[]= {0,1,2,2,1,1,1,0,2};
    p_6_7(arr, 9);
#endif
#if 0    
    int arr[]= {1,8,8,8,8,8,8,9};
    printf("%d\n", bisearch(arr, 8, 8, 1));
    //printf("%d\n", bisearch(arr, 8, 8, 0));
#endif
    //p_10_1("aabccb");
    //p_10_4(5);
    //p_10_9(4);
#if 0     
    int arr[M][N] = {
        {'A', 'B', 'C', 'E'},
        {'S', 'F', 'C', 'S'},
        {'A', 'D', 'E', 'E'}
    };
    p_10_11(arr, "ABCCED");
    p_10_11(arr, "SAB");
    p_10_11(arr, "SEEDFBA");
    p_10_11(arr, "SFEDFBA");
#endif
#if 0    
    char buf[128]= "";
    char used[128]= {0};
    print_permut("abcd", buf, used);
#endif

#if 0
    char buf[128]= "";
    char used[128]= {0};
    print_str("abcd", buf, used);
#endif
#if 0    
    int tel[3] = {2,3,4};
    char buf[128] = "";
    print_tel(tel , buf);
    printf("%d\n", g);
#endif
#if 0    
    printf("%c\n", find_nonrepeat("total"));
    printf("%c\n", find_nonrepeat("teeter"));
#endif
    //reverse_words("Do or do not, there is no try.");
    //str2int("12345");
    //int2str(12345);
    //test_bitf();
    //test_l2packet();
    test_ms();
}

