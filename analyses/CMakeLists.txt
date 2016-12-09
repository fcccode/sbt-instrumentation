add_library(Points_to_plugin MODULE
	points_to_plugin.cpp
)
target_link_libraries(Points_to_plugin PRIVATE ${dg_libs})
install(TARGETS Points_to_plugin
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
	
add_library(Globals_analysis_plugin MODULE
	globals_analysis_plugin.cpp
)

install(TARGETS Globals_analysis_plugin
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})