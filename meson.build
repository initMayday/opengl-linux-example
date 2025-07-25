project(
  'opengl',
  ['cpp', 'c'],
  version: '0.1',
  default_options: [
    'cpp_std=c++23',
    'warning_level=2',
  ],
)

glfw_dep = dependency('glfw3')
glad_inc = include_directories('gladdir')
spdlog_dep = dependency('spdlog')


program = executable(
  'opengl',
  ['main.cpp', 'shader.cpp', 'glad.c'],
  dependencies : [glfw_dep, spdlog_dep],
  include_directories: [include_directories('.'), glad_inc],
  install: true
)
