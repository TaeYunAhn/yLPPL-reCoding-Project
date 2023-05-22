#inlcude <cmath>
#include <vector>

using Matrix = std::vector<std::vector<double>>;  // C++ 14

bool EQ0(const double val)
{
    return abs(val) <= FLT_EPSILON;
}

bool EQ(const double val1, const double val2)
{
    return abs(val1 - val2) <= FLT_EPSILON;
}

bool EQVEC(const std::vector<double> &v1, const std::vector<double> &v2)
{
    if (v1.size() != v2.size())
        return false;

    const auto &size = v1.size();
    for (int i = 0; i < size; i++)
    {
        if (!EQ(v1[i], v2[i]))
            return false;
    }

    return true;
}

double SAFE_DIV(const double& val1, const double& val2)
{
    if (EQ0(val2))
        return 0.0;

    return val1 / val2;
}