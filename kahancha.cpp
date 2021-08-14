#include <iostream>
#include <vector>
#include <cmath>
#include <tuple>
#include <cstdio>
#include <filesystem>
#include <fstream>

float sum(float const *begin, float const *end)
{
    float s = 0;
    for (auto it = begin; it != end; ++it)
    {
        s += *it;
    }
    return s;
}

float with_double(float const *begin, float const *end)
{
    double s = 0;
    for (auto it = begin; it != end; ++it)
    {
        s += *it;
    }
    return static_cast<float>(s);
}

float kahan(float const *begin, float const *end)
{
    float sum = 0;
    float c = 0;
    for (auto it = begin; it != end; ++it)
    {
        auto y = *it - c;
        auto t = sum + y;
        c = (t - sum) - y;
        sum = t;
    }
    return sum;
}

float sum_of_parts(float const *begin, float const *end)
{
    auto len = end - begin;
    switch (len)
    {
    case 0:
        return 0;
    case 1:
        return *begin;
    default:
        break;
    }
    auto mid = begin + len / 2;
    return sum_of_parts(begin, mid) + sum_of_parts(mid, end);
}

using sumproc_t = float(float const *begin, float const *end);

void show(char const *name, sumproc_t *proc, float const *b, float const *e, float expected)
{
    float actual = proc(b, e);
    std::printf("%15s %16.10e %9.3e\n", name, actual, actual - expected);
}

void test(float expected, std::vector<float> const &data)
{
    auto b = data.data();
    auto e = b + data.size();
    show("sum", sum, b, e, expected);
    show("with_double", with_double, b, e, expected);
    show("sum_of_parts", sum_of_parts, b, e, expected);
    show("kahan", kahan, b, e, expected);
}

std::tuple<std::string, float, std::vector<float>>
read_file(char const *fn)
{
    auto size = std::filesystem::file_size(fn);
    auto count = size / sizeof(float);
    auto p = std::make_unique<float[]>(count);
    auto fs = std::ifstream(fn);
    fs.read(reinterpret_cast<char *>(p.get()), size);
    auto e = p[0];
    std::vector<float> data(p.get() + 1, p.get() + count);
    return {fn, e, data};
}

std::tuple<std::string, float, std::vector<float>>
create_data()
{
    std::vector<float> ones(1 << 25);
    for (auto &e : ones)
    {
        e = 1.0f;
    }
    return {"2**25 ones", 1 << 25, ones};
}

int main(int argc, char const *argv[])
{
    auto [title, expected, data] = argc == 2 ? read_file(argv[1]) : create_data();
    std::cout << title << std::endl;
    test(expected, data);
    return 0;
}