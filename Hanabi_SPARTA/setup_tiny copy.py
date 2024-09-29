from setuptools import setup
from setuptools.command.build_ext import build_ext
import pybind11

import sys
import os

os.environ['CXX'] = 'clang++'

OPTIONAL_SRC = []
if int(os.environ.get("INSTALL_TORCHBOT", 0)):
    OPTIONAL_SRC = ["csrc/TorchBot.cc"]

# boost_libs = ["boost_fiber", "boost_thread", "boost_context"]
# if sys.platform == "darwin":
#     boost_libs = [lib + '-mt' for lib in boost_libs]
# boost_libs = []

# OpenSpiel 라이브러리 설정
openspiel_libs = ['open_spiel']
openspiel_include_dirs = [
    '/home/swkang/Spiel_SPARTA/open_spiel',  # OpenSpiel의 기본 헤더 경로
    '/home/swkang/Spiel_SPARTA/open_spiel/open_spiel',  # 추가 헤더 경로
    '/home/swkang/Spiel_SPARTA/open_spiel/open_spiel/abseil-cpp',  # Abseil 헤더 경로
    '/home/swkang/Spiel_SPARTA/open_spiel/open_spiel/games/hanabi/hanabi-learning-environment/hanabi_learning_environment',
    '/home/swkang/Spiel_SPARTA/open_spiel/open_spiel/games/tiny_hanabi'  # tiny_hanabi.h 경로
]

openspiel_include_dirs = [
    '/home/swkang/Spiel_SPARTA/open_spiel',
    '/home/swkang/Spiel_SPARTA/open_spiel/open_spiel',
    '/home/swkang/Spiel_SPARTA/open_spiel/open_spiel/abseil-cpp',
    '/home/swkang/Spiel_SPARTA/open_spiel/open_spiel/games/hanabi/hanabi-learning-environment/hanabi_learning_environment',
    '/home/swkang/Spiel_SPARTA/open_spiel/open_spiel/games/tiny_hanabi',
    # os.getenv('CPLUS_INCLUDE_PATH').split(':')  # 추가된 include 경로
]

openspiel_library_dirs = [
    '/home/swkang/Spiel_SPARTA/open_spiel/build',
    # os.getenv('LIBRARY_PATH')  # 추가된 라이브러리 경로
]


setup(
    name='hanabi_lib_tiny',
    ext_modules=[
        CppExtension('hanabi_lib_tiny', [
            # "csrc/extension.cc",
            # "csrc/SimpleBot.cc",
            # "csrc/HolmesBot.cc",
            # "csrc/SmartBot.cc",
            # "csrc/SearchBot.cc",
            # "csrc/JointSearchBot.cc",
            # "csrc/HanabiServer.cc",
            # "csrc/BotUtils.cc",
            # "csrc/TorchBot.cc",
            "csrc/tiny_hanabi_test.cc",
            # "csrc/extensiontiny.cc",
            # "tiny_hanabi.cc",  # Add tiny_hanabi.cc
        ] + OPTIONAL_SRC,
        extra_compile_args=['-std=c++17', '-fPIC', '-Wno-deprecated', '-O3', '-Wno-sign-compare', '-D_GLIBCXX_USE_CXX11_ABI=0', '-DCARD_ID=1', '-fpermissive'],
        
        libraries=['z', 'open_spiel'] + openspiel_libs,  # Ensure open_spiel is included
        library_dirs=[
            '/usr/local/lib',
            '/home/swkang/miniconda3/envs/swkang_hanabi/lib'
        ] + openspiel_library_dirs,
        include_dirs=['csrc'] + openspiel_include_dirs,
        undef_macros=['NDEBUG'])
    ],
    cmdclass={"build_ext": BuildExtension}, 
    options={
        'build': {'build_base':  'build_tiny'} 
    }
)