import sys
import os
sys.path.insert(0, os.path.dirname(__file__))

from dryox_build import ffibuilder
from setuptools import setup

setup(
    ext_modules=[ffibuilder.distutils_extension()],
)