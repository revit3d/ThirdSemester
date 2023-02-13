#include <stdio.h>

int 
max(const int left, const int right){
    return left > right ? left : right;
}

int
min(const int left, const int right){
    return left < right ? left : right;
}

long long
update(
        int curleft, 
        int curright, 
        int left, 
        int right, 
        int pos, 
        int add, 
        int *tree_sum, 
        int *tree_add)
{
    enum { SHIFT = 2 };
    if(curleft == left && curright == right){
        tree_sum[pos] += (right - left + 1) * add;
        tree_add[pos] += add;
        return (right - left + 1) * add;
    }
    if(curleft > right || curright < left)
        return 0;
    int mid = (curleft + curright) >> 1;
    long long delta_left = 
        update(curleft, mid, left, min(mid, right), SHIFT * pos, add, tree_sum, tree_add);
    long long delta_right = 
        update(mid + 1, curright, max(left, mid + 1), right, SHIFT * pos + 1, add, tree_sum, tree_add);
    tree_sum[pos] += delta_left + delta_right;
    return delta_left + delta_right;
}

long long
get(
        int curleft, 
        int curright, 
        int left, 
        int right, 
        int pos,
        int *tree_sum,
        int *tree_add)
{
    enum { SHIFT = 2 };
    if(curleft == left && curright == right){
        return tree_sum[pos];
    }
    if(curleft > right || curright < left)
        return 0;
    int mid = (curleft + curright) >> 1;
    long long left_sum = 
        get(curleft, mid, left, min(mid, right), SHIFT * pos, tree_sum, tree_add);
    long long right_sum = 
        get(mid + 1, curright, max(left, mid + 1), right, SHIFT * pos + 1, tree_sum, tree_add);
    return (long long)left_sum + right_sum + (right - left + 1ll) * tree_add[pos];
}

int
main(void){
    enum { MAXN = 10000 };
    int tree_sum[MAXN * 4] = {};
    int tree_add[MAXN * 4] = {};

    enum QueryTypes
    {
        ADD_QUERY = 1,
        GET_QUERY = 2
    };

    enum ReadValues
    {
        ONE_READ_VALUE = 1,
        TWO_READ_VALUES = 2,
        THREE_READ_VALUES = 3
    };

    int n, query_cnt, query_type;
    int left, right, add;
    if(scanf("%d %d", &n, &query_cnt) != TWO_READ_VALUES)
        return 1;
    for(int query = 0; query < query_cnt; query++){
        if(scanf("%d", &query_type) != ONE_READ_VALUE)
            return 1;   
        if(query_type == ADD_QUERY){
            if(scanf("%d %d %d", &left, &right, &add) != THREE_READ_VALUES)
                return 1;
            update(0, n - 1, left, right - 1, 1, add, tree_sum, tree_add);
        }
        else if(query_type == GET_QUERY){
            if(scanf("%d %d", &left, &right) != TWO_READ_VALUES)
                return 1;
            printf("%lld\n", get(0, n - 1, left, right - 1, 1, tree_sum, tree_add));
        }
    }

    return 0;
}