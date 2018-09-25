// Auto generated

#ifndef NEOVIM_CPP__NVIM_HPP_
#define NEOVIM_CPP__NVIM_HPP_

#include "nvim_rpc.hpp"

namespace nvim
{

class nvim_api
{

protected:
    NvimRPC client_;

public:

    void connect_tcp(const String &host,
            const String &service, double timeout_sec = 10000)
    {
        client_.connect_tcp(host, service, timeout_sec);
    }

{% for func in functions%}
    virtual {{func.return}} {{func.name}} ({% for arg in func.args %}{{arg.type}} {{arg.name}}{% if not loop.last %}, {% endif %}{% endfor %})
    {
        {% if func.return != "void" %}
        {{func.return}} res;
        client_.call("{{func.name}}", res{% for arg in func.args %}, {{arg.name}}{% endfor %});
        return res;
        {% else %}
        client_.call("{{func.name}}", nullptr{% for arg in func.args %}, {{arg.name}}{% endfor %});
        {% endif %}
    }
{% endfor %}

};

} //namespace nvim

#endif //NEOVIM_CPP__NVIM_HPP_
