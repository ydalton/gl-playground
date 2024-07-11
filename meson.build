project(
  'gametest',
  'c',
  default_options: [
    'warning_level=3',
    'optimization=2',
    'c_std=c99'
  ]
)

add_global_arguments('-Werror=implicit-function-declaration', language: 'c')

gametest_deps = [
  dependency('sdl2'),
  dependency('cglm'),
]

tts = find_program('tts')
subdir('data')

gametest_sources = files()
subdir('src')

executable(
  meson.project_name(),
  gametest_sources,
  dependencies: gametest_deps,
  include_directories: include_directories('include', 'include/vendor')
)
