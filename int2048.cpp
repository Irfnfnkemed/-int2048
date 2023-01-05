#include "int2048.h"

const int condense = 1e8;
const int digits = 8;

sjtu::int2048::int2048() { return; }

sjtu::int2048::int2048(long long x) {
    if (x > 0) { positive = true; }
    else {
        positive = false;
        x = -x;
    }
    if (x == 0) {//对0特判
        len = 1;
        positive = true;
    } else {
        len = 0;
        while (x != 0) {
            num[len++] = x % condense;
            x /= condense;
        }
        for (int i = len; i < sjtu::MAX; ++i) {
            num[i] = 0;
        }
    }
}

sjtu::int2048::int2048(const std::string &s) {
    if (s[0] != '-') {
        positive = true;
        len = (s.size() / digits) + ((s.size() % digits == 0) ? 0 : 1);
        int j = 0, e = 1;
        for (int i = s.size() - 1; i >= 0; --i) {
            num[j] += e * (s[i] - '0');
            e *= 10;
            if (e == condense) {
                ++j;
                e = 1;
            }
        }
    } else {
        positive = false;
        len = ((s.size() - 1) / digits) + (((s.size() - 1) % digits == 0) ? 0 : 1);
        int j = 0, e = 1;
        for (int i = s.size() - 1; i >= 1; --i) {
            num[j] += e * (s[i] - '0');
            e *= 10;
            if (e == condense) {
                ++j;
                e = 1;
            }
        }
    }
    for (int i = len; i < sjtu::MAX; ++i) {
        num[i] = 0;
    }
    if (len > 1) {//除了0外，排除前导零
        int i = len - 1;
        while (1) {
            if (num[i] == 0) { --len; }
            else { break; }
            --i;
        }
    }
}

sjtu::int2048::int2048(const int2048 &f) :
        len(f.len), positive(f.positive) {
    for (int i = 0; i < len; ++i) { num[i] = f.num[i]; }
    for (int i = len; i < sjtu::MAX; ++i) {
        num[i] = 0;
    }
}

void sjtu::int2048::clear() {
    for (int i = 0; i < len; ++i) { num[i] = 0; }
    len = 1;
    positive = true;
}

void sjtu::int2048::read(const std::string &s) {
    clear();
    if (s[0] != '-') {
        positive = true;
        len = (s.size() / digits) + ((s.size() % digits == 0) ? 0 : 1);
        int j = 0, e = 1;
        for (int i = s.size() - 1; i >= 0; --i) {
            num[j] += e * (s[i] - '0');
            e *= 10;
            if (e == condense) {
                ++j;
                e = 1;
            }
        }
    } else {
        positive = false;
        len = ((s.size() - 1) / digits) + (((s.size() - 1) % digits == 0) ? 0 : 1);
        int j = 0, e = 1;
        for (int i = s.size() - 1; i >= 1; --i) {
            num[j] += e * (s[i] - '0');
            e *= 10;
            if (e == condense) {
                ++j;
                e = 1;
            }
        }
    }
    if (len > 1) {//除了0外，排除前导零
        int i = len - 1;
        while (1) {
            if (num[i] == 0) { --len; }
            else { break; }
            --i;
        }
    }
}

void sjtu::int2048::print() const {
    if (!positive && !(num[0] == 0 && len == 1)) { std::cout << '-'; }
    std::cout << num[len - 1];
    for (int i = len - 2; i >= 0; --i) {
        //确定每个压缩段存储的数据的位数
        int tmp = num[i];
        int j, c = condense / 10;
        for (j = 0; j < digits; ++j) {
            if (tmp / c != 0) break;
            c /= 10;
        }
        //除了第一段外，补上每段前的0
        for (int k = 1; k <= j; ++k) { std::cout << 0; }
        //若全为0，不需要再输出
        if (num[i] != 0) { std::cout << num[i]; }
    }
}

