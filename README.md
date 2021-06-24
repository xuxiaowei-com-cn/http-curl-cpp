# http-curl-cpp

## [vcpkg](https://github.com/microsoft/vcpkg)

- [中文概述](https://github.com/microsoft/vcpkg/blob/master/README_zh_CN.md)
    - [快速开始: Windows](https://github.com/microsoft/vcpkg/blob/master/README_zh_CN.md#%E5%BF%AB%E9%80%9F%E5%BC%80%E5%A7%8B-windows)
    - 安装依赖
        - 注意事项
            - 需要安装 Visual Studio 英文语言包
        - 编译
            ```
            bootstrap-vcpkg.bat
            ```
        - [curl](https://github.com/curl/curl)
            ```
            vcpkg install curl
            vcpkg install curl:x64-windows
            ```
        - [jsoncpp](https://github.com/open-source-parsers/jsoncpp)
            ```
            vcpkg install jsoncpp
            vcpkg install jsoncpp:x64-windows
            ```

        - 集成到全局
            ```
            vcpkg integrate install
            ```
        - 生成配置
            ```
            vcpkg integrate project
            ```
          按照以下提示配置项目
            ```
            Created nupkg: D:\GitHub stars\vcpkg\scripts\buildsystems\vcpkg.D.GitHubstars.vcpkg.1.0.0.nupkg
            
            With a project open, go to Tools->NuGet Package Manager->Package Manager Console and paste:
            Install-Package vcpkg.D.GitHubstars.vcpkg -Source "D:\GitHub stars\vcpkg\scripts\buildsystems"
            ```
