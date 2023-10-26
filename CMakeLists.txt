# Preamble
cmake_minimum_required(VERSION 3.28)
list(APPEND CMAKE_MESSAGE_CONTEXT "ext")
set(CMAKE_CXX_SCAN_FOR_MODULES OFF) # https://discourse.cmake.org/t/cmake-3-28-cmake-cxx-compiler-clang-scan-deps-notfound-not-found/9244
project(ext
    VERSION "0.0.0"
    DESCRIPTION "Homebrew extensions"
    LANGUAGES CXX
)
enable_testing()

# Project wide setup
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Dependencies
find_package(Qt6
    OPTIONAL_COMPONENTS Core Gui Widgets WebEngineWidgets
)
message("Qt6_FOUND: ${Qt6_FOUND}")
message("Qt6Core_FOUND: ${Qt6Core_FOUND}")
message("Qt6Gui_FOUND: ${Qt6Gui_FOUND}")
message("Qt6Widgets_FOUND: ${Qt6Widgets_FOUND}")
message("Qt6WebEngineWidgets_FOUND: ${Qt6WebEngineWidgets_FOUND}")

if(${PROJECT_IS_TOP_LEVEL})
    set(FETCHCONTENT_SOURCE_DIR_BOOST "C:/cpp/boost_1_83_0/boost")
endif()

include(FetchContent)
FetchContent_Declare(Boost
    GIT_REPOSITORY https://github.com/boostorg/boost.git
    GIT_TAG 564e2ac16907019696cdaba8a93e3588ec596062 # boost-1.83.0
    GIT_SHALLOW TRUE
    EXCLUDE_FROM_ALL
    #SYSTEM
    FIND_PACKAGE_ARGS 1.83.0
)
FetchContent_MakeAvailable(Boost)

# Targets
add_custom_target(ext_administrative_files)
target_sources(ext_administrative_files PRIVATE "./LICENSE")
set_source_files_properties("./LICENSE" PROPERTIES HEADER_FILE_ONLY YES)
add_custom_target(ext_tooling_support)
target_sources(ext_tooling_support PRIVATE "./.clang-format")
set_source_files_properties("./.clang-format" PROPERTIES HEADER_FILE_ONLY YES)

add_library(ext_headers INTERFACE)
add_library(${PROJECT_NAME}::headers ALIAS ext_headers)
set_target_properties(ext_headers PROPERTIES
    EXPORT_NAME headers
)
target_sources(ext_headers
    PUBLIC
        FILE_SET headers
            TYPE HEADERS
            BASE_DIRS "./include"
            FILES
                "./include/ext_infrastructure/ext_algorithm.hpp"
                "./include/ext_infrastructure/ext_graphviz.hpp"
                "./include/ext_infrastructure/ext_ranges.hpp"
                "./include/ext_infrastructure/ext_string.hpp"
                "./include/ext_infrastructure/ext_thread.hpp"
                "./include/ext_infrastructure/ext_type_traits.hpp"
                "./include/ext_infrastructure/ext_typeinfo.hpp"
                "./include/ext_infrastructure/ext_utility.hpp"
                "./include/ext_infrastructure/map_default_value.hpp"
                "./include/ext_infrastructure/overloaded.hpp"
)
if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC") # if(MSVC) clang-cl: warning: argument unused during compilation: '/Zc:preprocessor' [-Wunused-command-line-argument]
    target_compile_options(ext_headers INTERFACE "/Zc:preprocessor")
endif()
target_link_libraries(ext_headers INTERFACE Boost::utility Boost::mp11)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

