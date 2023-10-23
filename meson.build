project('circleq', 'c',
    version: '1.0.0',
    license: 'UNLICENSE',
    default_options: [
        'buildtype=debugoptimized',
        'default_library=static',
        'c_std=c11',
        'werror=true',
        'warning_level=2',
    ]
)

project_source_root = meson.current_source_dir()
project_build_root = meson.current_build_dir()

install_headers('src/circleq.h')

if get_option('tests')
    cc = meson.get_compiler('c')

    cflags = [
        '-D_GNU_SOURCE',
        '-D_LARGE_FILES',
        '-D_LARGEFILE_SOURCE',
        '-D_FILE_OFFSET_BITS=64',
        '-D_TIME_BITS=64',
    ]

    cflags_check = [
        '-pipe',
        '-funwind-tables',
        '-fno-common',
        '-Wfatal-errors',
        '-Wundef',
        '-pedantic',
        '-pedantic-errors',
        '-Wstrict-prototypes',
        '-Wmissing-prototypes',
        '-Wmissing-declarations',
        '-Wno-missing-field-initializers',
        '-Wno-unused-parameter',
        '-Wduplicated-branches',
        '-Wduplicated-cond',
        '-Wimplicit-fallthrough',
        '-Wdouble-promotion',
        '-Wpointer-arith',
        '-Wfloat-equal',
        '-Wlogical-op',
        '-Wshadow',
        '-Wvla',
        '-Walloca',
        '-Wc++-compat',
        '-Wformat=2',
        '-Wformat-overflow=2',
        '-Wformat-truncation=2',
        '-Wformat-signedness',
        '-Wno-format-pedantic',
        '-Wno-format-nonliteral',
        '-Wno-clobbered',
    ]

    if cc.get_id() == 'clang'
        cflags_check += [
            '-Wno-gnu',
            '-Wno-builtin-requires-header',
            '-Wno-missing-braces',
        ]
    endif

    if cc.get_id() == 'gcc' and get_option('analyzer')
        cflags_check += '-fanalyzer'
        cflags_check += '-Wno-analyzer-malloc-leak'

        if cc.version().version_compare('<12')
            cflags_check += '-Wno-analyzer-mismatching-deallocation'
        endif
    endif

    add_project_arguments(
        cflags,
        cc.get_supported_arguments(cflags_check),
        language: 'c'
    )

    circleq_test = executable('circleq_test', 'src/circleq_test.c',
        install: false,
    )

    if get_option('valgrind')
        valgrind = find_program('valgrind', required: true)
        valgrind_args = [
            '--tool=memcheck',
            '--leak-check=full',
            '--show-leak-kinds=all',
            '--track-fds=no',
            '--track-origins=yes',
            '--trace-children=yes',
            '--error-exitcode=1',
        ]
        test('circleq_test', valgrind, args: [valgrind_args, circleq_test])
    else
        test('circleq_test', circleq_test)
    endif
endif

astyle = find_program('astyle', required: false)
if astyle.found()
    custom_target('astyle',
        output: 'astyle',
        command: [
            astyle,
            '--options=@0@/.astylerc'.format(project_source_root),
            '-Q', '-n', '-r',
            project_source_root / 'src/*.c',
            project_source_root / 'src/*.h',
        ],
        console: true,
    )
endif
