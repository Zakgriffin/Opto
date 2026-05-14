add_rules("mode.debug", "mode.release")
add_rules("plugin.compile_commands.autoupdate", {outputdir = "."})
set_defaultmode("release")

add_requires("llvm")
add_requires("raylib 5.5", {configs = {runtimes = "MT"}})

set_toolchains("llvm@llvm")

target("Opto")
    set_kind("binary")
    set_languages("cxx20")
    set_runtimes("MT")
    add_files("src/**.cpp")
    add_includedirs("src", "src/object", "src/expressions", "src/control_flow", "src/data", "src/procedures", "src/optimize", "src/llvm", "src/run")
    add_packages("raylib", "llvm")

    on_load(function(target)
        local llvm_dir = target:pkg("llvm"):installdir()
        local cfg_exe = is_host("windows") and "llvm-config.exe" or "llvm-config"
        local llvm_config = path.join(llvm_dir, "bin", cfg_exe)

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
