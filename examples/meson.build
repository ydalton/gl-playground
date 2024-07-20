gametest_sources = files('main.c')

gametest_sources += shader_dep
gametest_sources += texture_dep

executable(
  'gametest',
  gametest_sources,
  c_args: [
    '-Wl,--gc-sections'
  ],
  include_directories: include_dirs,
  link_with: sephine_lib,
  dependencies: dependency('cglm')
)
