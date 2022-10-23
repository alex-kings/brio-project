project('core','cpp',default_options : ['default_library=static'])

inc = include_directories('include')
jsondep = dependency('jsoncpp')

subdir('include')
executable('main',['./src/main.cpp','./src/Vec2D.cpp'],include_directories : inc,dependencies : jsondep)