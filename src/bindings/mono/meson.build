add_languages('cs')


runtime_assemblies = []

# Check if we should use dotnet options
cs_is_dotnet = meson.get_compiler('cs').get_id().contains('dotnet')

if (cs_is_dotnet)

warning('Dotnet support is still not upstream in meson.')

runtime_assemblies += [
    '-r:System.Console.dll',
    '-r:Microsoft.CSharp.dll',
    '-r:System.Collections.dll',
    '-r:System.Collections.Concurrent.dll',
    '-r:System.ComponentModel.Primitives.dll',
    '-r:System.ComponentModel.Primitives.dll',
    '-r:System.Diagnostics.Debug.dll',
    '-r:System.Diagnostics.TraceSource.dll',
    '-r:System.Dynamic.Runtime.dll',
    '-r:System.Linq.dll',
    '-r:System.Runtime.dll',
    '-r:System.Runtime.Extensions.dll',
    '-r:System.Security.dll',
]

endif

mono_sublibs = [
  ['Eina',        true, ], #
  ['Eolian',      true, ], #
  ['Eo',          true, ], #
  ['Ecore',      false, ], #
  ['Efl',         true, ], #
  ['Evas',       false, ], #
  ['Edje',       false, ], #
  ['Eldbus',      true, ], #
  ['Elementary', false, ]  #
]

blacklisted_files = [
  'efl_class.eo',
  'efl_canvas_scene3d.eo',
  'evas_canvas3d_camera.eo',
  'evas_canvas3d_light.eo',
  'evas_canvas3d_material.eo',
  'evas_canvas3d_mesh.eo',
  'evas_canvas3d_node.eo',
  'evas_canvas3d_object.eo',
  'evas_canvas3d_primitive.eo',
  'evas_canvas3d_scene.eo',
  'evas_canvas3d_texture.eo',
  'efl_io_buffer.eo',
  'efl_io_queue.eo',
  'efl_io_sizer.eo',
  'efl_io_closer_fd.eo',
  'efl_io_buffered_stream.eo',
  'efl_io_positioner_fd.eo',
  'efl_io_reader_fd.eo',
  'efl_io_writer_fd.eo',
  'efl_io_copier_fd.eo',
  'efl_io_sizer_fd.eo',
  'efl_io_stdin.eo',
  'efl_io_stdout.eo',
  'efl_io_stderr.eo',
  'efl_io_file.eo',
  'efl_io_copier.eo',
  'efl_object_override.eo',
  'elm_web.eo',
  'elm_map.eo',
  'elm_list.eo',
  'elm_genlist.eo',
  'elm_view_list.eo',
  'elm_genlist_item.eo',
  'elm_gengrid.eo',
  'elm_glview_eo.cs',
  'elm_code_widget.eo',
  'elm_multibuttonentry_part.eo',
  'elm_atspi_bridge.eo',
  'elm_atspi_app_object.eo',
]

efl_mono_lib = library('eflcustomexportsmono',
    join_paths('..', '..', 'lib', 'efl_mono', 'efl_custom_exports_mono.c'),
    install : true,
    dependencies : [eo, eina, ecore],
    version : meson.project_version()
)

beta_option = []
if (get_option('mono-beta'))
   beta_option = '-b'
endif

mono_generator_target = []
mono_files = []
foreach lib : mono_sublibs
  package_name = lib[0].to_lower()
  eo_file_subdirs = get_variable(package_name + '_eo_subdirs')
  file_location = join_paths('..', '..', 'lib', package_name)

  if (package_name != 'eldbus')
    foreach eo_file_subdir : eo_file_subdirs
      if eo_file_subdir != ''
        mono_pub_eo_files = get_variable(package_name + '_' + eo_file_subdir +'_eo_files') + get_variable(package_name + '_' + eo_file_subdir + '_eot_files')
      else
        mono_pub_eo_files = get_variable(package_name +'_eo_files') + get_variable(package_name + '_eot_files')
      endif

      subdir_file_location = join_paths(file_location, eo_file_subdir)
      foreach mono_gen_file : mono_pub_eo_files
        if not blacklisted_files.contains(mono_gen_file)
          mono_generator_target += custom_target('eolian_mono_gen_'+mono_gen_file.underscorify()+'',
            input : join_paths(subdir_file_location, mono_gen_file),
            output : [mono_gen_file + '.cs'],
            command : [eolian_mono_gen, beta_option, '-I', meson.current_source_dir(), eolian_include_directories,
                                       '--dllimport', package_name,
                                       '-o', join_paths(meson.current_build_dir(), mono_gen_file + '.cs'),
                                       '-e', get_option('mono-examples-dir'),
                                       '@INPUT@'])
        endif
      endforeach
    endforeach
  endif
  if lib[1]
    subdir(package_name + '_mono')
  endif
endforeach

efl_mono_conf_data = configuration_data()
efl_mono_conf_data.set('EINA', eina_lib.full_path())
efl_mono_conf_data.set('EFL', efl_lib.full_path())
efl_mono_conf_data.set('ECORE', ecore_lib.full_path())
efl_mono_conf_data.set('EO', eo_lib.full_path())
efl_mono_conf_data.set('EVAS', evas_lib.full_path())
efl_mono_conf_data.set('ELDBUS', eldbus_lib.full_path())
efl_mono_conf_data.set('ELEMENTARY', elementary_lib.full_path())

efl_mono_dll_config = configure_file(input : 'efl_mono.dll.config.in',
                                 output : 'efl_mono.dll.config',
                                 configuration : efl_mono_conf_data)

extra_cs_args = runtime_assemblies

if get_option('mono-beta')
  extra_cs_args += '-d:EFL_BETA'
endif

efl_mono_install_dir = join_paths(dir_lib, 'efl-mono-'+version_major)
efl_mono_xml_doc = join_paths(meson.current_build_dir(), 'efl_mono.xml')

efl_mono = library('efl_mono',
    mono_generator_target + mono_files + [efl_src],
    install : true,
    install_dir : efl_mono_install_dir,
    cs_args : extra_cs_args + ['-doc:' + efl_mono_xml_doc]
)

meson.add_install_script(join_paths(meson.source_root(), 'meson', 'meson_csharp_docs.sh'),
                         efl_mono_xml_doc,
                         efl_mono_install_dir)

efl_mono_test_suite_path=join_paths(meson.current_build_dir())

pkgconfig.generate(
  name : 'efl-mono',
  description : 'Efl C# bindings',
  version : version_major + '.' + version_minor + '.' + version_micro,
  libraries : ['-r:${assemblies_dir}/efl_mono.dll'],
  variables : ['assemblies_dir='+join_paths(dir_lib, 'efl-mono-'+version_major),
               'mono_libs=-r:${assemblies_dir}/efl_mono.dll']
)
test_dirs += 'efl_mono'
