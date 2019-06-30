#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include "boost/variant.hpp"
#include "boost/tokenizer.hpp"
#include "boost/format.hpp"
#include "boost/lexical_cast.hpp"

struct Add
{
    int32_t compute(int32_t l, int32_t r, bool print_it) const
    {
        if (print_it) {
            std::cout << "adding " << l << " + "
                      << " r" << std::endl;
        }
        return l + r;
    }
};

struct Sub
{
    int32_t compute(int32_t l, int32_t r) const { return l - r; }
};

struct Mul
{
    int32_t compute(int32_t l, int32_t r) const { return l * r; }
};

class op_visitor : public boost::static_visitor<int32_t>
{
public:
    op_visitor(int32_t l, int32_t r) : l_(l), r_(r) {}

    /**
     * Visitor to handle the `Add` case which requires an extra argument
     */
    int32_t operator()(Add& op) { return op.compute(l_, r_, false); }

    /**
     * Generic visitor to handle the case where we just pass in the 2 arguments
     */
    template <typename T> int32_t operator()(T& op) const { return op.compute(l_, r_); }

private:
    int32_t l_;
    int32_t r_;
};

class Calc
{
public:
    /**
     * Input should be a string where you have a number followed by an operator.
     * Last input must be a number.
     */
    Calc(std::string input)
    {
        boost::char_separator<char> sep(" ");
        auto tok = boost::tokenizer<boost::char_separator<char>>(input, sep);
        auto expect_operator = false;
        auto first = true;
        for (auto beg = tok.begin(); beg != tok.end(); ++beg) {
            std::cout << *beg << " " << std::flush;
            if (first) {
                result_ = boost::lexical_cast<int32_t>(*beg);
                expect_operator = true;
                first = false;
                continue;
            }
            if (expect_operator) {
                operations_.push(op_factory(*beg));
                expect_operator = false;
                continue;
            } else {
                values_.push(boost::lexical_cast<int32_t>(*beg));
                expect_operator = true;
            }
        }
        if (!expect_operator) {
            throw std::runtime_error("Cannot end on an operator");
        }
    }

    bool empty() { return values_.empty(); }

    int32_t compute_next()
    {
        if (values_.empty())
            return result_;

        auto op = operations_.front();
        operations_.pop();

        auto value = values_.front();
        values_.pop();

        auto v = op_visitor(result_, value);
        result_ = boost::apply_visitor(v, op);
    }

    int32_t compute_all()
    {
        while (!values_.empty()) {
            compute_next();
        }
        return result_;
    }

private:
    using Operation = boost::variant<Add, Sub, Mul>;
    std::queue<Operation> operations_;
    std::queue<int32_t> values_;
    int32_t result_ = 0;

    Operation op_factory(const std::string& op)
    {
        if (op == "+") {
            return Add{};
        } else if (op == "-") {
            return Sub{};
        } else if (op == "*") {
            return Mul{};
        }
        throw std::runtime_error{boost::str(boost::format("\nUnknown Operator %1%") % op)};
    }
};

void print_help()
{
    std::cout << "Simple calculator.\n"
              << "Each string argument will perform the operations starting from left to right.\n"
              << "Supported operations include addition (+), subtraction (-), and multplication (*).\n"
              << "For example: `vbp \"1 + 3 - 2 * 4\"` will output `calc is 8`" << std::endl;
}

int main(int argc, char* argv[])
{
    if (argc < 2) {
        std::cerr << "ERROR: Missing argument" << std::endl;
        print_help();
        return 1;
    }

    for (int i = 1; i < argc; ++i) {
        std::cout << "calculating arg " << i - 1 << " : ";
        auto c = Calc(argv[i]);
        std::cout << "\ncalc is " << c.compute_all() << std::endl;
    }
    return 0;
}
