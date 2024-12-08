project('aoc2024', 'c', meson_version: '>= 1.3.0')

fs = import('fs')
cc = meson.get_compiler('c')
m_dep = cc.find_library('m', required : false)

foreach day : range(1,24)
  day_name = 'day' + day.to_string(fill: 2)
  dir_name = './' + day_name
  if fs.is_dir(dir_name)
    if (fs.is_file(dir_name / 'part1.c'))
      executable(day_name + '_1', dir_name / 'part1.c', dependencies: m_dep)
    endif
    if (fs.is_file(dir_name / 'part2.c'))
      executable(day_name + '_2', dir_name / 'part2.c', dependencies: m_dep)
    endif
  endif
endforeach
