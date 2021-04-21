{
    'target_defaults': {
        'default_configuration': 'Debug'
    },
    'targets': [
        {
            'target_name': 'VideoSource',
            'cflags_cc!': [ '-fno-rtti' ],
            'type': 'executable',
            'defines': [
                'UNICODE'
            ],
            'include_dirs': [
                './source_code/iris/include',
                './source_code/iris/include/iris',
                './source_code/iris/include/iris/internal',
                './source_code/iris/include/rtc',
                './source_code/iris/libyuv/include',
                './source_code/ipc',
                './source_code/common',
                './source_code/video_source',
                './source_code/plugin',
                './source_code/raw_data_observer',
                './source_code/process',
                "<!(node -e \"require('nan')\")"
            ],
            'sources': [
                "<!@(node -p \"var fs=require('fs'),path=require('path'),walk=function(r){let t,e=[],n=null;try{t=fs.readdirSync(r)}catch(r){n=r.toString()}if(n)return n;var a=0;return function n(){var i=t[a++];if(!i)return e;let u=path.resolve(r,i);i=r+'/'+i;let c=fs.statSync(u);if(c&&c.isDirectory()){let r=walk(i);return e=e.concat(r),n()}return e.push(i),n()}()};walk('./source_code/common/').join(' ');\")",
                "<!@(node -p \"var fs=require('fs'),path=require('path'),walk=function(r){let t,e=[],n=null;try{t=fs.readdirSync(r)}catch(r){n=r.toString()}if(n)return n;var a=0;return function n(){var i=t[a++];if(!i)return e;let u=path.resolve(r,i);i=r+'/'+i;let c=fs.statSync(u);if(c&&c.isDirectory()){let r=walk(i);return e=e.concat(r),n()}return e.push(i),n()}()};walk('./source_code/video_source/').join(' ');\")",
                "<!@(node -p \"var fs=require('fs'),path=require('path'),walk=function(r){let t,e=[],n=null;try{t=fs.readdirSync(r)}catch(r){n=r.toString()}if(n)return n;var a=0;return function n(){var i=t[a++];if(!i)return e;let u=path.resolve(r,i);i=r+'/'+i;let c=fs.statSync(u);if(c&&c.isDirectory()){let r=walk(i);return e=e.concat(r),n()}return e.push(i),n()}()};walk('./source_code/ipc/').join(' ');\")",
                "<!@(node -p \"var fs=require('fs'),path=require('path'),walk=function(r){let t,e=[],n=null;try{t=fs.readdirSync(r)}catch(r){n=r.toString()}if(n)return n;var a=0;return function n(){var i=t[a++];if(!i)return e;let u=path.resolve(r,i);i=r+'/'+i;let c=fs.statSync(u);if(c&&c.isDirectory()){let r=walk(i);return e=e.concat(r),n()}return e.push(i),n()}()};walk('./source_code/iris/src').join(' ');\")",
                "<!@(node -p \"var fs=require('fs'),path=require('path'),walk=function(r){let t,e=[],n=null;try{t=fs.readdirSync(r)}catch(r){n=r.toString()}if(n)return n;var a=0;return function n(){var i=t[a++];if(!i)return e;let u=path.resolve(r,i);i=r+'/'+i;let c=fs.statSync(u);if(c&&c.isDirectory()){let r=walk(i);return e=e.concat(r),n()}return e.push(i),n()}()};walk('./source_code/iris/libyuv/source').join(' ');\")",
                "<!@(node -p \"var fs=require('fs'),path=require('path'),walk=function(r){let t,e=[],n=null;try{t=fs.readdirSync(r)}catch(r){n=r.toString()}if(n)return n;var a=0;return function n(){var i=t[a++];if(!i)return e;let u=path.resolve(r,i);i=r+'/'+i;let c=fs.statSync(u);if(c&&c.isDirectory()){let r=walk(i);return e=e.concat(r),n()}return e.push(i),n()}()};walk('./source_code/plugin').join(' ');\")",
                "<!@(node -p \"var fs=require('fs'),path=require('path'),walk=function(r){let t,e=[],n=null;try{t=fs.readdirSync(r)}catch(r){n=r.toString()}if(n)return n;var a=0;return function n(){var i=t[a++];if(!i)return e;let u=path.resolve(r,i);i=r+'/'+i;let c=fs.statSync(u);if(c&&c.isDirectory()){let r=walk(i);return e=e.concat(r),n()}return e.push(i),n()}()};walk('./source_code/raw_data_observer').join(' ');\")"
            ],
            'conditions': [
                [
                    'OS=="win"',
                    {
                        'library_dirs': [

                        ]
                    }
                ], [
                    'OS=="mac"',
                    {
                        'mac_framework_dirs': [
                            '../sdk/lib/mac'
                        ],
                        'link_settings': {
                            'libraries': [
                                'AgoraAIDenoiseExtension.framework',
                                'AgoraCore.framework',
                                'AgoraDav1dExtension.framework',
                                'Agorafdkaac.framework',
                                'Agoraffmpeg.framework',
                                'AgoraRtcKit.framework',
                                'AgoraSoundTouch.framework',
                                'av1.framework'
                            ]
                        },
                        'include_dirs':[
                            './sdk/lib/mac/AgoraRtcKit.framework/Headers'
                        ],
                        'sources': [
                            './source_code/process/node_process_unix.cpp',
                        ],
                        'defines!': [
                            # '_HAS_EXCEPTIONS=0',
                            '-std=gnu++14'
                        ],
                        'OTHER_CFLAGS': [
                            '-std=c++11',
                            '-stdlib=libc++',
                            '-fexceptions'
                        ],
                        'xcode_settings': {
                            'MACOSX_DEPLOYMENT_TARGET': '10.13',
                            'GCC_ENABLE_CPP_RTTI': 'YES',
                            'FRAMEWORK_SEARCH_PATHS': [
                                './sdk/lib/mac'
                            ],
                            "DEBUG_INFORMATION_FORMAT": "dwarf-with-dsym",
                            'GCC_ENABLE_CPP_EXCEPTIONS': 'YES'
                        },
                    }
                ]
            ]
        },
        {
            'target_name': 'agora_node_ext',
            'cflags_cc!': [ '-fno-rtti' ],
            'include_dirs': [
                './source_code/iris/include',
                './source_code/iris/include/iris',
                './source_code/iris/include/iris/internal',
                './source_code/iris/include/rtc',
                './source_code/agora_node_ext',
                './source_code/iris/libyuv/include',
                './source_code/ipc/',
                './source_code/common/',
                './source_code/plugin',
                './source_code/raw_data_observer',
                './source_code/process',
                "<!(node -e \"require('nan')\")"
            ],
            'sources': [
                "<!@(node -p \"var fs=require('fs'),path=require('path'),walk=function(r){let t,e=[],n=null;try{t=fs.readdirSync(r)}catch(r){n=r.toString()}if(n)return n;var a=0;return function n(){var i=t[a++];if(!i)return e;let u=path.resolve(r,i);i=r+'/'+i;let c=fs.statSync(u);if(c&&c.isDirectory()){let r=walk(i);return e=e.concat(r),n()}return e.push(i),n()}()};walk('./source_code/agora_node_ext').join(' ');\")",
                "<!@(node -p \"var fs=require('fs'),path=require('path'),walk=function(r){let t,e=[],n=null;try{t=fs.readdirSync(r)}catch(r){n=r.toString()}if(n)return n;var a=0;return function n(){var i=t[a++];if(!i)return e;let u=path.resolve(r,i);i=r+'/'+i;let c=fs.statSync(u);if(c&&c.isDirectory()){let r=walk(i);return e=e.concat(r),n()}return e.push(i),n()}()};walk('./source_code/iris/src').join(' ');\")",
                "<!@(node -p \"var fs=require('fs'),path=require('path'),walk=function(r){let t,e=[],n=null;try{t=fs.readdirSync(r)}catch(r){n=r.toString()}if(n)return n;var a=0;return function n(){var i=t[a++];if(!i)return e;let u=path.resolve(r,i);i=r+'/'+i;let c=fs.statSync(u);if(c&&c.isDirectory()){let r=walk(i);return e=e.concat(r),n()}return e.push(i),n()}()};walk('./source_code/iris/libyuv/source').join(' ');\")",
                "<!@(node -p \"var fs=require('fs'),path=require('path'),walk=function(r){let t,e=[],n=null;try{t=fs.readdirSync(r)}catch(r){n=r.toString()}if(n)return n;var a=0;return function n(){var i=t[a++];if(!i)return e;let u=path.resolve(r,i);i=r+'/'+i;let c=fs.statSync(u);if(c&&c.isDirectory()){let r=walk(i);return e=e.concat(r),n()}return e.push(i),n()}()};walk('./source_code/ipc').join(' ');\")",
                "<!@(node -p \"var fs=require('fs'),path=require('path'),walk=function(r){let t,e=[],n=null;try{t=fs.readdirSync(r)}catch(r){n=r.toString()}if(n)return n;var a=0;return function n(){var i=t[a++];if(!i)return e;let u=path.resolve(r,i);i=r+'/'+i;let c=fs.statSync(u);if(c&&c.isDirectory()){let r=walk(i);return e=e.concat(r),n()}return e.push(i),n()}()};walk('./source_code/plugin').join(' ');\")",
                "<!@(node -p \"var fs=require('fs'),path=require('path'),walk=function(r){let t,e=[],n=null;try{t=fs.readdirSync(r)}catch(r){n=r.toString()}if(n)return n;var a=0;return function n(){var i=t[a++];if(!i)return e;let u=path.resolve(r,i);i=r+'/'+i;let c=fs.statSync(u);if(c&&c.isDirectory()){let r=walk(i);return e=e.concat(r),n()}return e.push(i),n()}()};walk('./source_code/raw_data_observer').join(' ');\")",
                "<!@(node -p \"var fs=require('fs'),path=require('path'),walk=function(r){let t,e=[],n=null;try{t=fs.readdirSync(r)}catch(r){n=r.toString()}if(n)return n;var a=0;return function n(){var i=t[a++];if(!i)return e;let u=path.resolve(r,i);i=r+'/'+i;let c=fs.statSync(u);if(c&&c.isDirectory()){let r=walk(i);return e=e.concat(r),n()}return e.push(i),n()}()};walk('./source_code/common').join(' ');\")"
            ],
            'conditions': [
                [
                    'OS=="win"',
                    {
                        'link_settings': {
                            'libraries': [
                                '-lws2_32.lib',
                                '-lRpcrt4.lib',
                                '-lgdiplus.lib'
                            ]
                        },
                        'defines!': [
                            '_USING_V110_SDK71_',
                            '_HAS_EXCEPTIONS=0'
                        ],

                    }
                ],
                [
                    'OS=="mac"',
                    {
                        'mac_framework_dirs': [
                            '../sdk/lib/mac'
                        ],
                        'copies': [{
                            'destination': '<(PRODUCT_DIR)',
                            'files': [
                                './sdk/lib/mac/AgoraAIDenoiseExtension.framework',
                                './sdk/lib/mac/AgoraCore.framework',
                                './sdk/lib/mac/AgoraDav1dExtension.framework',
                                './sdk/lib/mac/Agorafdkaac.framework',
                                './sdk/lib/mac/Agoraffmpeg.framework',
                                './sdk/lib/mac/AgoraRtcKit.framework',
                                './sdk/lib/mac/AgoraSoundTouch.framework',
                                './sdk/lib/mac/av1.framework'
                            ]
                        }],
                        'link_settings': {
                            'libraries': [
                                'AgoraAIDenoiseExtension.framework',
                                'AgoraCore.framework',
                                'AgoraDav1dExtension.framework',
                                'Agorafdkaac.framework',
                                'Agoraffmpeg.framework',
                                'AgoraRtcKit.framework',
                                'AgoraSoundTouch.framework',
                                'av1.framework'
                            ]
                        },
                        'sources': [
                            './source_code/agora_node_ext/node_screen_window_info_mac.cpp',
                            './source_code/agora_node_ext/node_screen_window_info.h',
                            './source_code/process/node_process_unix.cpp',
                            './source_code/iris/libyuv/source/compare_gcc.cc',
                            './source_code/iris/libyuv/source/rotate_gcc.cc',
                            './source_code/iris/libyuv/source/row_gcc.cc',
                            './source_code/iris/libyuv/source/scale_gcc.cc'
                        ],
                        'include_dirs': [
                            './sdk/lib/mac/AgoraRtcKit.framework/Headers'
                        ],
                        'defines!': [
                            '_NOEXCEPT',
                            '-std=c++11'
                        ],
                        'OTHER_CFLAGS': [
                            '-std=c++11',
                            '-stdlib=libc++',
                            '-fexceptions'
                        ],
                        'xcode_settings': {
                            'GCC_ENABLE_CPP_EXCEPTIONS': 'YES',
                            'GCC_ENABLE_CPP_RTTI': 'YES',
                            'MACOSX_DEPLOYMENT_TARGET': '10.11',
                            'EXECUTABLE_EXTENSION': 'node',
                            'FRAMEWORK_SEARCH_PATHS': [
                                './sdk/lib/mac'
                            ],
                            "DEBUG_INFORMATION_FORMAT": "dwarf-with-dsym"
                        },
                    }
                ]
            ]
        },
    ]
}
