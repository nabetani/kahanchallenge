#include <iostream>
#include <vector>
#include <cmath>

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

void show( char const * name, sumproc_t * proc, float const * b, float const * e, float expected)
{
    float actual = proc(b,e);
    std::printf( "%15s %16.10e %9.3e\n", name, actual, actual-expected);
}

int main(int argc, char const *argv[])
{
    std::vector<float> d(1<<25);
    for (size_t i = 0; i < d.size(); ++i)
    {
        d[i] = 1;
    }
    auto b = d.data();
    auto e = b + d.size();
    auto expected = with_double(b,e);
    show( "sum", sum, b, e ,expected);
    show( "with_double", with_double, b, e ,expected);
    show( "sum_of_parts", sum_of_parts, b, e ,expected);
    show( "kahan", kahan, b, e ,expected);
    return 0;
}