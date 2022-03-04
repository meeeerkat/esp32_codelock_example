set(srcs "main.c"
         "codelock.c"
         "validation_leds.c")
set(include_dirs ".")

idf_component_register(SRCS "${srcs}"
                       INCLUDE_DIRS "${include_dirs}")
