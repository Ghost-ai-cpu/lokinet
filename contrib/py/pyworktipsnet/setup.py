from setuptools import setup, find_packages



setup(
  name="pyworktipsnet",
  version="0.0.1",
  license="ZLIB",
  author="jeff",
  author_email="jeff@i2p.rocks",
  description="worktipsnet python bindings",
  url="https://github.com/worktips-project/worktips-network",
  install_requires=["pysodium", "requests", "python-dateutil"],
  packages=find_packages())