if(Qt6Gui_FOUND)
    add_library(ext_infrastructure)
    add_library(${PROJECT_NAME}::infrastructure ALIAS ext_infrastructure)
    set_target_properties(ext_infrastructure PROPERTIES
        EXPORT_NAME infrastructure
    )
    target_sources(ext_infrastructure
        PUBLIC
            FILE_SET headers
                TYPE HEADERS
                BASE_DIRS "./include"
                FILES
                    "./include/ext_infrastructure/ext_bitset.hpp"
                    "./include/ext_infrastructure/ext_configs.hpp"
                    "./include/ext_infrastructure/ext_debug.hpp"
                    "./include/ext_infrastructure/ext_debug_core.hpp"
                    "./include/ext_infrastructure/ext_debug_overrides.hpp"
                    "./include/ext_infrastructure/ext_error_handling.hpp"
                    "./include/ext_infrastructure/ext_hilbert.hpp"
                    "./include/ext_infrastructure/ext_json.hpp"
    )
    set_source_files_properties("./include/ext_infrastructure/ext_debug_overrides.hpp" PROPERTIES
        SKIP_LINTING ON
    )
    target_sources(ext_infrastructure
        PRIVATE
            "./src/ext_infrastructure/ext_bitset.cpp"
            "./src/ext_infrastructure/ext_configs.cpp"
            "./src/ext_infrastructure/ext_debug.cpp"
            "./src/ext_infrastructure/ext_debug_core.cpp"
            "./src/ext_infrastructure/ext_debug_overrides.cpp"
            "./src/ext_infrastructure/ext_error_handling.cpp"
            "./src/ext_infrastructure/ext_hilbert.cpp"
            "./src/ext_infrastructure/ext_json.cpp"
    )
    target_link_libraries(ext_infrastructure PUBLIC ext::headers Boost::stl_interfaces Boost::functional Qt6::Gui PRIVATE Boost::container_hash)
endif()

if(Qt6Widgets_FOUND)
    add_library(ext_core)
    add_library(${PROJECT_NAME}::core ALIAS ext_core)
    set_target_properties(ext_core PROPERTIES
        EXPORT_NAME core
    )
    target_sources(ext_core
        PUBLIC
            FILE_SET headers
                TYPE HEADERS
                BASE_DIRS "./include"
                FILES
                    "./include/ext_core/ext_dir_iterator.hpp"
                    "./include/ext_core/ext_file_info.hpp"
                    "./include/ext_core/ext_mimedata.hpp"
                    "./include/ext_core/ext_object.hpp"
                    "./include/ext_core/ext_string.hpp"
                    "./include/ext_core/ext_thread.hpp"
    )
    target_sources(ext_core
        PRIVATE
            "./src/ext_core/ext_dir_iterator.cpp"
            "./src/ext_core/ext_file_info.cpp"
            "./src/ext_core/ext_mimedata.cpp"
            "./src/ext_core/ext_string.cpp"
    )
    set_target_properties(ext_core PROPERTIES
        VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}"
        SOVERSION "${PROJECT_VERSION_MAJOR}"
    )
    target_link_libraries(ext_core PUBLIC ext::infrastructure PRIVATE Qt6::Widgets)
endif()

if(Qt6Widgets_FOUND)
    add_library(ext_gui)
    add_library(${PROJECT_NAME}::gui ALIAS ext_gui)
    set_target_properties(ext_gui PROPERTIES
        EXPORT_NAME gui
    )
    target_sources(ext_gui
        PUBLIC
            FILE_SET headers
                TYPE HEADERS
                BASE_DIRS "./include"
                FILES
                    "./include/ext_gui/ext_image.hpp"
    )
    target_sources(ext_gui
        PRIVATE
            "./src/ext_gui/ext_image.cpp"
    )
    set_target_properties(ext_gui PROPERTIES
        VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}"
        SOVERSION "${PROJECT_VERSION_MAJOR}"
    )
    target_link_libraries(ext_gui PUBLIC ext::core Qt6::Widgets)
endif()

