#include <iostream>
#include <fstream>
#include <vector>
#include <cctype>

using VEC_LL = std::vector<long long>;

static void get_history(std::string &line, VEC_LL &h)
{
    bool is_negative = false;
    long long num = 0;
    for (const auto c : line)
    {
        if (isdigit(c)) {
            num *= 10;
            num += c - '0';
        }
        else if (c == '-') {
            is_negative = true;
        }
        else {
            if (is_negative) {
                num *= -1;
                is_negative = false;
            }
            h.push_back(num);
            num = 0;
        }
    }

    if (is_negative)
        num *= -1;

    h.push_back(num);

    for (auto &n : h) {
        std::cout << n << " ";
    }
    std::cout << std::endl;
}

static void generate_diff_vectors(const VEC_LL &h, std::vector<VEC_LL> &diffs)
{
    const VEC_LL *in = &h;
    diffs.push_back({});
    bool exit = false;
    while (!exit) {
        unsigned int zeros = 0;
        for (unsigned int i = 0; i < in->size() - 1; i++) {
            long long diff = in->at(i + 1) - in->at(i);
            if (diff == 0) {
                zeros++;
            }
            diffs[diffs.size() - 1].push_back(diff);
        }

        if (zeros == in->size() - 1) {
            exit = true;
        }
        else {
            diffs.push_back({});
            in = &diffs[diffs.size() - 2];
        }
    }

    for (const auto &v : diffs) {
        for (const auto &n : v) {
            std::cout << n << " ";
        }
        std::cout << std::endl;
    }

}

static void extrapolate_values(VEC_LL &in, std::vector<VEC_LL> &diffs)
{
    long long new_val;
    diffs[diffs.size() - 1].push_back(0);
    for (unsigned int i = diffs.size() - 1; i > 0; --i)
    {
        new_val = diffs[i].back() + diffs[i - 1].back();
        diffs[i - 1].push_back(new_val);
    }
    new_val = in.back() + diffs[0].back();
    in.push_back(new_val);

    for (const auto &v : diffs) {
        for (const auto &n : v) {
            std::cout << n << " ";
        }
        std::cout << std::endl;
    }

    for (const auto &n : in) {
        std::cout << n << " ";
    }
    std::cout << std::endl;
}

static long long predict_next_val(VEC_LL &h)
{
    std::vector<VEC_LL> diffs;
    generate_diff_vectors(h, diffs);
    extrapolate_values(h, diffs);
    return h.back();
}

int main(const int argc, const char * argv[])
{
    std::fstream new_file; 
    std::string line;
    std::string input;
    if (argc != 2)
    {
        std::cerr << "Invalid arguments!" << std::endl;
        exit(EXIT_FAILURE);
    }

    new_file.open(argv[1], std::ios::in);

    if (!new_file.is_open())
    {
        std::cerr << "Could not open " << argv[1] << std::endl;
        exit(EXIT_FAILURE);
    }

    long long sum = 0;
    while (std::getline(new_file, line))
    {
        VEC_LL his;
        get_history(line, his);
        sum += predict_next_val(his);
    }
    std::cout << "Sum = " << sum << std::endl;
    return 0;
}

