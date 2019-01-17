
// You can use func_name:String, info:Array->[func_name, arg1, arg2, ...]
{% for name in message['plugin_name'] %}
{% if not loop.first %}else {% endif %}if(plugin_name == "{{name}}")
{
    if(args.size() > 1 and boost::get<String>(args.at(1)) == "factory")
    {
        auto tmp = {{name}}::factory(this, args);
        if(tmp != nullptr)
        {
            addPlugin("{{name}}", tmp);
        }
    }
    else
    {
        if(Plugins.count("{{name}}") > 0)
        {
            for(auto plugin: Plugins.at("{{name}}"))
            {
                plugin->execute(this, args);
            }
        }
    }
}
{% endfor %}

