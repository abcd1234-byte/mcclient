from distutils.core import setup, Extension

module1 = Extension('cmcclient', extra_compile_args=['-std=c99'],
                    sources = ['sector_py.c', 'sector.c', 'blocktypes.c',
                               'world_renderer_py.c', 'world_renderer.c',
                               'frustum.c', 'cmcclient.c', 'block_py.c'])

setup (name = 'Test',
       version = '1.0',
       description = 'This is a demo package',
       ext_modules = [module1])
