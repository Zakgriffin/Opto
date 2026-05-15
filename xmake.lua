add_rules("mode.debug", "mode.release")
add_rules("plugin.compile_commands.autoupdate", {outputdir = "."})
set_defaultmode("release")

add_requires("llvm")
add_requires("raylib 5.5", {configs = {runtimes = "MT"}})

add_cxflags("-w", {files = "src/codegen/llvm_codegen.cpp"})
-- ZZZZ silence these warnings for now

target("Opto")
    set_kind("binary")
    set_languages("cxx20")
    set_runtimes("MT")
    set_toolchains("llvm@llvm")
    add_files("src/**.cpp")
    add_includedirs("src", "src/object", "src/object_view", "src/expressions", "src/control_flow", "src/data", "src/procedures", "src/compile", "src/llvm", "src/run", "src/codegen")
    add_packages("raylib", "llvm")

    on_load(function(target)
        local llvm_dir = target:pkg("llvm"):installdir()
        local cfg_exe = is_host("windows") and "llvm-config.exe" or "llvm-config"
        local llvm_config = path.join(llvm_dir, "bin", cfg_exe)

        -- fallback for system LLVM (e.g. homebrew on macOS)
        if not os.isfile(llvm_config) and is_host("macosx") then
            local brew_prefix = try { function() return os.iorunv("brew", {"--prefix", "llvm"}) end }
            if brew_prefix then
                llvm_dir = brew_prefix:trim()
                llvm_config = path.join(llvm_dir, "bin", cfg_exe)
            end
        end

        target:add("sysincludedirs", path.join(llvm_dir, "include"))
        target:add("linkdirs", path.join(llvm_dir, "lib"))

        local components = {
            "core", "support", "target", "targetparser",
            "mc", "mcparser", "object",
            "x86codegen", "x86asmparser", "x86desc", "x86info",
            "asmprinter",
            "passes", "analysis", "scalaropts", "instcombine", "transformutils", "ipo"
        }
        local result = try { function()
            return os.iorunv(llvm_config, table.join({"--link-static", "--libnames"}, components))
        end }
        if result then
            local links = {}
            for lib in result:gmatch("LLVM%w+") do
                table.insert(links, lib)
            end
            if #links > 0 then
                target:add("links", table.unpack(links))
            end
        end

        if is_plat("windows") then
            target:add("syslinks", "ntdll")
        elseif is_plat("macosx") then
            target:add("frameworks", "IOKit", "Cocoa", "OpenGL")
        end
    end)