sjtu::int2048 &sjtu::int2048::add(const int2048 &add_after, bool flag) {
    //flag为false，表示f的正负要反一下
    bool add_positive = add_after.positive;
    if (!flag) {
        add_positive = !add_after.positive;
    }
    //对0的特判
    if (len == 1 && num[0] == 0) { positive = true; }
    if (add_after.len == 1 && add_after.num[0] == 0) {
        add_positive = true;
    }
    //加法，做两数均正或均负;一正一负，转至减法
    if ((positive && !add_positive) || (!positive && add_positive)) {
        return minus(add_after, false);
    } else {
        //相加
        for (int i = 0; i < ((len < add_after.len) ? len : add_after.len); ++i) {
            num[i] += add_after.num[i];
        }
        if (len < add_after.len) {
            for (int i = len; i < add_after.len; ++i) {
                num[i] = add_after.num[i];
            }
            len = add_after.len;
        }
        //进位
        int j = 0;
        while (j < len) {
            if (num[j] >= condense) {
                if (num[j + 1] == 0 && (j + 1) == len) { ++len; }
                ++num[j + 1];
                num[j] -= condense;
            }
            ++j;
        }
        return *this;
    }
}

sjtu::int2048 sjtu::add(int2048 add_l, const int2048 &add_r) {
    return add_l.add(add_r);
}

sjtu::int2048 &sjtu::int2048::minus(const int2048 &minus_after, bool flag) {
    //flag为false，表示f的正负要反一下
    bool minus_positive = minus_after.positive;
    if (!flag) {
        minus_positive = !minus_after.positive;
    }
    //对0的特判
    if (len == 1 && num[0] == 0) { positive = true; }
    if (minus_after.len == 1 && minus_after.num[0] == 0) {
        minus_positive = true;
    }
    if ((positive && !minus_positive) || (!positive && minus_positive)) {
        return add(minus_after, false);
    } else {
        //此时，两数同号，判断两个数绝对值的大小
        bool judge = true;//true表示第一个数绝对值大于等于第二个数绝对值
        int k;//当两数长度一样时，用于记录结果的长度
        if (len > minus_after.len) { judge = true; }
        else if (len < minus_after.len) { judge = false; }
        else {
            for (k = len - 1; k >= 0; --k) {
                if (num[k] < minus_after.num[k]) {
                    judge = false;
                    break;
                } else if (num[k] > minus_after.num[k]) { break; }
            }
        }
        //第一个数绝对值大于等于第二个数绝对值，结果与第一个数正负号相同
        if (judge) {
            //相减
            for (int i = 0; i < minus_after.len; ++i) {
                num[i] -= minus_after.num[i];
            }
            //退位
            if (len > minus_after.len) {//为了避免重新计算位数
                int j = 0, tmp_len = minus_after.len;
                while (j < tmp_len) {
                    if (num[j] < 0) {
                        if (num[j + 1] == 0 && (j + 1) == tmp_len) { ++tmp_len; }
                        --num[j + 1];
                        num[j] += condense;
                    }
                    ++j;
                }
                if (num[len - 1] == 0) { --len; }//可能的退位
            } else {
                int j = 0;
                while (j < k) {
                    if (num[j] < 0) {
                        --num[j + 1];
                        num[j] += condense;
                    }
                    ++j;
                }
                len = k + 1;
                if (num[len - 1] == 0) { --len; }//可能的退位
                if (j == -1) { len = 1; } //结果为0的特判
            }
        } else {//第一个数绝对值小第二个数绝对值，结果与第一个数正负号相反
            positive = !positive;
            //相减
            for (int i = 0; i < len; ++i) {
                num[i] = minus_after.num[i] - num[i];
            }
            for (int i = len; i < minus_after.len; ++i) {
                num[i] = minus_after.num[i];
            }
            //退位
            if (len < minus_after.len) {//为了避免重新计算位数
                int j = 0, tmp_len = len;
                while (j < tmp_len) {
                    if (num[j] < 0) {
                        if (num[j + 1] == 0 && (j + 1) == tmp_len) { ++tmp_len; }
                        --num[j + 1];
                        num[j] += condense;
                    }
                    ++j;
                }
                len = minus_after.len;
                if (num[len - 1] == 0) { --len; }//可能的退位
            } else {
                int j = 0;
                while (j < k) {
                    if (num[j] < 0) {
                        --num[j + 1];
                        num[j] += condense;
                    }
                    ++j;
                }
                len = k + 1;
                if (num[len - 1] == 0) { --len; }//可能的退位
                //结果为0的情况已归到上一种，不需特判
            }
        }
    }
    return *this;
}

