from distutils.core import setup, Extension

module1 = Extension('eps',
                    sources = ['epsmodule.cpp'])

setup (name = 'eps',
       version = '1.0',
       description = 'Emily Serial Protocol python extension',
       ext_modules = [module1])
       #ext_modules=[Extension('eps_pack_gps', ['epsmodule.cpp'])])
