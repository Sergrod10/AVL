#include <bits/stdc++.h>

#define st string
#define pb push_back
#define vec vector
#define all(a) a.begin(), a.end()
#define ff first
#define ss second
#define int long long

using namespace std;
using vl = vec<int>;
using vi = vec<int>;
using vvi = vec<vi>;
using pi = pair<int, int>;
using vpi = vec<pi>;
using vvvi = vec<vvi>;
using vvpi = vec<vec<pi>>;
using vvvpi = vec<vvpi>;
using vvvvi = vec<vvvi>;
using vppi = vec<pair<pair<int, int>, pair<int, int>>>;

bool comp(vi& a, vi& b) {
    return (a.size() < b.size());
}

struct Compare {
    bool operator()(int a, int b) const {
        return (a > b);
    }
};

template<typename T>
struct Node {
    int l = 0, r = 0;
    T val;
    int h = 0;
    int p = 0;
};


template<typename T>
class BinaryTree {
    vec<Node<T>> t;
    int c = 0;
    int root = 0;

public:

    BinaryTree() {
        Node<T> x;
        t.pb(x);
    }

    // правильно
    void chh(int v) {
        if (v == 0) {
            t[v].h = 0;
            return;
        }
        t[v].h = max(t[t[v].l].h + 1, t[t[v].r].h + 1);
    }

    // возвращает индекс корня теперь правильно
    int turnleft(int v) {
        if (v == 0) return 0;
        int rt = t[v].r;
        t[v].r = t[rt].l;
        t[rt].l = v;
        t[rt].p = t[v].p;
        t[v].p = rt;
        chh(v);
        chh(rt);
        return rt;
    }

    // правильно
    int turnright(int v) {
        if (v == 0) return 0;
        int rt = t[v].l;
        t[v].l = t[rt].r;
        t[rt].r = v;
        t[rt].p = t[v].p;
        t[v].p = rt;
        chh(v);
        chh(rt);
        return rt;
    }

    // слева больше правильно
    int balanceleft(int v) {
        int left = t[t[v].l].l;
        int right = t[t[v].l].r;
        if (t[right].h - t[left].h == 1) {
            t[v].l = turnleft(t[v].l);
            chh(t[v].l);
        }
        return turnright(v);
    }

    // справа больше правильно
    int balanceright(int v) {
        int left = t[t[v].r].l;
        int right = t[t[v].r].r;
        if (t[left].h - t[right].h == 1) {
            t[v].r = turnright(t[v].r);
            chh(t[v].r);
        }
        return turnleft(v);
    }

    // правильно
    int balance(int v) {
        if (v == 0) return 0;
        int left = t[v].l;
        int right = t[v].r;
        if (abs(t[left].h - t[right].h) <= 1) {
            return v;
        }
        if (t[left].h > t[right].h) {
            return balanceleft(v);
        }
        return balanceright(v);
    }

    // правильно
    void pr(int v) {
        if (v == 0) {
            return;
        }
        cout << "id: " << v << ", " << "val: " << t[v].val << ", " << "height: " << t[v].h << ", " << "left child: " << t[v].l << ", " << "right child: " << t[v].r << "\n";
        pr(t[v].l);
        pr(t[v].r);
    }

    // правильно
    void print() {
        pr(root);
    }

    void gett(int v, vi &a) {
        if (v == 0) {
            return;
        }
//        cout << v + 1 << " " << t[v].val << " " << t[v].h << " " << t[v].l + 1 << " " << t[v].r + 1 << "\n";
        gett(t[v].l, a);
        a.pb(t[v].val);
        gett(t[v].r, a);
    }

    vi get() {
        vi a;
        gett(root, a);
        return a;
    }

    //  НЕПРАВИЛЬНО .ff => < val
    pi split(int v, T val) {
        if (v == 0) return {0, 0};
        if (t[v].val >= val) {
            pi p = split(t[v].l, val);
            t[v].l = p.ss;
            chh(v);
            return {balance(p.ff), balance(v)};
        }
        else {
            pi p = split(t[v].r, val);
            t[v].r = p.ff;
            chh(v);
            return {balance(v), balance(p.ss)};
        }
    }

    // неправильно
    int merge(int a, int b) {
        if (a == 0) return balance(b);
        if (b == 0) return balance(a);
        t[b].l = merge(a, t[b].l);
        return balance(b);
    }

    // правильно
    int find(int v, T val) {
        if (v == 0) {
            return 0;
        }
        if (t[v].val == val) {
            return v;
        }
        if (t[v].val > val) {
            return find(t[v].l, val);
        }
        return find(t[v].r, val);
    }

    // l == 0 => left child, 1 => right child
    int push(int v, T val, int p, int l) {
        //cout << val << " " << p << " " << l << endl;
        //return;
        if (v == 0) {
            Node<T> x;
            x.val = val;
            x.h = 1;
            t.pb(x);
            c++;
            if (l == 0) {
                t[p].l = c;
            }
            else {
                t[p].r = c;
            }
            if (!p) {
                root = c;
            }
            t[c].p = p;
            return c;
        }
        if (t[v].val > val) {
            t[v].l = push(t[v].l, val, v, 0);
            chh(v);
            return balance(v);
        }
        t[v].r = push(t[v].r, val, v, 1);
        chh(v);
        return balance(v);
    }

    // правильно
    void insert(T val) {
        if (find(root, val)) {
            return;
        }
        root = push(root, val, 0, 0);
    }

    // возвраращает индекс элемента, если такого нет вернет 0
    int getval(T val) {
        return find(root, val);
    }

    // не надо менять
    int findmin(int v) {
        if (t[v].l != 0) {
            return findmin(t[v].l);
        }
        return v;
    }

