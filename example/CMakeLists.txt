aux_source_directory(. EXAMPLE_SRC_LIST)

foreach(fileName ${EXAMPLE_SRC_LIST})
	get_filename_component(name ${fileName} NAME_WE)
	add_executable(${name} ${name}.c)
	add_dependencies(${name} ${PROJECT_NAME})
	target_link_libraries(${name} ${PROJECT_NAME})
endforeach(fileName)