if(Qt6Widgets_FOUND)
    add_library(ext_widgets)
    add_library(${PROJECT_NAME}::widgets ALIAS ext_widgets)
    set_target_properties(ext_widgets PROPERTIES
        EXPORT_NAME widgets
    )
    target_sources(ext_widgets
        PUBLIC
            FILE_SET headers
                TYPE HEADERS
                BASE_DIRS "./include"
                FILES
                    "./include/ext_widgets/escape_ampersand.hpp"
                    "./include/ext_widgets/ext_button.hpp"
                    "./include/ext_widgets/ext_groupbox.hpp"
                    "./include/ext_widgets/ext_style.hpp"
                    "./include/ext_widgets/ext_text_edit.hpp"
                    "./include/ext_widgets/flow_layout.hpp"
                    "./include/ext_widgets/keep_window_stays_on_top.hpp"
                    "./include/ext_widgets/menu_nested.hpp"
                    "./include/ext_widgets/menu_permanent.hpp"
                    "./include/ext_widgets/menu_positioned.hpp"
                    "./include/ext_widgets/menu_window.hpp"
                    "./include/ext_widgets/scroll_area_vertical.hpp"
                    "./include/ext_widgets/widget_colored.hpp"
                    "./include/ext_widgets/widget_consuming_click.hpp"
    )
    target_include_directories(ext_widgets PRIVATE "./src")
    target_sources(ext_widgets
        PRIVATE
            "./src/ext_widgets/escape_ampersand.cpp"
            "./src/ext_widgets/ext_button.cpp"
            "./src/ext_widgets/ext_groupbox.cpp"
            "./src/ext_widgets/ext_style.cpp"
            "./src/ext_widgets/ext_text_edit.cpp"
            "./src/ext_widgets/flow_layout.cpp"
            "./include/ext_widgets/keep_window_stays_on_top_p.hpp"
            "./src/ext_widgets/keep_window_stays_on_top.cpp"
            "./include/ext_widgets/menu_nested_p.hpp"
            "./src/ext_widgets/menu_nested.cpp"
            "./include/ext_widgets/menu_permanent_p.hpp"
            "./src/ext_widgets/menu_permanent.cpp"
            "./include/ext_widgets/menu_positioned_p.hpp"
            "./src/ext_widgets/menu_positioned.cpp"
            "./src/ext_widgets/menu_window.cpp"
            "./src/ext_widgets/scroll_area_vertical.cpp"
            "./src/ext_widgets/widget_colored.cpp"
            "./src/ext_widgets/widget_consuming_click.cpp"
    )
    set_target_properties(ext_widgets PROPERTIES
        VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}"
        SOVERSION "${PROJECT_VERSION_MAJOR}"
    )
    target_link_libraries(ext_widgets PUBLIC ext::gui)
endif()

if(Qt6WebEngineWidgets_FOUND)
    add_library(ext_web)
    add_library(${PROJECT_NAME}::web ALIAS ext_web)
    set_target_properties(ext_web PROPERTIES
        EXPORT_NAME web
    )
    target_sources(ext_web
        PUBLIC
            FILE_SET headers
                TYPE HEADERS
                BASE_DIRS "./include"
                FILES
                    "./include/ext_web/ext_webengine.hpp"
    )
    target_include_directories(ext_web PRIVATE "./src")
    target_sources(ext_web
        PRIVATE
            "./include/ext_web/ext_webengine_p.hpp"
            "./src/ext_web/ext_webengine.cpp"
    )
    set_target_properties(ext_web PROPERTIES
        VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}"
        SOVERSION "${PROJECT_VERSION_MAJOR}"
    )
    target_link_libraries(ext_web PUBLIC ext::widgets Qt6::WebEngineWidgets)
endif()

if(Qt6Gui_FOUND)
    target_compile_definitions(ext_infrastructure PRIVATE "QT_NO_KEYWORDS")
endif()
if(Qt6Widgets_FOUND)
    target_compile_definitions(ext_core PRIVATE "QT_NO_KEYWORDS")
endif()
if(Qt6Widgets_FOUND)
    target_compile_definitions(ext_gui PRIVATE "QT_NO_KEYWORDS")
endif()
if(Qt6Widgets_FOUND)
    target_compile_definitions(ext_widgets PRIVATE "QT_NO_KEYWORDS")
