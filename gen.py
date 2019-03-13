
from jinja2 import Template, Environment, FileSystemLoader
from pynvim import attach
from glob import glob
import os, sys, re
import toml

fill = {}
this_dir = os.path.abspath(os.path.dirname(__file__))
env = Environment(loader=FileSystemLoader(this_dir  + '/template'))

# no gui plugins
if len(sys.argv) >= 3 and sys.argv[2] == 'np':
    # dependency for Basilico
    fill['qt_version'] = 5.7
    fill['boost_version'] = 1.55
    fill['qt_dependency'] = ['Widgets', 'Core']
    fill['boost_dependency'] = ['system', 'thread']

else:
    if len(sys.argv) >= 2:
        path = sys.argv[1]
    else:
        print('argment 3: path to socket is required')
        sys.exit(1)

    nvim = attach('socket', path=path)

    basil_file_name = 'basil.toml'

    fill['qt_version'] = 5.7
    fill['boost_version'] = 1.55
    fill['qt_dependency'] = ['Widgets', 'Core']
    fill['boost_dependency'] = ['system', 'thread']
    fill['plugin_paths'] = []
    fill['plugin_name'] = []
    fill['autocmd'] = []
    fill['resource'] = []

# array
    runtimepath = nvim.eval('&runtimepath').split(',')
    for path in runtimepath:
        basil_path = glob(os.path.join(path, basil_file_name))
        if basil_path != [] and path != '.':
            head, tail = os.path.split(path)
            if tail != 'Basilico':
                fill['plugin_paths'].append(path)
                fill['plugin_name'].append(tail)

    for p, name in zip(fill['plugin_paths'], fill['plugin_name']):
        try:
            f = toml.load(open(os.path.join(p, basil_file_name)))
        except(FileNotFoundError):
            continue

        f.setdefault('dependency', {})
        lang = f['dependency'].setdefault('lang', 'cpp')
        if lang == 'cpp':
            fill['qt_dependency'].extend(f['dependency'].setdefault('qt_lib', ''))
            fill['boost_dependency'].extend(f['dependency'].setdefault('boost_lib', ''))
            if 'neovim' in f.keys():
                fill['autocmd'].extend(f['neovim'].setdefault('autocmd', ''))
            fill['qt_version'] = f['dependency']['qt_version'] \
                    if fill['qt_version'] < f['dependency'].setdefault('qt_version', 0) else fill['qt_version']
            fill['boost_version'] = f['dependency']['boost_version'] \
                    if fill['boost_version'] < f['dependency'].setdefault('boost_version', 0)\
                    else fill['boost_version']

        if 'asset' in f.keys():
            path_list = f['asset'].setdefault('resource', [])
            for path in path_list:
                fill['resource'].extend(glob(os.path.join(p, path)))

    fill['source_paths'] = [
        os.path.join(fill['plugin_paths'][i], 'src', fill['plugin_name'][i] + '.cpp')
        for i in range(len(fill['plugin_name']))]

    fill['qt_dependency'] = list(set(fill['qt_dependency']))
    fill['boost_dependency'] = list(set(fill['boost_dependency']))
    fill['autocmd'] = list(set(fill['autocmd']))

for k in fill.keys():
    print(k, ':', fill[k])

template = env.get_template('CMakeLists.txt')
rendered = template.render(message=fill)
with open(this_dir + '/CMakeLists.txt', 'w') as f:
    f.write(rendered)

template = env.get_template('plugins.cpp')
rendered = template.render(message=fill)
with open(this_dir + '/src/plugins.cpp', 'w') as f:
    f.write(rendered)

template = env.get_template('Basilico.hpp')
rendered = template.render(message=fill)
with open(this_dir + '/include/Basilico.hpp', 'w') as f:
    f.write(rendered)

template = env.get_template('basilico_autocmds.vim')
rendered = template.render(message=fill)
with open(this_dir + '/plugin/basilico_autocmds.vim', 'w') as f:
    f.write(rendered)

template = env.get_template('Basilico.qrc')
rendered = template.render(message=fill)
with open(this_dir + '/Basilico.qrc', 'w') as f:
    f.write(rendered)
