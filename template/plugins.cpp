
// You can use func_name:String, info:Array->[func_name, arg1, arg2, ...]
{% for name in message['plugin_name'] %}
{% if not loop.first %}else {% endif %}if(plugin_name == "{{name}}")
{
    if(args.size() > 1 and boost::get<String>(args.at(1)) == "factory")
    {
        // neovim.read_request(100);
        neovim.set_input_control_flag(true);
        neovim.nvim_input("<Esc>");
        auto&& [tmp, option] = {{name}}::factory(this, args);

        if(tmp != nullptr)
        {
            if(option == "dock")
            {
                addPlugin("{{name}}", tmp);
            }
            else if(option == "tab" and neovim.get_ui_option("ext_tabline"))
            {
                auto&& tab = makeTabForPlugin("{{name}}");
                addTabPluginId(tmp, "{{name}}", tab);
                addPlugin("{{name}}", tmp);
            }
            else if(option == "split")
            {
                auto&& tab = neovim.nvim_get_current_tabpage();
                neovim_layout.addWidget(tmp, 1, 2);
                auto&& [r, c, w, h] = tmp->splitPluginPosition(tab);
                SplitPlugins.emplace(tab, std::make_tuple(tmp, r, c, w, h));
                addPlugin("{{name}}", tmp);
            }
            else if(option == "others"){}
            else
            {
                std::cerr << "ERROR: There is no option "
                    << option << std::endl;
                delete tmp;
            }
        }
        neovim.set_input_control_flag(false);
    }
    else
    {
        if(Plugins.count("{{name}}") > 0)
        {
            for(auto&& plugin: Plugins.at("{{name}}"))
            {
                plugin->execute(this, args);
            }
        }
    }
}
{% if loop.last %}
else  // autocmd
{
    for(auto& [_, plugins]: Plugins)
    {
        for(auto plugin:plugins)
        {
            plugin->autocmdExecute(this, boost::get<String>(args.at(0)));
        }
    }
}
{% endif %}
{% endfor %}

