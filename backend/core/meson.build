project('core','cpp',default_options : ['default_library=static'])

inc = include_directories('include')
jsondep = dependency('jsoncpp')

subdir('include')

cppFiles = ['./src/main.cpp','./src/ParseJson.cpp','./src/Vec2D.cpp','./src/Piece.cpp','./src/Part.cpp','./src/TrackGenerator.cpp','./src/Connector.cpp','./src/Obb.cpp']

executable('main',cppFiles ,include_directories : inc,dependencies : jsondep)