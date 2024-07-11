shaders = [
  'frag.glsl',
  'vert.glsl'
]

fs = import('fs')

foreach file : shaders
  basename = fs.name(file)
  shader_dep += custom_target(
    basename,
    input: file,
    output: file + '.h',
    command: [tts, '-o', '@OUTPUT@', '@INPUT@']
  )
endforeach
