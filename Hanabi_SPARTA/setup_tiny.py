from setuptools import setup
from setuptools.command.build_ext import build_ext
from setuptools import Extension
import os

# clang++ 설정
os.environ['CXX'] = 'clang++'

# OpenSpiel 및 라이브러리 경로 설정
openspiel_libs = ['open_spiel']
openspiel_include_dirs = [
    '/home/swkang/Spiel_SPARTA/open_spiel',
    '/home/swkang/Spiel_SPARTA/open_spiel/open_spiel',
    '/home/swkang/Spiel_SPARTA/open_spiel/open_spiel/abseil-cpp',
    '/home/swkang/Spiel_SPARTA/open_spiel/open_spiel/games/hanabi/hanabi-learning-environment/hanabi_learning_environment',
    '/home/swkang/Spiel_SPARTA/open_spiel/open_spiel/games/tiny_hanabi'
]

openspiel_library_dirs = [
    '/home/swkang/Spiel_SPARTA/open_spiel/build',
]

# Pybind11의 include 경로를 가져오는 헬퍼 함수
def get_pybind_include():
    import pybind11
    return pybind11.get_include()

include_dirs = ['csrc'] + openspiel_include_dirs + [get_pybind_include()]

setup(
    name='hanabi_lib_tiny',
    ext_modules=[
        Extension(
            'hanabi_lib_tiny',
            [
                "csrc/tiny_hanabi_test.cc",
            ],
            extra_compile_args=['-std=c++17', '-fPIC', '-Wno-deprecated', '-O3', '-Wno-sign-compare', '-fpermissive'],
            libraries=['z', 'open_spiel'] + openspiel_libs,  # OpenSpiel 라이브러리 포함
            library_dirs=[
                '/usr/local/lib',
                '/home/swkang/miniconda3/envs/swkang_hanabi/lib'
            ] + openspiel_library_dirs,
            include_dirs=include_dirs,
            undef_macros=['NDEBUG']
        )
    ],
    cmdclass={"build_ext": build_ext},
    options={'build': {'build_base': 'build_tiny'}}
)