    // возвращает пару {значение, индекс} минимального элемента, если такого нет вернет {0, 0}
    pi getmin() {
        int id = findmin(root);
        if (id == 0) {
            return {0, 0};
        }
        return {t[id].val, id};
    }

    int dodel(int v, int id) {
//        cout << v + 1 << " " << id << "\n";
        if (v == 0) return 0;
        if (v == id) {
            if (!t[v].r) {
                return t[v].l;
            }
            t[v].r = dodel(t[v].r, id);
            chh(v);
            return balance(v);
        }
        else if (t[v].l) {
            t[v].l = dodel(t[v].l, id);
            chh(v);
            return balance(v);
        }
        else {
            t[id].val = t[v].val;
            t[v].h = 0;
            return t[v].r;
        }
    }

    int findtodel(int v, T val) {
//        cout << v + 1 << " " << t[v].l + 1 << " " << t[v].r + 1 << " " << t[v].val << " " << val << "\n";
        if (v == 0) {
            return 0;
        }
        if (t[v].val == val) {
            return dodel(v, v);
        }
        if (t[v].val > val) {
            t[v].l = findtodel(t[v].l, val);
            chh(v);
            return balance(v);
        }
        t[v].r = findtodel(t[v].r, val);
        chh(v);
        return balance(v);
    }

    void del(T val) {
        root = findtodel(root, val);
    }

    void delmin() {
        pi p = getmin();
//        cout << p.ff << " " << p.ss << "\n";
        if (!p.ss) {
            return;
        }
        del(p.ff);
    }

    int get_height() {
        return t[root].h;
    }

    int checkbal(int v) {
        if (v == 0) {
            return 1;
        }
        int ok1 = checkbal(t[v].l);
        int ok2 = checkbal(t[v].r);
        int ok3 = (abs(t[t[v].l].h - t[t[v].r].h) <= 1);
        return (ok1 && ok2 && ok3);
    }

    int isbalance() {
        return checkbal(root);
    }
};

void solve() {
    system("chcp 65001");

//    BinaryTree<int> t;
//    int x; cin >> x;
//    while (x) {
//        t.print();
//        cout << "\n";
//        t.pushnew(x);
//        cin >> x;
//    }
//    t.print();
//    cout << t.isbalance();

//    int n = 100000;
//    int sz = 20;
//    for (int i = 0; i < n; i++) {
//        cout << "TESTING #" << i << endl;
//        BinaryTree<int> t;
//        vi a;
//        for (int j = 0; j < sz; j++) {
//            int x = rand() % 40 + 1;
//            t.insert(x);
//            a.pb(x);
//        }
//        if (!t.isbalance()) {
//            for (auto u : a) {
//                cout << u << " ";
//            }
//            cout << endl;
//            t.print();
//            break;
//        }
//    }

//    int n = 10000;
//    int quers = 10000;
//    for (int i = 0; i < n; i++) {
//        cout << "TESTING #" << i << endl;
//        BinaryTree<int> t;
//        set<int> s;
//        vpi q;
//        int ok = 1;
//        for (int j = 0; j < quers; j++) {
//            int x = rand() * 141327ll;
//            int tt = rand() % 3;
//            q.pb({x, tt});
//            if (tt == 0) {
//                t.insert(x);
//                s.insert(x);
//            }
//            else if (tt == 1) {
//                t.del(x);
//                if (s.find(x) != s.end()) {
//                    s.erase(x);
//                }
//            }
//            else {
//                t.delmin();
//                if (s.size()) {
//                    s.erase(s.begin());
//                }
//            }
//
//            vi my = t.get();
//            int fl = (my.size() != s.size());
//            if (!fl) {
//                int ind = 0;
//                for (auto u : s) {
//                    if (u != my[ind]) {
//                        fl = 1;
//                        break;
//                    }
//                    ind++;
//                }
//            }
//            if (fl) {
//                for (auto u : s) {
//                    cout << u << " ";
//                }
//                cout << "\n\n";
//                t.print();
//                cout << "\n";
//                for (auto u : q) {
//                    cout << u.ff << " " << u.ss << "\n";
//                }
//                ok = 0;
//                break;
//            }
//        }
//        if (!ok) {
//            break;
//        }
//    }

//    BinaryTree<int> t;
//    t.insert(1);
//    t.insert(2);
//    t.delmin();
//    t.insert(2);
//    t.print();

    BinaryTree<int> t;
    cout << "1 - Вставка элемента с заданным ключом\n2 - Поиск элемента по ключу (возвращает указатель, если такого нет вернет 0)\n";
    cout << "3 - Поиск минимального элемента (возвращает пару {значение, индекс}, если такого нет вернет {0, 0})\n4 - Удаление элемента с минимальным ключом\n";
    cout << "5 - Удаление элемента с заданным ключом\n6 - Вывод дерева на экран дерева\n(в виде индекс, ключ, высота, индексы сыновей). Если нет сына вернет 0\n";
    cout << "Сколько хотите операций?" << endl;
    int q; cin >> q;
    while (q--) {
        int tp; cin >> tp;
        if (tp == 1) {
            int val; cin >> val;
            t.insert(val);
        }
        else if (tp == 2) {
            int val; cin >> val;
            cout << t.getval(val) << endl; // ???
        }
        else if (tp == 3) {
            pi p = t.getmin();
            cout << p.ff << " " << p.ss << endl;
        }
        else if (tp == 4) {
            t.delmin();
        }
        else if (tp == 5) {
            int val; cin >> val;
            t.del(val);
        }
        else {
            t.print();
            cout << endl;
        }
    }
}

signed main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
    // freopen("unionday.in", "r", stdin);
    // freopen("unionday.out", "w", stdout);
    solve();
}
