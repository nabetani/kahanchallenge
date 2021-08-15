#include <iostream>
#include <vector>
#include <cmath>
#include <tuple>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <algorithm>

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

float sort_kahan(float const *begin, float const *end)
{
    std::vector<float> v(begin, end);
    std::sort(v.rbegin(), v.rend());
    return kahan(v.data(), v.data() + v.size());
}

using sumproc_t = float(float const *begin, float const *end);

void show(char const *name, sumproc_t *proc, std::vector<float> const &expected, std::vector<float> const &data)
{
    auto size = data.size();
    double sum2 = 0;
    auto b = data.data();
    auto skip = size / 10;
    auto count = data.size() - skip;
    for (size_t ix = skip; ix < size; ++ix)
    {
        auto sum = proc(b, b + ix + 1);
        auto diff = (sum - expected[ix]) / expected[ix];
        sum2 += diff * diff;
    }
    auto stdev = std::sqrt(sum2 / count);
    std::printf("%15s %16.10e\n", name, stdev);
}

void test(std::vector<float> const &expected, std::vector<float> const &data)
{
    auto b = data.data();
    auto e = b + data.size();
    show("sum", sum, expected, data);
    show("with_double", with_double, expected, data);
    show("sum_of_parts", sum_of_parts, expected, data);
    show("kahan", kahan, expected, data);
    show("sort_kahan", sort_kahan, expected, data);
}

std::tuple<std::vector<float>, std::vector<float>>
read_file(char const *fn)
{
    auto size = std::filesystem::file_size(fn);
    auto count = size / sizeof(float) / 2;
    auto p = std::make_unique<float[]>(count * 2);
    auto fs = std::ifstream(fn);
    fs.read(reinterpret_cast<char *>(p.get()), size);
    std::vector<float> e(p.get(), p.get() + count);
    std::vector<float> d(p.get() + count, p.get() + count * 2);
    return {e, d};
}

std::tuple<std::vector<float>, std::vector<float>>
create_data()
{
    size_t count = 1 << 25;
    std::vector<float> data(count);
    std::vector<float> expected(count);
    for (size_t ix = 0; ix < count; ++ix)
    {
        data[ix] = 1;
        expected[ix] = ix + 1;
    }
    return {expected, data};
}

int main(int argc, char const *argv[])
{
    auto [expected, data] = argc == 2 ? read_file(argv[1]) : create_data();
    test(expected, data);
    return 0;
}