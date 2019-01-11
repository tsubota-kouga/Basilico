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
    String port;

    virtual void connect_tcp(const String &host,
            const String &service, double timeout_millsec = 10000)
    {
        client_.connect_tcp(host, service, timeout_millsec);
        port = service;
    }


    virtual Integer nvim_buf_line_count (Buffer buffer)
    {
        Integer res;
        client_.call("nvim_buf_line_count", res, buffer);
        return res;
        
    }

    virtual String buffer_get_line (Buffer buffer, Integer index)
    {
        String res;
        client_.call("buffer_get_line", res, buffer, index);
        return res;
        
    }

    virtual bool nvim_buf_attach (Buffer buffer, bool send_buffer, const Dictionary& opts)
    {
        bool res;
        client_.call("nvim_buf_attach", res, buffer, send_buffer, opts);
        return res;
        
    }

    virtual bool nvim_buf_detach (Buffer buffer)
    {
        bool res;
        client_.call("nvim_buf_detach", res, buffer);
        return res;
        
    }

    virtual void buffer_set_line (Buffer buffer, Integer index, const String& line)
    {
         client_.call("buffer_set_line", nullptr, buffer, index, line);
    }

    virtual void buffer_del_line (Buffer buffer, Integer index)
    {
         client_.call("buffer_del_line", nullptr, buffer, index);
    }

    virtual std::vector<String> buffer_get_line_slice (Buffer buffer, Integer start, Integer end, bool include_start, bool include_end)
    {
        std::vector<String> res;
        client_.call("buffer_get_line_slice", res, buffer, start, end, include_start, include_end);
        return res;
        
    }

    virtual std::vector<String> nvim_buf_get_lines (Buffer buffer, Integer start, Integer end, bool strict_indexing)
    {
        std::vector<String> res;
        client_.call("nvim_buf_get_lines", res, buffer, start, end, strict_indexing);
        return res;
        
    }

    virtual void buffer_set_line_slice (Buffer buffer, Integer start, Integer end, bool include_start, bool include_end, const std::vector<String>& replacement)
    {
         client_.call("buffer_set_line_slice", nullptr, buffer, start, end, include_start, include_end, replacement);
    }

    virtual void nvim_buf_set_lines (Buffer buffer, Integer start, Integer end, bool strict_indexing, const std::vector<String>& replacement)
    {
         client_.call("nvim_buf_set_lines", nullptr, buffer, start, end, strict_indexing, replacement);
    }

    virtual Integer nvim_buf_get_offset (Buffer buffer, Integer index)
    {
        Integer res;
        client_.call("nvim_buf_get_offset", res, buffer, index);
        return res;
        
    }

    virtual Object nvim_buf_get_var (Buffer buffer, const String& name)
    {
        Object res;
        client_.call("nvim_buf_get_var", res, buffer, name);
        return res;
        
    }

    virtual Integer nvim_buf_get_changedtick (Buffer buffer)
    {
        Integer res;
        client_.call("nvim_buf_get_changedtick", res, buffer);
        return res;
        
    }

    virtual std::vector<Dictionary> nvim_buf_get_keymap (Buffer buffer, const String& mode)
    {
        std::vector<Dictionary> res;
        client_.call("nvim_buf_get_keymap", res, buffer, mode);
        return res;
        
    }

    virtual Dictionary nvim_buf_get_commands (Buffer buffer, const Dictionary& opts)
    {
        Dictionary res;
        client_.call("nvim_buf_get_commands", res, buffer, opts);
        return res;
        
    }

    virtual void nvim_buf_set_var (Buffer buffer, const String& name, const Object& value)
    {
         client_.call("nvim_buf_set_var", nullptr, buffer, name, value);
    }

    virtual void nvim_buf_del_var (Buffer buffer, const String& name)
    {
         client_.call("nvim_buf_del_var", nullptr, buffer, name);
    }

    virtual Object buffer_set_var (Buffer buffer, const String& name, const Object& value)
    {
        Object res;
        client_.call("buffer_set_var", res, buffer, name, value);
        return res;
        
    }

    virtual Object buffer_del_var (Buffer buffer, const String& name)
    {
        Object res;
        client_.call("buffer_del_var", res, buffer, name);
        return res;
        
    }

    virtual Object nvim_buf_get_option (Buffer buffer, const String& name)
    {
        Object res;
        client_.call("nvim_buf_get_option", res, buffer, name);
        return res;
        
    }

    virtual void nvim_buf_set_option (Buffer buffer, const String& name, const Object& value)
    {
         client_.call("nvim_buf_set_option", nullptr, buffer, name, value);
    }

    virtual Integer nvim_buf_get_number (Buffer buffer)
    {
        Integer res;
        client_.call("nvim_buf_get_number", res, buffer);
        return res;
        
    }

    virtual String nvim_buf_get_name (Buffer buffer)
    {
        String res;
        client_.call("nvim_buf_get_name", res, buffer);
        return res;
        
    }

    virtual void nvim_buf_set_name (Buffer buffer, const String& name)
    {
         client_.call("nvim_buf_set_name", nullptr, buffer, name);
    }

    virtual bool nvim_buf_is_loaded (Buffer buffer)
    {
        bool res;
        client_.call("nvim_buf_is_loaded", res, buffer);
        return res;
        
    }

    virtual bool nvim_buf_is_valid (Buffer buffer)
    {
        bool res;
        client_.call("nvim_buf_is_valid", res, buffer);
        return res;
        
    }

    virtual void buffer_insert (Buffer buffer, Integer lnum, const std::vector<String>& lines)
    {
         client_.call("buffer_insert", nullptr, buffer, lnum, lines);
    }

    virtual std::vector<Integer> nvim_buf_get_mark (Buffer buffer, const String& name)
    {
        std::vector<Integer> res;
        client_.call("nvim_buf_get_mark", res, buffer, name);
        return res;
        
    }

    virtual Integer nvim_buf_add_highlight (Buffer buffer, Integer ns_id, const String& hl_group, Integer line, Integer col_start, Integer col_end)
    {
        Integer res;
        client_.call("nvim_buf_add_highlight", res, buffer, ns_id, hl_group, line, col_start, col_end);
        return res;
        
    }

    virtual void nvim_buf_clear_namespace (Buffer buffer, Integer ns_id, Integer line_start, Integer line_end)
    {
         client_.call("nvim_buf_clear_namespace", nullptr, buffer, ns_id, line_start, line_end);
    }

    virtual void nvim_buf_clear_highlight (Buffer buffer, Integer ns_id, Integer line_start, Integer line_end)
    {
         client_.call("nvim_buf_clear_highlight", nullptr, buffer, ns_id, line_start, line_end);
    }

    virtual Integer nvim_buf_set_virtual_text (Buffer buffer, Integer ns_id, Integer line, const Array& chunks, const Dictionary& opts)
    {
        Integer res;
        client_.call("nvim_buf_set_virtual_text", res, buffer, ns_id, line, chunks, opts);
        return res;
        
    }

    virtual std::vector<Window> nvim_tabpage_list_wins (Tabpage tabpage)
    {
        std::vector<Window> res;
        client_.call("nvim_tabpage_list_wins", res, tabpage);
        return res;
        
    }

    virtual Object nvim_tabpage_get_var (Tabpage tabpage, const String& name)
    {
        Object res;
        client_.call("nvim_tabpage_get_var", res, tabpage, name);
        return res;
        
    }

    virtual void nvim_tabpage_set_var (Tabpage tabpage, const String& name, const Object& value)
    {
         client_.call("nvim_tabpage_set_var", nullptr, tabpage, name, value);
    }

    virtual void nvim_tabpage_del_var (Tabpage tabpage, const String& name)
    {
         client_.call("nvim_tabpage_del_var", nullptr, tabpage, name);
    }

    virtual Object tabpage_set_var (Tabpage tabpage, const String& name, const Object& value)
    {
        Object res;
        client_.call("tabpage_set_var", res, tabpage, name, value);
        return res;
        
    }

    virtual Object tabpage_del_var (Tabpage tabpage, const String& name)
    {
        Object res;
        client_.call("tabpage_del_var", res, tabpage, name);
        return res;
        
    }

    virtual Window nvim_tabpage_get_win (Tabpage tabpage)
    {
        Window res;
        client_.call("nvim_tabpage_get_win", res, tabpage);
        return res;
        
    }

    virtual Integer nvim_tabpage_get_number (Tabpage tabpage)
    {
        Integer res;
        client_.call("nvim_tabpage_get_number", res, tabpage);
        return res;
        
    }

    virtual bool nvim_tabpage_is_valid (Tabpage tabpage)
    {
        bool res;
        client_.call("nvim_tabpage_is_valid", res, tabpage);
        return res;
        
    }

    virtual void nvim_command (const String& command)
    {
         client_.call("nvim_command", nullptr, command);
    }

    virtual Dictionary nvim_get_hl_by_name (const String& name, bool rgb)
    {
        Dictionary res;
        client_.call("nvim_get_hl_by_name", res, name, rgb);
        return res;
        
    }

    virtual Dictionary nvim_get_hl_by_id (Integer hl_id, bool rgb)
    {
        Dictionary res;
        client_.call("nvim_get_hl_by_id", res, hl_id, rgb);
        return res;
        
    }

    virtual void nvim_feedkeys (const String& keys, const String& mode, bool escape_csi)
    {
         client_.call("nvim_feedkeys", nullptr, keys, mode, escape_csi);
    }

    virtual Integer nvim_input (const String& keys)
    {
        Integer res;
        client_.call("nvim_input", res, keys);
        return res;
        
    }

    virtual String nvim_replace_termcodes (const String& str, bool from_part, bool do_lt, bool special)
    {
        String res;
        client_.call("nvim_replace_termcodes", res, str, from_part, do_lt, special);
        return res;
        
    }

    virtual String nvim_command_output (const String& command)
    {
        String res;
        client_.call("nvim_command_output", res, command);
        return res;
        
    }

    virtual Object nvim_eval (const String& expr)
    {
        Object res;
        client_.call("nvim_eval", res, expr);
        return res;
        
    }

    virtual Object nvim_execute_lua (const String& code, const Array& args)
    {
        Object res;
        client_.call("nvim_execute_lua", res, code, args);
        return res;
        
    }

    virtual Object nvim_call_function (const String& fn, const Array& args)
    {
        Object res;
        client_.call("nvim_call_function", res, fn, args);
        return res;
        
    }

    virtual Object nvim_call_dict_function (const Object& dict, const String& fn, const Array& args)
    {
        Object res;
        client_.call("nvim_call_dict_function", res, dict, fn, args);
        return res;
        
    }

    virtual Integer nvim_strwidth (const String& text)
    {
        Integer res;
        client_.call("nvim_strwidth", res, text);
        return res;
        
    }

    virtual std::vector<String> nvim_list_runtime_paths ()
    {
        std::vector<String> res;
        client_.call("nvim_list_runtime_paths", res);
        return res;
        
    }

    virtual void nvim_set_current_dir (const String& dir)
    {
         client_.call("nvim_set_current_dir", nullptr, dir);
    }

    virtual String nvim_get_current_line ()
    {
        String res;
        client_.call("nvim_get_current_line", res);
        return res;
        
    }

    virtual void nvim_set_current_line (const String& line)
    {
         client_.call("nvim_set_current_line", nullptr, line);
    }

    virtual void nvim_del_current_line ()
    {
         client_.call("nvim_del_current_line", nullptr);
    }

    virtual Object nvim_get_var (const String& name)
    {
        Object res;
        client_.call("nvim_get_var", res, name);
        return res;
        
    }

    virtual void nvim_set_var (const String& name, const Object& value)
    {
         client_.call("nvim_set_var", nullptr, name, value);
    }

    virtual void nvim_del_var (const String& name)
    {
         client_.call("nvim_del_var", nullptr, name);
    }

    virtual Object vim_set_var (const String& name, const Object& value)
    {
        Object res;
        client_.call("vim_set_var", res, name, value);
        return res;
        
    }

    virtual Object vim_del_var (const String& name)
    {
        Object res;
        client_.call("vim_del_var", res, name);
        return res;
        
    }

    virtual Object nvim_get_vvar (const String& name)
    {
        Object res;
        client_.call("nvim_get_vvar", res, name);
        return res;
        
    }

    virtual Object nvim_get_option (const String& name)
    {
        Object res;
        client_.call("nvim_get_option", res, name);
        return res;
        
    }

    virtual void nvim_set_option (const String& name, const Object& value)
    {
         client_.call("nvim_set_option", nullptr, name, value);
    }

    virtual void nvim_out_write (const String& str)
    {
         client_.call("nvim_out_write", nullptr, str);
    }

    virtual void nvim_err_write (const String& str)
    {
         client_.call("nvim_err_write", nullptr, str);
    }

    virtual void nvim_err_writeln (const String& str)
    {
         client_.call("nvim_err_writeln", nullptr, str);
    }

    virtual std::vector<Buffer> nvim_list_bufs ()
    {
        std::vector<Buffer> res;
        client_.call("nvim_list_bufs", res);
        return res;
        
    }

    virtual Buffer nvim_get_current_buf ()
    {
        Buffer res;
        client_.call("nvim_get_current_buf", res);
        return res;
        
    }

    virtual void nvim_set_current_buf (Buffer buffer)
    {
         client_.call("nvim_set_current_buf", nullptr, buffer);
    }

    virtual std::vector<Window> nvim_list_wins ()
    {
        std::vector<Window> res;
        client_.call("nvim_list_wins", res);
        return res;
        
    }

    virtual Window nvim_get_current_win ()
    {
        Window res;
        client_.call("nvim_get_current_win", res);
        return res;
        
    }

    virtual void nvim_set_current_win (Window window)
    {
         client_.call("nvim_set_current_win", nullptr, window);
    }

    virtual std::vector<Tabpage> nvim_list_tabpages ()
    {
        std::vector<Tabpage> res;
        client_.call("nvim_list_tabpages", res);
        return res;
        
    }

    virtual Tabpage nvim_get_current_tabpage ()
    {
        Tabpage res;
        client_.call("nvim_get_current_tabpage", res);
        return res;
        
    }

    virtual void nvim_set_current_tabpage (Tabpage tabpage)
    {
         client_.call("nvim_set_current_tabpage", nullptr, tabpage);
    }

    virtual Integer nvim_create_namespace (const String& name)
    {
        Integer res;
        client_.call("nvim_create_namespace", res, name);
        return res;
        
    }

    virtual Dictionary nvim_get_namespaces ()
    {
        Dictionary res;
        client_.call("nvim_get_namespaces", res);
        return res;
        
    }

    virtual Integer nvim_get_color_by_name (const String& name)
    {
        Integer res;
        client_.call("nvim_get_color_by_name", res, name);
        return res;
        
    }

    virtual Dictionary nvim_get_color_map ()
    {
        Dictionary res;
        client_.call("nvim_get_color_map", res);
        return res;
        
    }

    virtual Dictionary nvim_get_mode ()
    {
        Dictionary res;
        client_.call("nvim_get_mode", res);
        return res;
        
    }

    virtual std::vector<Dictionary> nvim_get_keymap (const String& mode)
    {
        std::vector<Dictionary> res;
        client_.call("nvim_get_keymap", res, mode);
        return res;
        
    }

    virtual Dictionary nvim_get_commands (const Dictionary& opts)
    {
        Dictionary res;
        client_.call("nvim_get_commands", res, opts);
        return res;
        
    }

    virtual Array nvim_get_api_info ()
    {
        Array res;
        client_.call("nvim_get_api_info", res);
        return res;
        
    }

    virtual void nvim_set_client_info (const String& name, const Dictionary& version, const String& type, const Dictionary& methods, const Dictionary& attributes)
    {
         client_.call("nvim_set_client_info", nullptr, name, version, type, methods, attributes);
    }

    virtual Dictionary nvim_get_chan_info (Integer chan)
    {
        Dictionary res;
        client_.call("nvim_get_chan_info", res, chan);
        return res;
        
    }

    virtual Array nvim_list_chans ()
    {
        Array res;
        client_.call("nvim_list_chans", res);
        return res;
        
    }

    virtual Array nvim_call_atomic (const Array& calls)
    {
        Array res;
        client_.call("nvim_call_atomic", res, calls);
        return res;
        
    }

    virtual Dictionary nvim_parse_expression (const String& expr, const String& flags, bool highlight)
    {
        Dictionary res;
        client_.call("nvim_parse_expression", res, expr, flags, highlight);
        return res;
        
    }

    virtual Array nvim_list_uis ()
    {
        Array res;
        client_.call("nvim_list_uis", res);
        return res;
        
    }

    virtual Array nvim_get_proc_children (Integer pid)
    {
        Array res;
        client_.call("nvim_get_proc_children", res, pid);
        return res;
        
    }

    virtual Object nvim_get_proc (Integer pid)
    {
        Object res;
        client_.call("nvim_get_proc", res, pid);
        return res;
        
    }

    virtual Buffer nvim_win_get_buf (Window window)
    {
        Buffer res;
        client_.call("nvim_win_get_buf", res, window);
        return res;
        
    }

    virtual void nvim_win_set_buf (Window window, Buffer buffer)
    {
         client_.call("nvim_win_set_buf", nullptr, window, buffer);
    }

    virtual std::vector<Integer> nvim_win_get_cursor (Window window)
    {
        std::vector<Integer> res;
        client_.call("nvim_win_get_cursor", res, window);
        return res;
        
    }

    virtual void nvim_win_set_cursor (Window window, const std::vector<Integer>& pos)
    {
         client_.call("nvim_win_set_cursor", nullptr, window, pos);
    }

    virtual Integer nvim_win_get_height (Window window)
    {
        Integer res;
        client_.call("nvim_win_get_height", res, window);
        return res;
        
    }

    virtual void nvim_win_set_height (Window window, Integer height)
    {
         client_.call("nvim_win_set_height", nullptr, window, height);
    }

    virtual Integer nvim_win_get_width (Window window)
    {
        Integer res;
        client_.call("nvim_win_get_width", res, window);
        return res;
        
    }

    virtual void nvim_win_set_width (Window window, Integer width)
    {
         client_.call("nvim_win_set_width", nullptr, window, width);
    }

    virtual Object nvim_win_get_var (Window window, const String& name)
    {
        Object res;
        client_.call("nvim_win_get_var", res, window, name);
        return res;
        
    }

    virtual void nvim_win_set_var (Window window, const String& name, const Object& value)
    {
         client_.call("nvim_win_set_var", nullptr, window, name, value);
    }

    virtual void nvim_win_del_var (Window window, const String& name)
    {
         client_.call("nvim_win_del_var", nullptr, window, name);
    }

    virtual Object window_set_var (Window window, const String& name, const Object& value)
    {
        Object res;
        client_.call("window_set_var", res, window, name, value);
        return res;
        
    }

    virtual Object window_del_var (Window window, const String& name)
    {
        Object res;
        client_.call("window_del_var", res, window, name);
        return res;
        
    }

    virtual Object nvim_win_get_option (Window window, const String& name)
    {
        Object res;
        client_.call("nvim_win_get_option", res, window, name);
        return res;
        
    }

    virtual void nvim_win_set_option (Window window, const String& name, const Object& value)
    {
         client_.call("nvim_win_set_option", nullptr, window, name, value);
    }

    virtual std::vector<Integer> nvim_win_get_position (Window window)
    {
        std::vector<Integer> res;
        client_.call("nvim_win_get_position", res, window);
        return res;
        
    }

    virtual Tabpage nvim_win_get_tabpage (Window window)
    {
        Tabpage res;
        client_.call("nvim_win_get_tabpage", res, window);
        return res;
        
    }

    virtual Integer nvim_win_get_number (Window window)
    {
        Integer res;
        client_.call("nvim_win_get_number", res, window);
        return res;
        
    }

    virtual bool nvim_win_is_valid (Window window)
    {
        bool res;
        client_.call("nvim_win_is_valid", res, window);
        return res;
        
    }

    virtual Integer buffer_line_count (Buffer buffer)
    {
        Integer res;
        client_.call("buffer_line_count", res, buffer);
        return res;
        
    }

    virtual std::vector<String> buffer_get_lines (Buffer buffer, Integer start, Integer end, bool strict_indexing)
    {
        std::vector<String> res;
        client_.call("buffer_get_lines", res, buffer, start, end, strict_indexing);
        return res;
        
    }

    virtual void buffer_set_lines (Buffer buffer, Integer start, Integer end, bool strict_indexing, const std::vector<String>& replacement)
    {
         client_.call("buffer_set_lines", nullptr, buffer, start, end, strict_indexing, replacement);
    }

    virtual Object buffer_get_var (Buffer buffer, const String& name)
    {
        Object res;
        client_.call("buffer_get_var", res, buffer, name);
        return res;
        
    }

    virtual Object buffer_get_option (Buffer buffer, const String& name)
    {
        Object res;
        client_.call("buffer_get_option", res, buffer, name);
        return res;
        
    }

    virtual void buffer_set_option (Buffer buffer, const String& name, const Object& value)
    {
         client_.call("buffer_set_option", nullptr, buffer, name, value);
    }

    virtual Integer buffer_get_number (Buffer buffer)
    {
        Integer res;
        client_.call("buffer_get_number", res, buffer);
        return res;
        
    }

    virtual String buffer_get_name (Buffer buffer)
    {
        String res;
        client_.call("buffer_get_name", res, buffer);
        return res;
        
    }

    virtual void buffer_set_name (Buffer buffer, const String& name)
    {
         client_.call("buffer_set_name", nullptr, buffer, name);
    }

    virtual bool buffer_is_valid (Buffer buffer)
    {
        bool res;
        client_.call("buffer_is_valid", res, buffer);
        return res;
        
    }

    virtual std::vector<Integer> buffer_get_mark (Buffer buffer, const String& name)
    {
        std::vector<Integer> res;
        client_.call("buffer_get_mark", res, buffer, name);
        return res;
        
    }

    virtual Integer buffer_add_highlight (Buffer buffer, Integer ns_id, const String& hl_group, Integer line, Integer col_start, Integer col_end)
    {
        Integer res;
        client_.call("buffer_add_highlight", res, buffer, ns_id, hl_group, line, col_start, col_end);
        return res;
        
    }

    virtual void buffer_clear_highlight (Buffer buffer, Integer ns_id, Integer line_start, Integer line_end)
    {
         client_.call("buffer_clear_highlight", nullptr, buffer, ns_id, line_start, line_end);
    }

    virtual std::vector<Window> tabpage_get_windows (Tabpage tabpage)
    {
        std::vector<Window> res;
        client_.call("tabpage_get_windows", res, tabpage);
        return res;
        
    }

    virtual Object tabpage_get_var (Tabpage tabpage, const String& name)
    {
        Object res;
        client_.call("tabpage_get_var", res, tabpage, name);
        return res;
        
    }

    virtual Window tabpage_get_window (Tabpage tabpage)
    {
        Window res;
        client_.call("tabpage_get_window", res, tabpage);
        return res;
        
    }

    virtual bool tabpage_is_valid (Tabpage tabpage)
    {
        bool res;
        client_.call("tabpage_is_valid", res, tabpage);
        return res;
        
    }

    virtual void vim_command (const String& command)
    {
         client_.call("vim_command", nullptr, command);
    }

    virtual void vim_feedkeys (const String& keys, const String& mode, bool escape_csi)
    {
         client_.call("vim_feedkeys", nullptr, keys, mode, escape_csi);
    }

    virtual Integer vim_input (const String& keys)
    {
        Integer res;
        client_.call("vim_input", res, keys);
        return res;
        
    }

    virtual String vim_replace_termcodes (const String& str, bool from_part, bool do_lt, bool special)
    {
        String res;
        client_.call("vim_replace_termcodes", res, str, from_part, do_lt, special);
        return res;
        
    }

    virtual String vim_command_output (const String& command)
    {
        String res;
        client_.call("vim_command_output", res, command);
        return res;
        
    }

    virtual Object vim_eval (const String& expr)
    {
        Object res;
        client_.call("vim_eval", res, expr);
        return res;
        
    }

    virtual Object vim_call_function (const String& fn, const Array& args)
    {
        Object res;
        client_.call("vim_call_function", res, fn, args);
        return res;
        
    }

    virtual Integer vim_strwidth (const String& text)
    {
        Integer res;
        client_.call("vim_strwidth", res, text);
        return res;
        
    }

    virtual std::vector<String> vim_list_runtime_paths ()
    {
        std::vector<String> res;
        client_.call("vim_list_runtime_paths", res);
        return res;
        
    }

    virtual void vim_change_directory (const String& dir)
    {
         client_.call("vim_change_directory", nullptr, dir);
    }

    virtual String vim_get_current_line ()
    {
        String res;
        client_.call("vim_get_current_line", res);
        return res;
        
    }

    virtual void vim_set_current_line (const String& line)
    {
         client_.call("vim_set_current_line", nullptr, line);
    }

    virtual void vim_del_current_line ()
    {
         client_.call("vim_del_current_line", nullptr);
    }

    virtual Object vim_get_var (const String& name)
    {
        Object res;
        client_.call("vim_get_var", res, name);
        return res;
        
    }

    virtual Object vim_get_vvar (const String& name)
    {
        Object res;
        client_.call("vim_get_vvar", res, name);
        return res;
        
    }

    virtual Object vim_get_option (const String& name)
    {
        Object res;
        client_.call("vim_get_option", res, name);
        return res;
        
    }

    virtual void vim_set_option (const String& name, const Object& value)
    {
         client_.call("vim_set_option", nullptr, name, value);
    }

    virtual void vim_out_write (const String& str)
    {
         client_.call("vim_out_write", nullptr, str);
    }

    virtual void vim_err_write (const String& str)
    {
         client_.call("vim_err_write", nullptr, str);
    }

    virtual void vim_report_error (const String& str)
    {
         client_.call("vim_report_error", nullptr, str);
    }

    virtual std::vector<Buffer> vim_get_buffers ()
    {
        std::vector<Buffer> res;
        client_.call("vim_get_buffers", res);
        return res;
        
    }

    virtual Buffer vim_get_current_buffer ()
    {
        Buffer res;
        client_.call("vim_get_current_buffer", res);
        return res;
        
    }

    virtual void vim_set_current_buffer (Buffer buffer)
    {
         client_.call("vim_set_current_buffer", nullptr, buffer);
    }

    virtual std::vector<Window> vim_get_windows ()
    {
        std::vector<Window> res;
        client_.call("vim_get_windows", res);
        return res;
        
    }

    virtual Window vim_get_current_window ()
    {
        Window res;
        client_.call("vim_get_current_window", res);
        return res;
        
    }

    virtual void vim_set_current_window (Window window)
    {
         client_.call("vim_set_current_window", nullptr, window);
    }

    virtual std::vector<Tabpage> vim_get_tabpages ()
    {
        std::vector<Tabpage> res;
        client_.call("vim_get_tabpages", res);
        return res;
        
    }

    virtual Tabpage vim_get_current_tabpage ()
    {
        Tabpage res;
        client_.call("vim_get_current_tabpage", res);
        return res;
        
    }

    virtual void vim_set_current_tabpage (Tabpage tabpage)
    {
         client_.call("vim_set_current_tabpage", nullptr, tabpage);
    }

    virtual Integer vim_name_to_color (const String& name)
    {
        Integer res;
        client_.call("vim_name_to_color", res, name);
        return res;
        
    }

    virtual Dictionary vim_get_color_map ()
    {
        Dictionary res;
        client_.call("vim_get_color_map", res);
        return res;
        
    }

    virtual Array vim_get_api_info ()
    {
        Array res;
        client_.call("vim_get_api_info", res);
        return res;
        
    }

    virtual Buffer window_get_buffer (Window window)
    {
        Buffer res;
        client_.call("window_get_buffer", res, window);
        return res;
        
    }

    virtual std::vector<Integer> window_get_cursor (Window window)
    {
        std::vector<Integer> res;
        client_.call("window_get_cursor", res, window);
        return res;
        
    }

    virtual void window_set_cursor (Window window, const std::vector<Integer>& pos)
    {
         client_.call("window_set_cursor", nullptr, window, pos);
    }

    virtual Integer window_get_height (Window window)
    {
        Integer res;
        client_.call("window_get_height", res, window);
        return res;
        
    }

    virtual void window_set_height (Window window, Integer height)
    {
         client_.call("window_set_height", nullptr, window, height);
    }

    virtual Integer window_get_width (Window window)
    {
        Integer res;
        client_.call("window_get_width", res, window);
        return res;
        
    }

    virtual void window_set_width (Window window, Integer width)
    {
         client_.call("window_set_width", nullptr, window, width);
    }

    virtual Object window_get_var (Window window, const String& name)
    {
        Object res;
        client_.call("window_get_var", res, window, name);
        return res;
        
    }

    virtual Object window_get_option (Window window, const String& name)
    {
        Object res;
        client_.call("window_get_option", res, window, name);
        return res;
        
    }

    virtual void window_set_option (Window window, const String& name, const Object& value)
    {
         client_.call("window_set_option", nullptr, window, name, value);
    }

    virtual std::vector<Integer> window_get_position (Window window)
    {
        std::vector<Integer> res;
        client_.call("window_get_position", res, window);
        return res;
        
    }

    virtual Tabpage window_get_tabpage (Window window)
    {
        Tabpage res;
        client_.call("window_get_tabpage", res, window);
        return res;
        
    }

    virtual bool window_is_valid (Window window)
    {
        bool res;
        client_.call("window_is_valid", res, window);
        return res;
        
    }


    Object read_request(double timeout_millisec)
    {
        if(client_.available())
        {
            return client_.read_info(timeout_millisec);
        }
        else
        {
            return Object{};
        }
    }
};

} //namespace nvim

#endif //NEOVIM_CPP__NVIM_HPP_