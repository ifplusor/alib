# PCRE2
option(ENABLE_PCRE2 "Enable support for PCRE2." ON)
if (ENABLE_PCRE2)
    add_subdirectory(pcre2)
endif ()