sjtu::int2048 sjtu::minus(int2048 minus_l, const int2048 &minus_r) {
    return minus_l.minus(minus_r);
}

sjtu::int2048 &sjtu::int2048::operator=(const int2048 &op) {
    if (op == *(this)) return *this;
    len = op.len;
    positive = op.positive;
    for (int i = 0; i < len; ++i) {
        num[i] = op.num[i];
    }
    for (int i = len; i < sjtu::MAX; ++i) {
        num[i] = 0;
    }
    return *this;
}

sjtu::int2048 &sjtu::int2048::operator+=(const int2048 &op) {
    add(op);
    return *this;
}

sjtu::int2048 sjtu::operator+(int2048 op_l, const int2048 &op_r) {
    return op_l.add(op_r);
}

sjtu::int2048 &sjtu::int2048::operator-=(const int2048 &op) {
    minus(op);
    return *this;
}

sjtu::int2048 sjtu::operator-(int2048 op_l, const int2048 &op_r) {
    return op_l.minus(op_r);
}

sjtu::int2048 &sjtu::int2048::operator*=(const int2048 &op) {
    if (!op.positive) { positive = !positive; }
    long long tmp[sjtu::MAX] = {0};
    for (int i = 0; i < len; ++i)
        for (int j = 0; j < op.len; ++j)
            tmp[i + j] += (long long) num[i] * op.num[j];////////////////////为什么不强制类型转换会寄？
    //进位
    len += (op.len - 1);
    int j = 0;
    while (j < len) {
        if (tmp[j] >= condense) {
            if (tmp[j + 1] == 0 && (j + 1) == len) { ++len; }
            tmp[j + 1] += tmp[j] / condense;
            tmp[j] %= condense;
        }
        ++j;
    }
    for (int i = 0; i < len; ++i) {
        num[i] = tmp[i];
    }
    return *this;
}

sjtu::int2048 sjtu::operator*(int2048 op_l, const int2048 &op_r) {
    return op_l *= op_r;
}

sjtu::int2048 &sjtu::int2048::operator/=(const int2048 &op) {
    bool flag = true;
    if ((positive && !op.positive) || (!positive && op.positive)) { flag = false; }
    positive = true;
    if (len < op.len) { clear(); }//被除数比除数小
    else {
        int out[sjtu::MAX] = {0};
        int h = len - op.len;//商的可能的最大位的下标
        sjtu::int2048 tmp;
        for (int i = h; i >= 0; --i) {
            bool flag2 = true;//true表tmp=op*商，反之tmp=op*(商+1*后面位数)
            int l = 0, r = condense - 1;
            while (l < r) {
                int mid = (l + r + 1) / 2;
                long long tmp3[sjtu::MAX] = {0};//试商过程中用到乘法
                //计算 (mid*后面的位数)*op
                tmp.clear();
                for (int j = 0; j < op.len; ++j)
                    tmp3[i + j] += (long long) mid * op.num[j];
                //进位
                tmp.len = i + op.len;
                int j = i;
                while (j < tmp.len) {
                    if (tmp3[j] >= condense) {
                        if (tmp3[j + 1] == 0 && (j + 1) == tmp.len) { ++tmp.len; }
                        tmp3[j + 1] += tmp3[j] / condense;
                        tmp3[j] %= condense;
                    }
                    ++j;
                }
                //将结果存回tmp中
                for (int j = 0; j < tmp.len; ++j) { tmp.num[j] = tmp3[j]; }
                if (tmp == *this) {//若恰好整除，直接退出试商
                    flag2 = true;
                    l = mid;
                    break;
                }
                if (tmp > *this) {
                    flag2 = false;
                    r = mid - 1;
                } else {
                    flag2 = true;
                    l = mid;
                }
            }
            if (!flag2) {
                sjtu::int2048 tmp2;
                tmp2.len = i + op.len;
                for (int j = i; j < tmp2.len; ++j) {
                    tmp2.num[j] = op.num[j - i];
                }
                tmp -= tmp2;
            }
            *this -= tmp;
            out[i] = l;
        }
        for (int i = h; i >= 0; --i) { num[i] = out[i]; }
        for (int i = h; i >= 0; --i) {
            if (num[i] != 0) {
                len = i + 1;
                break;
            }
            if (i == 0) { len = 1; }//对0的特判
        }
    }
    if (!flag) { positive = false; }
    return *this;
}