endif()
if(Qt6WebEngineWidgets_FOUND)
    target_compile_definitions(ext_web PRIVATE "QT_NO_KEYWORDS")
endif()

message("CMAKE_CXX_COMPILER_ID: ${CMAKE_CXX_COMPILER_ID}")
message("CMAKE_CXX_SIMULATE_ID: ${CMAKE_CXX_SIMULATE_ID}")
message("CMAKE_CXX_COMPILER_FRONTEND_VARIANT: ${CMAKE_CXX_COMPILER_FRONTEND_VARIANT}")
if(PROJECT_IS_TOP_LEVEL)
    if(CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "MSVC") # https://discourse.cmake.org/t/how-to-determine-if-clang-or-clang-cl-is-used-on-windows/4129
        if(Qt6Gui_FOUND)
            target_compile_options(ext_infrastructure PRIVATE "/W4" "/WX")
        endif()
        if(Qt6Widgets_FOUND)
            target_compile_options(ext_core PRIVATE "/W4" "/WX")
        endif()
        if(Qt6Widgets_FOUND)
            target_compile_options(ext_gui PRIVATE "/W4" "/WX")
        endif()
        if(Qt6Widgets_FOUND)
            target_compile_options(ext_widgets PRIVATE "/W4" "/WX")
        endif()
        if(Qt6WebEngineWidgets_FOUND)
            target_compile_options(ext_web PRIVATE "/W4" "/WX")
        endif()
    elseif(CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "GNU")
        if(Qt6Gui_FOUND)
            target_compile_options(ext_infrastructure PRIVATE "-Wall" "-Wextra" "-pedantic" "-Werror" "-Wno-language-extension-token")
        endif()
        if(Qt6Widgets_FOUND)
            target_compile_options(ext_core PRIVATE "-Wall" "-Wextra" "-pedantic" "-Werror" "-Wno-language-extension-token")
        endif()
        if(Qt6Widgets_FOUND)
            target_compile_options(ext_gui PRIVATE "-Wall" "-Wextra" "-pedantic" "-Werror" "-Wno-language-extension-token")
        endif()
        if(Qt6Widgets_FOUND)
            target_compile_options(ext_widgets PRIVATE "-Wall" "-Wextra" "-pedantic" "-Werror" "-Wno-language-extension-token")
        endif()
        if(Qt6WebEngineWidgets_FOUND)
            target_compile_options(ext_web PRIVATE "-Wall" "-Wextra" "-pedantic" "-Werror" "-Wno-language-extension-token")
        endif()
    else()
        message(FATAL_ERROR "unrecognized CMAKE_CXX_COMPILER_FRONTEND_VARIANT")
    endif()
endif()

# Testing
option(${PROJECT_NAME}_ENABLE_TESTING "whether to enable testing for ext" ${PROJECT_IS_TOP_LEVEL})
if(NOT PROJECT_IS_TOP_LEVEL)
    mark_as_advanced(${PROJECT_NAME}_ENABLE_TESTING)
endif()
if(${PROJECT_NAME}_ENABLE_TESTING)
block()
    list(APPEND CMAKE_MESSAGE_CONTEXT test)

    if(NOT CMAKE_VERIFY_INTERFACE_HEADER_SETS)
        message([[testing enabled but CMAKE_VERIFY_INTERFACE_HEADER_SETS is not true, consider adding
    -DCMAKE_VERIFY_INTERFACE_HEADER_SETS=TRUE
to configure phase and using
    --target all_verify_interface_header_sets
at build phase, to also verify independent interface headers]])
    endif()
endblock()
endif()

