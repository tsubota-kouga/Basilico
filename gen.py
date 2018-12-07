
from jinja2 import Template, Environment, FileSystemLoader
from neovim import attach
from glob import glob
import pathlib
import os, sys, re
import subprocess

port: str
try:
    port = sys.argv[1]
except:
    port = '6666'

# subprocess.Popen(['nvim', '--listen', '127.0.0.1:' + port, '--headless'])

env = Environment(loader=FileSystemLoader('template'))
template = env.get_template('CMakeLists.txt')
fill = {}

nvim = attach('tcp', address='localhost', port=port)

basilico_plugin_default_dir = nvim.eval('g:basilico_plugin_default_dir')

basil_file_name = '.basil_plugin'
basil_plugin_paths = [
    os.path.dirname(path)
    for path in glob(os.path.join(basilico_plugin_default_dir, '*', '*', basil_file_name))]

fill['plugin_paths'] = [path for path in basil_plugin_paths]
fill['plugin_name'] = [tail for head, tail in
    [os.path.split(basil_plugin_path) for basil_plugin_path in basil_plugin_paths]]

# array
basilico_plugin_added = nvim.eval('g:basilico_plugin_added')
for path in basilico_plugin_added:
    head, tail = os.path.split(path)
    fill['plugin_paths'].append(path)
    fill['plugin_name'].append(tail)

fill['source_paths'] = [
    os.path.join(fill['plugin_paths'][i], 'src', fill['plugin_name'][i] + '.cpp')
    for i in range(len(fill['plugin_name']))]

for k in fill.keys():
    print(fill[k])

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

