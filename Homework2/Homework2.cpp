#include <iostream>
#include <vector>
#include <chrono>

class Polynomial {
private:
    std::vector<std::pair<float, int>> termArray;

public:
    Polynomial();
    Polynomial Add(const Polynomial& poly) const;
    Polynomial Mult(const Polynomial& poly) const;
    float Eval(float x) const;

    friend std::ostream& operator<<(std::ostream& os, const Polynomial& poly);
    friend std::istream& operator>>(std::istream& is, Polynomial& poly);
};

Polynomial::Polynomial() {

    termArray.push_back({ 0.0, 0 });
}

Polynomial Polynomial::Add(const Polynomial& poly) const {
    Polynomial result;

    auto it1 = termArray.begin();
    auto it2 = poly.termArray.begin();

    while (it1 != termArray.end() && it2 != poly.termArray.end()) {
        if (it1->second == it2->second) {
            result.termArray.push_back({ it1->first + it2->first, it1->second });
            ++it1;
            ++it2;
        }
        else if (it1->second < it2->second) {
            result.termArray.push_back(*it1);
            ++it1;
        }
        else {
            result.termArray.push_back(*it2);
            ++it2;
        }
    }

    while (it1 != termArray.end()) {
        result.termArray.push_back(*it1);
        ++it1;
    }

    while (it2 != poly.termArray.end()) {
        result.termArray.push_back(*it2);
        ++it2;
    }

    return result;
}

Polynomial Polynomial::Mult(const Polynomial& poly) const {
    Polynomial result;

    for (const auto& term1 : termArray) {
        for (const auto& term2 : poly.termArray) {
            result.termArray.push_back({ term1.first * term2.first, term1.second + term2.second });
        }
    }

    for (auto it = result.termArray.begin(); it != result.termArray.end(); ++it) {
        for (auto it2 = it + 1; it2 != result.termArray.end();) {
            if (it->second == it2->second) {
                it->first += it2->first;
                it2 = result.termArray.erase(it2);
            }
            else {
                ++it2;
            }
        }
    }

    return result;
}

float Polynomial::Eval(float x) const {
    float result = 0.0;
    for (const auto& term : termArray) {
        result += term.first * pow(x, term.second);
    }
    return result;
}
std::ostream& operator<<(std::ostream& os, const Polynomial& poly) {
    for (const auto& term : poly.termArray) {
        os << term.first << "x^" << term.second << " + ";
    }
    return os;
}

std::istream& operator>>(std::istream& is, Polynomial& poly) {
    
    std::cout << "輸入多項式的項數: ";
    int numTerms;
    is >> numTerms;

    poly.termArray.clear(); 

    for (int i = 0; i < numTerms; ++i) {
        float coefficient;
        int exponent;
        std::cout << "輸入多項式的第 " << i + 1 << "個係數跟次方: ";
        is >> coefficient >> exponent;
        poly.termArray.push_back({ coefficient, exponent });
    }

    return is;
}

int main() {
    Polynomial p1, p2;

    std::cout << "第一個多項式:\n";
    std::cin >> p1;

    std::cout << "第二個多項式:\n";
    std::cin >> p2;

    Polynomial sum = p1.Add(p2);
    Polynomial product = p1.Mult(p2);

    std::cout << "多項式相加:: " << sum << std::endl;
    std::cout << "多項式相乘: " << product << std::endl;

    float x;

    std::cout << "輸入x值計算多項式: ";
    std::cin >> x;

    std::cout << "多項式相加後代入x值: " << sum.Eval(x) << std::endl;
    std::cout << "多項式相乘後帶入x值: " << product.Eval(x) << std::endl;

    auto startAdd = std::chrono::high_resolution_clock::now();
    Polynomial sumPerformance = p1.Add(p2);
    auto endAdd = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> durationAdd = endAdd - startAdd;

    std::cout << "Add operation took " << durationAdd.count() << " seconds.\n";

    auto startMult = std::chrono::high_resolution_clock::now();
    Polynomial productPerformance = p1.Mult(p2);
    auto endMult = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> durationMult = endMult - startMult;

    std::cout << "Mult operation took " << durationMult.count() << " seconds.\n";

    return 0;
}