# Packaging
if(PROJECT_IS_TOP_LEVEL)
block()
    list(APPEND CMAKE_MESSAGE_CONTEXT packaging)

    include(GNUInstallDirs)
    set(CMAKE_INSTALL_DOCDIR "${CMAKE_INSTALL_DATAROOTDIR}/doc/${PROJECT_NAME}")
    if(NOT WIN32 AND PROJECT_IS_TOP_LEVEL)
        set(CMAKE_INSTALL_PREFIX "/opt/jhcarl0814/${PROJECT_NAME}")
    endif()
    file(RELATIVE_PATH relDir
        "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}"
        "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}"
    )
    set(CMAKE_INSTALL_RPATH "$ORIGIN" "$ORIGIN/${relDir}")

    install(TARGETS ext_headers
        boost_utility
        boost_mp11
            boost_core
            boost_io
            boost_preprocessor
            boost_static_assert
            boost_throw_exception
            boost_type_traits
                boost_config
                boost_assert
        EXPORT ${PROJECT_NAME}_headers_export
        FILE_SET headers
            DESTINATION "./${CMAKE_INSTALL_INCLUDEDIR}"
            COMPONENT ${PROJECT_NAME}_headers_development
        INCLUDES DESTINATION "./${CMAKE_INSTALL_INCLUDEDIR}"
    )
    if(Qt6Gui_FOUND)
        install(TARGETS ext_infrastructure
            boost_stl_interfaces
            boost_functional
            boost_container_hash
                boost_function
                boost_function_types
                boost_mpl
                boost_typeof
                boost_describe
                    boost_bind
                    boost_detail
                    boost_predef
            EXPORT ${PROJECT_NAME}_infrastructure_export
            RUNTIME
                DESTINATION ${CMAKE_INSTALL_BINDIR}
                COMPONENT ${PROJECT_NAME}_infrastructure_runtime
            LIBRARY
                DESTINATION ${CMAKE_INSTALL_LIBDIR}
                COMPONENT ${PROJECT_NAME}_infrastructure_runtime
                NAMELINK_COMPONENT ${PROJECT_NAME}_infrastructure_development
            ARCHIVE
                DESTINATION ${CMAKE_INSTALL_LIBDIR}
                COMPONENT ${PROJECT_NAME}_infrastructure_development
            FILE_SET headers
                DESTINATION "./${CMAKE_INSTALL_INCLUDEDIR}"
                COMPONENT ${PROJECT_NAME}_infrastructure_development
            INCLUDES DESTINATION "./${CMAKE_INSTALL_INCLUDEDIR}"
        )
    endif()
    if(Qt6Widgets_FOUND)
        install(TARGETS ext_core
            EXPORT ${PROJECT_NAME}_core_export
            RUNTIME
                DESTINATION ${CMAKE_INSTALL_BINDIR}
                COMPONENT ${PROJECT_NAME}_core_runtime
            LIBRARY
                DESTINATION ${CMAKE_INSTALL_LIBDIR}
                COMPONENT ${PROJECT_NAME}_core_runtime
                NAMELINK_COMPONENT ${PROJECT_NAME}_core_development
            ARCHIVE
                DESTINATION ${CMAKE_INSTALL_LIBDIR}
                COMPONENT ${PROJECT_NAME}_core_development
            FILE_SET headers
                DESTINATION "./${CMAKE_INSTALL_INCLUDEDIR}"
                COMPONENT ${PROJECT_NAME}_core_development
            INCLUDES DESTINATION "./${CMAKE_INSTALL_INCLUDEDIR}"
        )
    endif()
    if(Qt6Widgets_FOUND)
        install(TARGETS ext_gui
            EXPORT ${PROJECT_NAME}_gui_export
            RUNTIME
                DESTINATION ${CMAKE_INSTALL_BINDIR}
                COMPONENT ${PROJECT_NAME}_gui_runtime
            LIBRARY
                DESTINATION ${CMAKE_INSTALL_LIBDIR}
                COMPONENT ${PROJECT_NAME}_gui_runtime
                NAMELINK_COMPONENT ${PROJECT_NAME}_gui_development
            ARCHIVE
                DESTINATION ${CMAKE_INSTALL_LIBDIR}
                COMPONENT ${PROJECT_NAME}_gui_development
            FILE_SET headers
                DESTINATION "./${CMAKE_INSTALL_INCLUDEDIR}"
                COMPONENT ${PROJECT_NAME}_gui_development
            INCLUDES DESTINATION "./${CMAKE_INSTALL_INCLUDEDIR}"
        )
    endif()
    if(Qt6Widgets_FOUND)
        install(TARGETS ext_widgets
            EXPORT ${PROJECT_NAME}_widgets_export
            RUNTIME
                DESTINATION ${CMAKE_INSTALL_BINDIR}
                COMPONENT ${PROJECT_NAME}_widgets_runtime
            LIBRARY
                DESTINATION ${CMAKE_INSTALL_LIBDIR}
                COMPONENT ${PROJECT_NAME}_widgets_runtime
                NAMELINK_COMPONENT ${PROJECT_NAME}_widgets_development
            ARCHIVE
                DESTINATION ${CMAKE_INSTALL_LIBDIR}
                COMPONENT ${PROJECT_NAME}_widgets_development
            FILE_SET headers
                DESTINATION "./${CMAKE_INSTALL_INCLUDEDIR}"
                COMPONENT ${PROJECT_NAME}_widgets_development
            INCLUDES DESTINATION "./${CMAKE_INSTALL_INCLUDEDIR}"
        )
    endif()
    if(Qt6WebEngineWidgets_FOUND)
        install(TARGETS ext_web
            EXPORT ${PROJECT_NAME}_web_export
            RUNTIME
                DESTINATION ${CMAKE_INSTALL_BINDIR}
                COMPONENT ${PROJECT_NAME}_web_runtime
            LIBRARY
                DESTINATION ${CMAKE_INSTALL_LIBDIR}
                COMPONENT ${PROJECT_NAME}_web_runtime
                NAMELINK_COMPONENT ${PROJECT_NAME}_web_development
            ARCHIVE
                DESTINATION ${CMAKE_INSTALL_LIBDIR}
                COMPONENT ${PROJECT_NAME}_web_development
            FILE_SET headers
                DESTINATION "./${CMAKE_INSTALL_INCLUDEDIR}"
                COMPONENT ${PROJECT_NAME}_web_development
            INCLUDES DESTINATION "./${CMAKE_INSTALL_INCLUDEDIR}"
        )
    endif()

    install(EXPORT ${PROJECT_NAME}_headers_export
        DESTINATION "./cmake/${PROJECT_NAME}"
        FILE ${PROJECT_NAME}_headers_development.cmake
        NAMESPACE ${PROJECT_NAME}::
        COMPONENT ${PROJECT_NAME}_headers_development
    )
    if(Qt6Gui_FOUND)
        install(EXPORT ${PROJECT_NAME}_infrastructure_export
            DESTINATION "./cmake/${PROJECT_NAME}"
            FILE ${PROJECT_NAME}_infrastructure_development.cmake
            NAMESPACE ${PROJECT_NAME}::
            COMPONENT ${PROJECT_NAME}_infrastructure_development
        )
    endif()
    if(Qt6Widgets_FOUND)
        install(EXPORT ${PROJECT_NAME}_core_export
            DESTINATION "./cmake/${PROJECT_NAME}"
            FILE ${PROJECT_NAME}_core_development.cmake
            NAMESPACE ${PROJECT_NAME}::
            COMPONENT ${PROJECT_NAME}_core_development
        )
    endif()
    if(Qt6Widgets_FOUND)
        install(EXPORT ${PROJECT_NAME}_gui_export
            DESTINATION "./cmake/${PROJECT_NAME}"
            FILE ${PROJECT_NAME}_gui_development.cmake
            NAMESPACE ${PROJECT_NAME}::
            COMPONENT ${PROJECT_NAME}_gui_development
        )
    endif()
    if(Qt6Widgets_FOUND)
        install(EXPORT ${PROJECT_NAME}_widgets_export
            DESTINATION "./cmake/${PROJECT_NAME}"
            FILE ${PROJECT_NAME}_widgets_development.cmake
            NAMESPACE ${PROJECT_NAME}::
            COMPONENT ${PROJECT_NAME}_widgets_development
        )
    endif()
    if(Qt6WebEngineWidgets_FOUND)
        install(EXPORT ${PROJECT_NAME}_web_export
            DESTINATION "./cmake/${PROJECT_NAME}"
            FILE ${PROJECT_NAME}_web_development.cmake
            NAMESPACE ${PROJECT_NAME}::
            COMPONENT ${PROJECT_NAME}_web_development
        )
    endif()

    include(CMakePackageConfigHelpers)
    configure_package_config_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}Config.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
        INSTALL_DESTINATION "./cmake/${PROJECT_NAME}"
        NO_SET_AND_CHECK_MACRO
        NO_CHECK_REQUIRED_COMPONENTS_MACRO
    )
    write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
        VERSION "${PROJECT_VERSION}"
        COMPATIBILITY SameMajorVersion
        ARCH_INDEPENDENT
    )
    install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
        DESTINATION "./cmake/${PROJECT_NAME}"
        COMPONENT ${PROJECT_NAME}_development
    )

    get_cmake_property(CPACK_COMPONENTS_ALL COMPONENTS)
    message("CPACK_COMPONENTS_ALL: ${CPACK_COMPONENTS_ALL}")

    set(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
    set(CPACK_PACKAGE_VENDOR "jhcarl0814")
    set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${PROJECT_DESCRIPTION}")
    set(CPACK_PACKAGE_INSTALL_DIRECTORY "${CPACK_PACKAGE_NAME}")
    set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
    set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
    set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
    set(CPACK_VERBATIM_VARIABLES YES)
    #set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_LIST_DIR}/Description.txt") #set(CPACK_PACKAGE_DESCRIPTION "${PROJECT_DESCRIPTION}")
    #set(CPACK_RESOURCE_FILE_WELCOME "${CMAKE_CURRENT_LIST_DIR}/Welcome.txt")
    set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_LIST_DIR}/License")
    #set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_LIST_DIR}/Readme.txt")
    set(CPACK_SOURCE_IGNORE_FILES
        /\\..*
        /CMakeLists\\.txt\\.user
    )
    set(CPACK_IFW_PACKAGE_TITLE "Homebrew extensions")
    set(CPACK_IFW_PACKAGE_MAINTENANCE_TOOL_NAME "${PROJECT_NAME}_MaintenanceTool")
    set(CPACK_IFW_PACKAGE_MAINTENANCE_TOOL_INI_FILE "${CPACK_IFW_PACKAGE_MAINTENANCE_TOOL_NAME}.ini")
    include(CPack)
    include(CPackIFW)
    cpack_add_install_type(${PROJECT_NAME}_install_type_all DISPLAY_NAME "all")
    cpack_add_component_group(${PROJECT_NAME}_install_group_top_level
        DISPLAY_NAME "Homebrew extensions"
        DESCRIPTION "${PROJECT_DESCRIPTION}"
        EXPANDED
        BOLD_TITLE
    )
    cpack_add_component(${PROJECT_NAME}_headers_development
        DISPLAY_NAME "Headers Development"
        DESCRIPTION "Headers Development Component"
        GROUP ${PROJECT_NAME}_install_group_top_level
        REQUIRED
        INSTALL_TYPES ${PROJECT_NAME}_install_type_all
    )
    if(Qt6Gui_FOUND)
        cpack_add_component(${PROJECT_NAME}_infrastructure_development
            DISPLAY_NAME "Infrastructure Development"
            DESCRIPTION "Infrastructure Development Component"
            DEPENDS ${PROJECT_NAME}_headers_development
            GROUP ${PROJECT_NAME}_install_group_top_level
            INSTALL_TYPES ${PROJECT_NAME}_install_type_all
        )
        cpack_add_component(${PROJECT_NAME}_infrastructure_runtime
            DISPLAY_NAME "Infrastructure Runtime"
            DESCRIPTION "Infrastructure Runtime Component"
            GROUP ${PROJECT_NAME}_install_group_top_level
            INSTALL_TYPES ${PROJECT_NAME}_install_type_all
        )
    endif()
    if(Qt6Widgets_FOUND)
        cpack_add_component(${PROJECT_NAME}_core_development
            DISPLAY_NAME "Core Development"
            DESCRIPTION "Core Development Component"
            DEPENDS ${PROJECT_NAME}_infrastructure_development
            GROUP ${PROJECT_NAME}_install_group_top_level
            INSTALL_TYPES ${PROJECT_NAME}_install_type_all
        )
        cpack_add_component(${PROJECT_NAME}_core_runtime
            DISPLAY_NAME "Core Runtime"
            DESCRIPTION "Core Runtime Component"
            DEPENDS ${PROJECT_NAME}_infrastructure_runtime
            GROUP ${PROJECT_NAME}_install_group_top_level
            INSTALL_TYPES ${PROJECT_NAME}_install_type_all
        )
    endif()
    if(Qt6Widgets_FOUND)
        cpack_add_component(${PROJECT_NAME}_gui_development
            DISPLAY_NAME "Gui Development"
            DESCRIPTION "Gui Development Component"
            DEPENDS ${PROJECT_NAME}_core_development
            GROUP ${PROJECT_NAME}_install_group_top_level
            INSTALL_TYPES ${PROJECT_NAME}_install_type_all
        )
        cpack_add_component(${PROJECT_NAME}_gui_runtime
            DISPLAY_NAME "Gui Runtime"
            DESCRIPTION "Gui Runtime Component"
            DEPENDS ${PROJECT_NAME}_core_runtime
            GROUP ${PROJECT_NAME}_install_group_top_level
            INSTALL_TYPES ${PROJECT_NAME}_install_type_all
        )
    endif()
    if(Qt6Widgets_FOUND)
        cpack_add_component(${PROJECT_NAME}_widgets_development
            DISPLAY_NAME "Widgets Development"
            DESCRIPTION "Widgets Development Component"
            DEPENDS ${PROJECT_NAME}_gui_development
            GROUP ${PROJECT_NAME}_install_group_top_level
            INSTALL_TYPES ${PROJECT_NAME}_install_type_all
        )
        cpack_add_component(${PROJECT_NAME}_widgets_runtime
            DISPLAY_NAME "Widgets Runtime"
            DESCRIPTION "Widgets Runtime Component"
            DEPENDS ${PROJECT_NAME}_gui_runtime
            GROUP ${PROJECT_NAME}_install_group_top_level
            INSTALL_TYPES ${PROJECT_NAME}_install_type_all
        )
    endif()
    if(Qt6WebEngineWidgets_FOUND)
        cpack_add_component(${PROJECT_NAME}_web_development
            DISPLAY_NAME "Web Development"
            DESCRIPTION "Web Development Component"
            DEPENDS ${PROJECT_NAME}_widgets_development
            GROUP ${PROJECT_NAME}_install_group_top_level
            INSTALL_TYPES ${PROJECT_NAME}_install_type_all
        )
        cpack_add_component(${PROJECT_NAME}_web_runtime
            DISPLAY_NAME "Web Runtime"
            DESCRIPTION "Web Runtime Component"
            DEPENDS ${PROJECT_NAME}_widgets_runtime
            GROUP ${PROJECT_NAME}_install_group_top_level
            INSTALL_TYPES ${PROJECT_NAME}_install_type_all
        )
    endif()
    cpack_add_component(${PROJECT_NAME}_development
        DISPLAY_NAME "Config Development"
        DESCRIPTION "Config.cmake file"
        GROUP ${PROJECT_NAME}_install_group_top_level
        REQUIRED
        HIDDEN
        INSTALL_TYPES ${PROJECT_NAME}_install_type_all
    )
    cpack_ifw_configure_component_group(${PROJECT_NAME}_install_group_top_level
        LICENSES "Unlicense license (The Unlicense)" "${CPACK_RESOURCE_FILE_LICENSE}"
    )
endblock()
endif()
