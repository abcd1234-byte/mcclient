from distutils.core import setup, Extension

module1 = Extension('sector', extra_compile_args=['-std=c99'],
                    sources = ['sector_py.c', 'sector.c', 'blocktypes.c'])

module2 = Extension('worldrenderer', extra_compile_args=['-std=c99'],
                    sources = ['world_renderer_py.c', 'world_renderer.c',
                               'frustum.c', 'sector.c', 'blocktypes.c'])

setup (name = 'Test',
       version = '1.0',
       description = 'This is a demo package',
       ext_modules = [module1, module2])
