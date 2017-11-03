# PCRE
option(ENABLE_PCRE "Enable support for PCRE." ON)
message("ENABLE_PCRE: ${ENABLE_PCRE}")
if (ENABLE_PCRE)
    add_subdirectory(pcre)
endif ()

# PCRE2
option(ENABLE_PCRE2 "Enable support for PCRE2." OFF)
message("ENABLE_PCRE2: ${ENABLE_PCRE2}")
if (ENABLE_PCRE2)
    add_subdirectory(pcre2)
endif ()
