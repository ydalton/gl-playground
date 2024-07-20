project(
  'sephine',
  'c',
  default_options: [
    'warning_level=3',
    'optimization=2',
    'c_std=c99'
  ]
)

add_global_arguments('-Werror=implicit-function-declaration', language: 'c')

sephine_deps = [
  dependency('sdl2'),
  dependency('cglm'),
]

tts = find_program('tts')
subdir('data')

sephine_sources = files()
subdir('src')

include_dirs = include_directories('include', 'include/vendor')

sephine_lib = shared_library(
  meson.project_name(),
  sephine_sources,
  c_args: [
    '-Wl,--gc-sections'
  ],
  include_directories: include_dirs,
  dependencies: sephine_deps,
  install: true
)

pkg = import('pkgconfig')
pkg.generate(sephine_lib)

subdir('examples')
