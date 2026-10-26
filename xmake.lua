add_rules("mode.debug", "mode.release")
set_languages("c++20")

target("modul_levi")
    set_kind("shared")
    add_files("src/*.cpp")
    add_includedirs("src", "nmdk/include")
    add_linkdirs("nmdk/libs/arm64-v8a")
    add_links("nmdk")
    
    -- Dependency Xmake sesuai NMDK v1.0.1
    add_packages("preloader_android", "boost_pfr", "nlohmann_json", "magic_enum", "fmt", "lunasvg", "entt")
    
    -- Optimasi dan flag standar
    if is_mode("release") then
        set_optimize("fastest")
        set_strip("all")
        add_cxflags("-fvisibility=hidden", "-fvisibility-inlines-hidden")
    end

    if is_plat("android") then
        set_arch("arm64-v8a")
        add_syslinks("android", "log")
        add_cxflags("-fno-rtti")
    end
