shaders = [
  'bowser.png',
  'checkertile.png'
]

foreach file : shaders
  basename = fs.name(file)
  texture_dep += custom_target(
    basename,
    input: file,
    output: file + '.h',
    command: [tts, '-b', '-o', '@OUTPUT@', '@INPUT@']
  )
endforeach