sjtu::int2048 sjtu::operator/(int2048 op_l, const int2048 &op_r) {
    return op_l /= op_r;
}

std::istream &sjtu::operator>>(std::istream &is, int2048 &op) {
    std::string tmp;
    is >> tmp;
    op.read(tmp);
    return is;
}

std::ostream &sjtu::operator<<(std::ostream &os, const int2048 &op) {
    op.print();
    return os;
}

bool sjtu::operator==(const int2048 &op_l, const int2048 &op_r) {
    if (op_l.len == 1 && op_r.len == 1 &&
        op_l.num[0] == 0 && op_r.num[0] == 0) { return true; }//特判0
    if (op_l.positive != op_r.positive) { return false; }
    if (op_l.len != op_r.len) { return false; }
    for (int i = op_l.len - 1; i >= 0; --i) {
        if (op_l.num[i] != op_r.num[i]) { return false; }
    }
    return true;
}

bool sjtu::operator!=(const int2048 &op_l, const int2048 &op_r) {
    if (op_l.len == 1 && op_r.len == 1 &&
        op_l.num[0] == 0 && op_r.num[0] == 0) { return false; }//特判0
    if (op_l.positive != op_r.positive) { return true; }
    if (op_l.len != op_r.len) { return true; }
    for (int i = op_l.len - 1; i >= 0; --i) {
        if (op_l.num[i] != op_r.num[i]) { return true; }
    }
    return false;
}

bool sjtu::operator<(const int2048 &op_l, const int2048 &op_r) {
    if (op_l.len == 1 && op_r.len == 1 &&
        op_l.num[0] == 0 && op_r.num[0] == 0) { return false; }//特判0
    if (op_l.positive && !op_r.positive) { return false; }
    else if (!op_l.positive && op_r.positive) { return true; }
    else if (op_l.positive && op_r.positive) {
        if (op_l.len < op_r.len) { return true; }
        else if (op_l.len > op_r.len) { return false; }
        else {
            for (int i = op_l.len - 1; i >= 0; --i) {
                if (op_l.num[i] < op_r.num[i]) { return true; }
                else if (op_l.num[i] > op_r.num[i]) { return false; }
            }
        }
        return false;
    } else {
        if (op_l.len > op_r.len) { return true; }
        else if (op_l.len < op_r.len) { return false; }
        else {
            for (int i = op_l.len - 1; i >= 0; --i) {
                if (op_l.num[i] > op_r.num[i]) { return true; }
                else if (op_l.num[i] < op_r.num[i]) { return false; }
            }
        }
        return false;
    }
}

bool sjtu::operator>(const int2048 &op_l, const int2048 &op_r) {
    if (op_l.len == 1 && op_r.len == 1 &&
        op_l.num[0] == 0 && op_r.num[0] == 0) { return false; }//特判0
    if (op_l.positive && !op_r.positive) { return true; }
    else if (!op_l.positive && op_r.positive) { return false; }
    else if (op_l.positive && op_r.positive) {
        if (op_l.len > op_r.len) { return true; }
        else if (op_l.len < op_r.len) { return false; }
        else {
            for (int i = op_l.len - 1; i >= 0; --i) {
                if (op_l.num[i] > op_r.num[i]) { return true; }
                else if (op_l.num[i] < op_r.num[i]) { return false; }
            }
        }
        return false;
    } else {
        if (op_l.len < op_r.len) { return true; }
        else if (op_l.len > op_r.len) { return false; }
        else {
            for (int i = op_l.len - 1; i >= 0; --i) {
                if (op_l.num[i] < op_r.num[i]) { return true; }
                else if (op_l.num[i] > op_r.num[i]) { return false; }
            }
        }
        return false;
    }
}

