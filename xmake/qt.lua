-- Call this in an existing target to configure it as a Qt application.
function add_qt()
    add_rules("qt.application")
    add_rules("qt.qrc")
    add_rules("qt.moc")

    -- Path to your Qt installation folder, including the version
    -- e.g. C:\Qt\5.12.0\msvc2017_64 or /usr/local/Qt-5.12.0
    local qt_root = os.getenv("QTROOT")

    if is_host("windows") then

        local qt_platform_folder = path.join(qt_root, "msvc2019_64")
        local qt_lib = path.join(qt_platform_folder, "lib")
        local qt_include = path.join(qt_platform_folder, "include")

        -- include
        add_includedirs(path.join(qt_include, "QtCore"))
        add_includedirs(path.join(qt_include, "QtGui"))
        add_includedirs(path.join(qt_include, "QtWidgets"))

        -- link
        add_linkdirs(qt_lib)
        add_links("Qt6Core", "Qt6Gui", "Qt6Widgets")

        -- compiler flag
        add_cxflags("/Zc:__cplusplus")

    elseif is_host("macosx") then

        local qt_platform_folder = path.join(qt_root, "macos")
        local qt_lib = path.join(qt_platform_folder, "lib")
        local qt_include = path.join(qt_platform_folder, "include")

        -- include
        add_includedirs(qt_include)
        add_includedirs(path.join(qt_lib, "QtCore.framework", "Headers"))
        add_includedirs(path.join(qt_lib, "QtGui.framework", "Headers"))
        add_includedirs(path.join(qt_lib, "QtWidgets.framework", "Headers"))

        -- link
        add_linkdirs(path.join(qt_lib, "QtCore.framework"))
        add_linkdirs(path.join(qt_lib, "QtGui.framework"))
        add_linkdirs(path.join(qt_lib, "QtWidgets.framework"))

        -- frameworks
        add_ldflags("-F" .. qt_lib)
        add_frameworks("QtCore", "QtGui", "QtWidgets")

    elseif is_host("linux") then
        -- TODO

    else
        print("Unsupported host platform")
    end
end