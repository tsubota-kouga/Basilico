
from jinja2 import Template, Environment, FileSystemLoader
from neovim import attach
from glob import glob
import os, sys, re
import toml

port: str
try:
    port = sys.argv[1]
except:
    port = '6666'

env = Environment(loader=FileSystemLoader('template'))
fill = {}

nvim = attach('tcp', address='localhost', port=port)

# basilico_plugin_default_dir = nvim.eval('g:basilico_plugin_default_dir')

basil_file_name = 'basil.toml'

# basil_plugin_paths = [
#     os.path.dirname(path)
#     for path in glob(os.path.join(basilico_plugin_default_dir, '*', '*', basil_file_name))]

fill['qt_version'] = 5.7
fill['boost_version'] = 1.55
fill['qt_dependency'] = []
fill['boost_dependency'] = []
# fill['plugin_paths'] = [path for path in basil_plugin_paths]
# fill['plugin_name'] = [tail for head, tail in
#     [os.path.split(basil_plugin_path) for basil_plugin_path in basil_plugin_paths]]
fill['plugin_paths'] = []
fill['plugin_name'] = []

# array
# runtimepath = nvim.eval('g:basilico_plugin_added')
runtimepath = nvim.eval('&runtimepath').split(',')
for path in runtimepath:
    basil_path = glob(os.path.join(path, basil_file_name))
    if basil_path != [] and path != '.':
        head, tail = os.path.split(path)
        fill['plugin_paths'].append(path)
        fill['plugin_name'].append(tail)

for p in fill['plugin_paths']:
    try:
        f = toml.load(open(os.path.join(p, basil_file_name)))
    except(FileNotFoundError):
        continue

    f.setdefault('dependency', {})
    lang = f['dependency'].setdefault('lang', 'cpp')
    if lang == 'cpp':
        fill['qt_dependency'].extend(f['dependency'].setdefault('qt_lib', ''))
        fill['boost_dependency'].extend(f['dependency'].setdefault('boost_lib', ''))
        fill['qt_version'] = f['dependency']['qt_version'] \
                if fill['qt_version'] < f['dependency'].setdefault('qt_version', 0) else fill['qt_version']
        fill['boost_version'] = f['dependency']['boost_version'] \
                if fill['boost_version'] < f['dependency'].setdefault('boost_version', 0)\
                else fill['boost_version']

fill['plugin_paths'].remove(os.path.abspath('.'))
fill['plugin_name'].remove('Basilico')

fill['source_paths'] = [
    os.path.join(fill['plugin_paths'][i], 'src', fill['plugin_name'][i] + '.cpp')
    for i in range(len(fill['plugin_name']))]

for k in fill.keys():
    print(k, ':', fill[k])

# no gui plugins
try:
    if sys.argv[2] == 'np':
        # dependency for Basilico
        fill = {}
        fill['qt_version'] = 5.7
        fill['boost_version'] = 1.55
        fill['qt_dependency'] = ['Widgets', 'Core']
        fill['boost_dependency'] = ['system', 'thread']
except:
    pass

template = env.get_template('CMakeLists.txt')
rendered = template.render(message=fill)
with open('CMakeLists.txt', 'w') as f:
    f.write(rendered)

template = env.get_template('plugins.cpp')
rendered = template.render(message=fill)
with open('src/plugins.cpp', 'w') as f:
    f.write(rendered)

template = env.get_template('Basilico.hpp')
rendered = template.render(message=fill)
with open('include/Basilico.hpp', 'w') as f:
    f.write(rendered)

