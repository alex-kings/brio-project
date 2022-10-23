project('core','cpp',default_options : ['default_library=static'])
install_headers('Vec2D.h')
jsondep = dependency('jsoncpp')
executable('main',['main.cpp','Vec2D.cpp'],dependencies : jsondep)