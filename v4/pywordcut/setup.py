from distutils.core import setup, Extension
import os,string,re

def run_cmd(cmd):
    file=os.popen(cmd,'r')
    text=file.read()
    file.close()
    return text

incopt=string.strip(run_cmd('pkg-config wordcut --cflags'))
incdir=[]
for inctok in string.split(incopt):
    incdir.append(re.match(r'-I(.+)',inctok).group(1))

libopt=string.strip(run_cmd('pkg-config wordcut --libs'))
libs=[]
libdir=[]
for libtok in string.split(libopt):
    if libtok[0:2] == '-l':
        libs.append(libtok[2:])
    elif libtok[0:2] == '-L':
        libdir.append(libtok[2:])
        
setup(name="pywordcut", version="0.0.1",
      ext_modules=[
    Extension("wordcut", ["pywordcut.c"],
              include_dirs=incdir,
              libraries=libs,
              library_dirs=libdir
              )       
    ])
