examples = [
  'evas-3d-aabb',
  'evas-3d-blending',
  'evas-3d-colorpick',
  'evas-3d-cube2',
  'evas-3d-cube',
  'evas-3d-cube-rotate',
  'evas-3d-eet',
  'evas-3d-fog',
  'evas-3d-frustum',
  'evas-3d-hull',
  'evas-3d-md2',
  'evas-3d-mmap-set',
  'evas-3d-obj',
  'evas-3d-parallax-occlusion',
  'evas-3d-pick',
  'evas-3d-ply',
  'evas-3d-proxy',
  'evas-3d-shadows',
  'evas-3d-static-lod',
  #'evas-aspect-hints',
  'evas-box',
  'evas-buffer-simple',
  'evas-event-filter',
  'evas-events',
  'evas-gl',
  'evas-hints',
  'evas-images2',
  'evas-images3',
  'evas-images4',
  'evas-images5',
  'evas-images6',
  'evas-images',
  'evas-init-shutdown',
  'evas-map-aa',
  'evas-map-aa-eo',
  'evas-map-utils',
  'evas-map-utils-eo',
  'evas-multiseat-events',
  'evas-multi-touch',
  'evas-object-manipulation',
  'evas-object-manipulation-eo',
  'evas-smart-interface',
  'evas-smart-object',
  'evas-stacking',
  'evas-table',
  'evas-textblock-obstacles',
  'evas-text',
  'evas-transparent',
  'evas-vg-batman',
  'evas-vg-simple',
  'evas-vg-json',
]

foreach example : examples
  executable(example, example + '.c',
    dependencies: [eina, ecore_evas, ecore_file, m],
    c_args : ['-DPACKAGE_EXAMPLES_DIR="'+meson.current_source_dir()+'"'])
endforeach

subdir('shooter')
