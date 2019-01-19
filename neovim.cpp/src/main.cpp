#include <iostream>
#include "neovim.hpp"

using std::cout;
using std::wcout;
using std::endl;

int main(int argc, char* argv[])
{
    neovim nvim(100, 55, {{"rgb", true}, {"ext_linegrid", true}});
    nvim.connect_tcp("localhost", "6666");
    auto buf = nvim.nvim_get_current_buf();
    nvim.nvim_buf_attach(buf, "nvim_changetick_event", {});

    // nvim.nvim_eval("\( 3 + 2 \) \* 4");
    // nvim.nvim_input("<Esc>");
    // nvim.operation();
    // nvim.nvim_input("<Right>");
    // nvim.operation();
    // nvim.nvim_input("a");
    // nvim.operation();
    // nvim.nvim_input("in");
    // nvim.operation();
    // nvim.nvim_input("n");
    // nvim.operation();
    // nvim.nvim_input("n");
    nvim.operation();

    // nvim.nvim_get_current_line();
    // nvim.operation();
    // for(auto x:nvim.nvim_screen)
    // {
    //     cout << x << endl;
    //     cout << endl;
    // }
}

