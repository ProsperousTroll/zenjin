project(
  'zenjin',
  'cpp',
  default_options:['default_library=static']
)

inc = include_directories('include')
# you can add dependencies by installing them with 'meson wrap install (package name)'
# then listing them as variables, like 'ex_dep = dependency(example)'
rl = dependency('raylib')

executable(
  'zenjin',
  ['src/main.cpp', 'src/states.cpp', 'src/entity.cpp'], # source files go here
  include_directories: inc,
  dependencies: [rl] # dependencies go here
)
