add_executable(update_namesilo_record main.c request.c namesilo.c env_vars.c util.c)
#target_compile_features(update_namesilo_record PRIVATE cxx_std_17)

target_link_libraries(update_namesilo_record ${CONAN_LIBS})
