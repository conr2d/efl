# README
# 
# Evas build is a bit more complex.
# the final shared library is build in src/lib/evas_goal/meson.build.
#
# The content of lib/evas is build as static_library as 
# are all the engines loaders and savers.
#
# All those stages are defining dependencies with theire source code
# as 'source :', later everything is build as libevas.so.
#
# For now loaders and savers are ALWAYS build statically.
# 

evas_deps = [eo, eet, eina, efl, emile, ector, ecore]
pub_eo_file_target = []

pub_legacy_eo_files = [

] 

foreach eo_file : pub_legacy_eo_files
  pub_eo_file_target += custom_target('eolian_gen_' + eo_file,
    input : eo_file,
    output : [eo_file + '.h', eo_file + '.legacy.h',],
    install : true,
    install_dir : dir_package_include,
    command : [eolian_gen, '-I', meson.current_source_dir(), eolian_include_directories,
                           '-o', 'h:' + join_paths(meson.current_build_dir(), eo_file + '.h'),
                           '-o', 'c:' + join_paths(meson.current_build_dir(), eo_file + '.c'),
                           '-o', 'l:' + join_paths(meson.current_build_dir(), eo_file + '.legacy.h'),
                           '-gchl', '@INPUT@'])

endforeach

pub_eo_files = [
  
]

foreach eo_file : pub_eo_files
  pub_eo_file_target += custom_target('eolian_gen_' + eo_file,
    input : eo_file,
    output : [eo_file + '.h'],
    install : true,
    install_dir : dir_package_include,
    command : [eolian_gen, '-I', meson.current_source_dir(), eolian_include_directories,
                           '-o', 'h:' + join_paths(meson.current_build_dir(), eo_file + '.h'),
                           '-o', 'c:' + join_paths(meson.current_build_dir(), eo_file + '.c'),
                           '-gch', '@INPUT@'])
endforeach


pub_eo_types_files = [

]

foreach eo_file : pub_eo_types_files
  pub_eo_file_target += custom_target('eolian_gen_' + eo_file,
    input : eo_file,
    output : [eo_file + '.h'],
    install : true,
    install_dir : dir_package_include,
    command : [eolian_gen, '-I', meson.current_source_dir(), eolian_include_directories,
                           '-o', 'h:' + join_paths(meson.current_build_dir(), eo_file + '.h'),
                           '-gh', '@INPUT@'])
endforeach

eolian_include_directories += ['-I', meson.current_source_dir()]

evas_header_src = [
  'Evas.h',
  'Evas_Common.h',
  'Evas_Eo.h',
  'Evas_GL.h',
  'Evas_Internal.h',
  'Evas_Legacy.h',
  'Evas_Loader.h',
]

evas_include_directories = [
  include_directories('.'),
  include_directories('common'),
  include_directories('common3d'),
  include_directories(join_paths('common3d', 'save_load')),
  include_directories('include'),
  include_directories('filters'),
  vg_common_inc_dir
]

evas_src = [
   'main.c'
]

evas_src += vg_common_src

evas_deps += dependency('freetype2')

font_config = dependency('fontconfig')
if (font_config.found())
   config_h.set('HAVE_FONTCONFIG', '1')
   evas_deps += font_config
endif

fribidi = dependency('fribidi')
if (fribidi.found())
   config_h.set('HAVE_FRIBIDI', '1')
   evas_deps += fribidi
endif

subdir('include')
subdir('common')
subdir('canvas')
subdir('gesture')
subdir('filters')
subdir('cache')
subdir('common3d')
subdir('file')
subdir('vg')

gl_deps = []

if get_option('harfbuzz')
  evas_deps += dependency('harfbuzz')
  config_h.set('HAVE_HARFBUZZ', '1')
endif

if get_option('ecore_wl2') or get_option('ecore_buffer')
  evas_deps += wayland_protocol
endif


if get_option('opengl') != 'none'
  gl_deps += dependency('gl')
endif

if get_option('opengl') == 'es-egl'
  config_h.set('GL_GLES', '1')
  gl_deps += dependency('egl')
endif

evas_pre = declare_dependency(
  include_directories: evas_include_directories + [vg_common_inc_dir],
  sources : pub_eo_file_target,
  dependencies: [eina, eo, ector, emile, evas_deps],
)

evas_pub_eo_file_target = pub_eo_file_target

evas_pre_lib_dep = declare_dependency(
  include_directories: evas_include_directories + [vg_common_inc_dir],
  sources : [evas_src, evas_pub_eo_file_target],
  dependencies: [evas_deps, m, draw, valgrind, libunibreak]
)

#install_data(pub_eo_files,
#  install_dir: join_paths(eolian_include_dir, package_version_name)
#)

install_headers(evas_header_src,
  install_dir : dir_package_include,
)