project('core','cpp',default_options : ['default_library=static'])

inc = include_directories('include')
jsondep = dependency('jsoncpp')

subdir('include')

cppFiles = ['./src/main.cpp','./src/ReadJson.cpp','./src/Vec2D.cpp','./src/Piece.cpp']

executable('main',cppFiles ,include_directories : inc,dependencies : jsondep)