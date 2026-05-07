# Sourced by CPack once per generator, with CPACK_GENERATOR set to the
# active generator (DEB, RPM, TGZ, ...). Selects the components that should
# end up in the final package.

if(CPACK_GENERATOR STREQUAL "DEB")
    set(CPACK_COMPONENTS_ALL runtime deb_license)
elseif(CPACK_GENERATOR STREQUAL "RPM")
    set(CPACK_COMPONENTS_ALL runtime rpm_license)
else()
    set(CPACK_COMPONENTS_ALL runtime)
endif()
