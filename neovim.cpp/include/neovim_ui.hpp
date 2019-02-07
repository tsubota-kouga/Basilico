#ifndef NEOVIM_CPP__NVIM_UI_HPP
#define NEOVIM_CPP__NVIM_UI_HPP

#include "nvim_rpc.hpp"

namespace nvim
{

class nvim_ui
{

protected:
    NvimRPC ui_client_;

public:
    virtual void connect_tcp_ui(const String& host, const String& service, long timeout_sec=1000)
    {
        ui_client_.connect_tcp(host, service, timeout_sec);
    }

    virtual void nvim_ui_attach (Integer width, Integer height, const Dictionary& options)
    {
        ui_client_.call("nvim_ui_attach", nullptr, width, height, options);
    }

    [[deprecated("Please use nvim_ui_attach instead of ui_attach")]]
    virtual void ui_attach (Integer width, Integer height, bool enable_rgb)
    {
        ui_client_.call("ui_attach", nullptr, width, height, enable_rgb);
    }

    virtual void nvim_ui_detach ()
    {
        ui_client_.call("nvim_ui_detach", nullptr);
    }

    virtual void nvim_ui_try_resize (Integer width, Integer height)
    {
        ui_client_.call("nvim_ui_try_resize", nullptr, width, height);
    }

    virtual void nvim_ui_try_resize_grid(Integer grid, Integer width, Integer height)
    {
        ui_client_.call("nvim_ui_try_resize_grid", nullptr, grid, width, height);
    }

    virtual void nvim_ui_set_option (const String& name, const Object& value)
    {
        ui_client_.call("nvim_ui_set_option", nullptr, name, value);
    }

    virtual void nvim_subscribe (const String& event)
    {
        ui_client_.call("nvim_subscribe", nullptr, event);
    }

    virtual void nvim_unsubscribe (const String& event)
    {
        ui_client_.call("nvim_unsubscribe", nullptr, event);
    }

    virtual void nvim_buf_attach (Buffer buffer, bool send_buffer, const Dictionary& opts)
    {
        ui_client_.call("nvim_buf_attach", nullptr, buffer, send_buffer, opts);
    }

    virtual void nvim_buf_detach (Buffer buffer)
    {
        ui_client_.call("nvim_buf_detach", nullptr, buffer);
    }

    std::size_t available(){ return ui_client_.available(); }

};

}


#endif
