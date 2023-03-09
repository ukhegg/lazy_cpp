//
// Created by Zeliukin Nick on 09.03.23.
//

#include <lazy_cpp/lazy_t.hpp>

int main(int argc, char *argv[])
{
    auto t = lazy_cpp::lazy_from_value(5);
    return t.get() == 5 ? 0 : -1;
}