bool sjtu::operator<=(const int2048 &op_l, const int2048 &op_r) {
    if (op_l.len == 1 && op_r.len == 1 &&
        op_l.num[0] == 0 && op_r.num[0] == 0) { return true; }//特判0
    if (op_l.positive && !op_r.positive) { return false; }
    else if (!op_l.positive && op_r.positive) { return true; }
    else if (op_l.positive && op_r.positive) {
        if (op_l.len < op_r.len) { return true; }
        else if (op_l.len > op_r.len) { return false; }
        else {
            for (int i = op_l.len - 1; i >= 0; --i) {
                if (op_l.num[i] < op_r.num[i]) { return true; }
                else if (op_l.num[i] > op_r.num[i]) { return false; }
            }
        }
        return true;
    } else {
        if (op_l.len > op_r.len) { return true; }
        else if (op_l.len < op_r.len) { return false; }
        else {
            for (int i = op_l.len - 1; i >= 0; --i) {
                if (op_l.num[i] > op_r.num[i]) { return true; }
                else if (op_l.num[i] < op_r.num[i]) { return false; }
            }
        }
        return true;
    }
}

bool sjtu::operator>=(const int2048 &op_l, const int2048 &op_r) {
    if (op_l.len == 1 && op_r.len == 1 &&
        op_l.num[0] == 0 && op_r.num[0] == 0) { return true; }//特判0
    if (op_l.positive && !op_r.positive) { return true; }
    else if (!op_l.positive && op_r.positive) { return false; }
    else if (op_l.positive && op_r.positive) {
        if (op_l.len > op_r.len) { return true; }
        else if (op_l.len < op_r.len) { return false; }
        else {
            for (int i = op_l.len - 1; i >= 0; --i) {
                if (op_l.num[i] > op_r.num[i]) { return true; }
                else if (op_l.num[i] < op_r.num[i]) { return false; }
            }
        }
        return true;
    } else {
        if (op_l.len < op_r.len) { return true; }
        else if (op_l.len > op_r.len) { return false; }
        else {
            for (int i = op_l.len - 1; i >= 0; --i) {
                if (op_l.num[i] < op_r.num[i]) { return true; }
                else if (op_l.num[i] > op_r.num[i]) { return false; }
            }
        }
        return true;
    }
}
//    int2048 op = opr;
//    if (!op.positive) { positive = !positive; }
//    if (len == 1 && op.len == 1) {
//        long long s = (long long) num[0] * op.num[0];
//        if (s >= condense) {
//            num[0] = s % condense;
//            num[1] = s / condense;
//            len = 2;
//        } else { num[0] = s; }
//        return *this;
//    }
//    if (len < op.len) {
//        if (op.len % 2 == 1) { ++op.len; }
//        len = op.len;
//    } else {
//        if (len % 2 == 1) { ++len; }
//        op.len = len;
//    }
//    int2048 a, c;
//    a.len = len / 2;
//    c.len = op.len / 2;
//    for (int i = 0; i < a.len; ++i) {
//        a.num[i] = num[i + a.len];
//        num[i + a.len] = 0;
//    }
//    for (int i = 0; i < c.len; ++i) {
//        c.num[i] = op.num[i + c.len];
//        op.num[i + c.len] = 0;
//    }
//    len /= 2;
//    op.len /= 2;
//    int2048 p1 = a * c, p3 = (*this) * op;
//    int2048 p2 = (a + (*this)) * (c + op);
//    for (int i = 0; i < len; ++i) {
//        p1.num[i + 2 * len] = p1.num[i];
//        p1.num[i] = 0;
//        p1.len += 2 * len;
//    }
//    for (int i = 0; i < len; ++i) {
//        p2.num[i + len] = p2.num[i];
//        p2.num[i] = 0;
//        p2.len += len;
//    }
//    *this = p1 + p2 + p3;
//    return *this;
