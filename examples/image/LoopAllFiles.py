#!/usr/bin/env python
# coding=UTF-8

import os
import shutil
import sys


class color:
    def __init__(self):
        pass

    ERROR = '\033[1;31;0m'
    SUCCESS = '\033[1;34;0m'
    INFO = '\033[92m'
    ENDC = '\033[0m'


def list_all_files(rootdir):
    import os
    _files = []
    list = os.listdir(rootdir)  # 列出文件夹下所有的目录与文件
    for i in range(0, len(list)):
        path = os.path.join(rootdir, list[i])
        if os.path.isdir(path):
            _files.extend(list_all_files(path))
        if os.path.isfile(path):
            _files.append(path)
    return _files


dir = os.getcwd();
allFils = list_all_files(dir)
index = 1;
for file in allFils:
    file = file.strip()
    if file.endswith('jpg'):
        os.system("ffmpeg -i " + file + " -vf scale=1080:-1 111/" + index.__str__() + ".jpg")
        index += 1
