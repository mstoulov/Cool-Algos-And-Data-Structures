#include <bits/stdc++.h>
using namespace std;

template <class T> class pers_stack;
template <class T> class pers_stack_kek;

template <class T>
class pers_stack_node {
 public:
    pers_stack_node(T val, int prev, int size) : val(val), prev(prev), size(size) {}
 private:
    friend class pers_stack_kek<T>;
    T val;
    int prev, size;
};

template <class T>
class pers_stack_kek {
 public:
    pers_stack_kek(pers_stack<T> *st, int ind) : st(st), ind(ind) {}
    void push(T x) {
        st->vec.push_back(pers_stack_node<T>(x, ind, st->vec[ind].size + 1));
    }
    T top() { return st->vec[ind].val; }
    int prev() { return st->vec[ind].prev; }
    int pop() {
        st->vec.push_back(pers_stack_node<T>(st->vec[st->vec[ind].prev].val, st->vec[st->vec[ind].prev].prev, st->vec[st->vec[ind].prev].size));
        return st->vec[ind].val;
    }
    int size() { return st->vec[ind].size; }
    bool empty() { return st->vec[ind].size == 0; }
 private:
    pers_stack<T> *st;
    int ind;
};

template <typename T>
class pers_stack {
 public:
    pers_stack() { vec.push_back(pers_stack_node<T>(empty_val, 0, 0)); }
    ~pers_stack() = default;
    int size() { return vec.size(); }
    pers_stack_kek<T> back() { return pers_stack_kek<T>(this, vec.size() - 1); }
    pers_stack_kek<T> operator[](int i) { return pers_stack_kek<T>(this, i); }
 private:
    vector<pers_stack_node<T>> vec;
    T empty_val = -INT_MAX - 1;
    friend class pers_stack_kek<T>;
};

template <typename T>
class pers_queue_elem;

template <class T>
class pers_queue {
 public:
    pers_queue() {
        L.push_back(&L_);
        R.push_back(&R_);
        L1.push_back(&L1_);
        Rc.push_back(&Rc_);
        Rc1.push_back(&Rc1_);
        S.push_back(&S_);
        recopy.push_back(0);
        copied.push_back(0);
        to_copy.push_back(0);
        L_inds.push_back(0);
        R_inds.push_back(0);
        L1_inds.push_back(0);
        Rc_inds.push_back(0);
        Rc1_inds.push_back(0);
        S_inds.push_back(0);
    }
    pers_queue_elem<T> back() {
        return pers_queue_elem<T>(*this, L.size() - 1);
    }
    pers_queue_elem<T> operator[](int ind) {
        return pers_queue_elem<T>(*this, ind);
    }
 private:
    pers_stack<T> L_, R_, L1_, Rc_, Rc1_, S_;
    vector<pers_stack<T>*> L, R, L1, Rc, Rc1, S;
    vector<int> L_inds, R_inds, L1_inds, Rc_inds, Rc1_inds, S_inds;
    vector<bool> recopy, copied;
    vector<int> to_copy;
    friend class pers_queue_elem<T>;
};

