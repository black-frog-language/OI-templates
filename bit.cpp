#include <cstdio>
#include <vector>
using namespace std;

class BIT {
private:
    vector<int> v;
public:
    static inline int lowbit(int n) { return n & -n; }
    BIT(int len): v(vector<int>(len + 1, 0)) {}
    void add(int pos, int val) {
        while (pos < v.size()) {
            v[pos] += val;
            pos += lowbit(pos);
        }
    }
    int get(int end) {
        if (end == 0) return 0;
        return get(end - lowbit(end)) + v[end];
    }
};

int main() {
    int nNums, nOps;
    scanf("%d%d", &nNums, &nOps);
    BIT bit(nNums);
    for (int i = 1; i <= nNums; i++) {
        int num;
        scanf("%d", &num);
        bit.add(i, num);
    }
    for (int i = 0; i < nOps; i++) {
        int op, lhs, rhs;
        scanf("%d%d%d", &op, &lhs, &rhs);
        if (op == 1) {
            bit.add(lhs, rhs);
        } else if (op == 2) {
            printf("%d\n", bit.get(rhs) - bit.get(lhs - 1));
        }
    }
    return 0;
}

