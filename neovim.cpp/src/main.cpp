#include <iostream>
#include "neovim.hpp"

using std::cout;
using std::wcout;
using std::endl;

int main(int argc, char* argv[])
{
    neovim nvim(100, 55, {{"rgb", true}, {"ext_newgrid", true}});
    nvim.connect_tcp("localhost", "6666");
    nvim.nvim_ui_attach();

    nvim.nvim_eval("\( 3 + 2 \) \* 4");
    nvim.nvim_input("<Esc>");
    nvim.draw();
    nvim.nvim_input("<Right>");
    nvim.draw();
    nvim.nvim_input("a");
    nvim.draw();
    nvim.nvim_input("in");
    nvim.draw();
    nvim.nvim_input("n");
    nvim.draw();
    nvim.nvim_input("n");
    nvim.draw();

    // nvim.nvim_get_current_line();
    nvim.draw();
    for(auto x:nvim.nvim_screen)
    {
        for(auto y:x)
        {
            cout << y;
        }
        cout << endl;
    }
}