template <typename T>
class pers_queue_elem {
 public:
    pers_queue_elem(pers_queue<T> &q, int ind) : q(q), i(ind) {}
    bool empty() {
        return !q.recopy[i] && (*q.R[i])[q.R_inds[i]].empty();
    }
    int size() {
        if (!q.recopy[i]) return (*q.L[i])[q.L_inds[i]].size() + (*q.R[i])[q.R_inds[i]].size();
        if (!q.copied[i]) return (*q.Rc[i])[q.Rc_inds[i]].size() + (*q.L[i])[q.L_inds[i]].size() + (*q.L1[i])[q.L1_inds[i]].size();
        else return (*q.L[i])[q.L_inds[i]].size() + (*q.L1[i])[q.L1_inds[i]].size() + (*q.R[i])[q.R_inds[i]].size() + q.to_copy[i];
    }
    void push(T x) {
        make_new_elem();
        if (!q.recopy[i]) {
            (*q.L[i])[q.L_inds[i]].push(x);
            q.L_inds[i] = q.L[i]->size() - 1;
            if ((*q.Rc1[i])[q.Rc1_inds[i]].size()) {
                (*q.Rc1[i])[q.Rc1_inds[i]].pop();
                q.Rc1_inds[i] = q.Rc1[i]->size() - 1;
            }
            check_recopy();
        } else {
            (*q.L1[i])[q.L1_inds[i]].push(x);
            q.L1_inds[i] = q.L1[i]->size() - 1;
            add_ops();
        }
    }
    T pop() {
        make_new_elem();
        if (!q.recopy[i]) {
            T x = (*q.R[i])[q.R_inds[i]].pop();
            q.R_inds[i] = q.R[i]->size() - 1;
            (*q.Rc[i])[q.Rc_inds[i]].pop();
            q.Rc_inds[i] = q.Rc[i]->size() - 1;
            if ((*q.Rc1[i])[q.Rc1_inds[i]].size()) {
                (*q.Rc1[i])[q.Rc1_inds[i]].pop();
                q.Rc1_inds[i] = q.Rc1[i]->size() - 1;
            }
            check_recopy();
            return x;
        } else {
            T x = (*q.Rc[i])[q.Rc_inds[i]].pop();
            q.Rc_inds[i] = q.Rc[i]->size() - 1;
            if (q.to_copy[i]) {
                --q.to_copy[i];
            } else {
                (*q.R[i])[q.R_inds[i]].pop();
                q.R_inds[i] = q.R[i]->size() - 1;
                (*q.Rc1[i])[q.Rc1_inds[i]].pop();
                q.Rc1_inds[i] = q.Rc1[i]->size() - 1;
            }
            add_ops();
            return x;
        }
    }
    T front() {
        if (!q.recopy[i]) return (*q.R[i])[q.R_inds[i]].top();
        else return (*q.Rc[i])[q.Rc_inds[i]].top();
    }
    T top() {
        if (!q.recopy[i]) return (*q.L[i])[q.L_inds[i]].top();
        else return (*q.L1[i])[q.L1_inds[i]].top();
    }
    void clear() {
        make_new_elem();
        while (!(*q.L[i])[q.L_inds[i]].empty()) {
            (*q.L[i])[q.L_inds[i]].pop();
            q.L_inds[i] = q.L[i]->size() - 1;
        }
        while (!(*q.L1[i])[q.L1_inds[i]].empty()) {
            (*q.L1[i])[q.L1_inds[i]].pop();
            q.L1_inds[i] = q.L1[i]->size() - 1;
        }
        while (!(*q.R[i])[q.R_inds[i]].empty()) {
            (*q.R[i])[q.R_inds[i]].pop();
            q.R_inds[i] = q.R[i]->size() - 1;
        }
        while (!(*q.Rc[i])[q.Rc_inds[i]].empty()) {
            (*q.Rc[i])[q.Rc_inds[i]].pop();
            q.Rc_inds[i] = q.Rc[i]->size() - 1;
        }
        while (!(*q.Rc1[i])[q.Rc1_inds[i]].empty()) {
            (*q.Rc1[i])[q.Rc1_inds[i]].pop();
            q.Rc1_inds[i] = q.Rc1[i]->size() - 1;
        }
        while (!(*q.S[i])[q.S_inds[i]].empty()) {
            (*q.S[i])[q.S_inds[i]].pop();
            q.S_inds[i] = q.S[i]->size() - 1;
        }
        q.recopy[i] = q.copied[i] = 0;
        q.to_copy[i] = 0;
    }
 private:
    void make_new_elem() {
        q.S.push_back(q.S[i]);
        q.L.push_back(q.L[i]);
        q.L1.push_back(q.L1[i]);
        q.R.push_back(q.R[i]);
        q.Rc.push_back(q.Rc[i]);
        q.Rc1.push_back(q.Rc1[i]);
        q.L_inds.push_back(q.L_inds[i]);
        q.R_inds.push_back(q.R_inds[i]);
        q.S_inds.push_back(q.S_inds[i]);
        q.L1_inds.push_back(q.L1_inds[i]);
        q.Rc_inds.push_back(q.Rc_inds[i]);
        q.Rc1_inds.push_back(q.Rc1_inds[i]);
        q.recopy.push_back(q.recopy[i]);
        q.copied.push_back(q.copied[i]);
        q.to_copy.push_back(q.to_copy[i]);
        i = q.S.size() - 1;
    }
    void add_ops() {
        int to_do = 3;
        for (; !q.copied[i] && to_do && (*q.R[i])[q.R_inds[i]].size() > 0; --to_do) {
            (*q.S[i])[q.S_inds[i]].push((*q.R[i])[q.R_inds[i]].pop());
            q.R_inds[i] = q.R[i]->size() - 1;
            q.S_inds[i] = q.S[i]->size() - 1;
        }
        for (; to_do && (*q.L[i])[q.L_inds[i]].size(); --to_do) {
            q.copied[i] = true;
            T x = (*q.L[i])[q.L_inds[i]].pop();
            q.L_inds[i] = q.L[i]->size() - 1;
            (*q.R[i])[q.R_inds[i]].push(x);
            q.R_inds[i] = q.R[i]->size() - 1;
            (*q.Rc1[i])[q.Rc1_inds[i]].push(x);
            q.Rc1_inds[i] = q.Rc1[i]->size() - 1;
        }
        for (; to_do && (*q.S[i])[q.S_inds[i]].size(); --to_do) {
            T x = (*q.S[i])[q.S_inds[i]].pop();
            q.S_inds[i] = q.S[i]->size() - 1;
            if (q.to_copy[i]) {
                (*q.R[i])[q.R_inds[i]].push(x);
                q.R_inds[i] = q.R[i]->size() - 1;
                (*q.Rc1[i])[q.Rc1_inds[i]].push(x);
                q.Rc1_inds[i] = q.Rc1[i]->size() - 1;
                --q.to_copy[i];
            }
        }
        if ((*q.S[i])[q.S_inds[i]].empty()) {
            swap(q.L[i], q.L1[i]);
            swap(q.L_inds[i], q.L1_inds[i]);
            swap(q.Rc[i], q.Rc1[i]);
            swap(q.Rc_inds[i], q.Rc1_inds[i]);
            q.recopy[i] = 0;
        }
    }
    void check_recopy() {
        if ((*q.L[i])[q.L_inds[i]].size() > (*q.R[i])[q.R_inds[i]].size()) {
            q.recopy[i] = 1;
            q.to_copy[i] = (*q.R[i])[q.R_inds[i]].size();
            q.copied[i] = false;
            add_ops();
        }
    }
    pers_queue<T>& q;
    int i;
    friend class pers_queue<T>;
};
