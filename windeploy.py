#!/usr/bin/python3
# coding: UTF-8

from re import compile as regex, IGNORECASE, match
from shutil import copy
from os.path import isdir, join, dirname, abspath
from os import getcwd, chdir, listdir, walk, remove
from sys import exit as sysexit
from subprocess import run
from uuid import uuid4

signtool = "signtool.exe"
windeployqt = "windeployqt.exe"
heat = "heat.exe"
candle = "candle.exe"
light = "light.exe"

rootdir = dirname(abspath(__file__))
builddir = getcwd()

def get_release_information():
    rxstr = r"^\s*project\(([^\s]+)\s+version\s+(\d(\.\d)*)\s+description\s+.*\)$"
    project_re = regex(rxstr, IGNORECASE)
    with open(join(rootdir, "CMakeLists.txt")) as fh:
        lines = [project_re.match(X.strip()) for X in fh.readlines() 
                if project_re.match(X.strip())]
    if lines == []:
        print("Couldn't find versioning information.")
        sysexit(1)
    return (lines[0].group(1), lines[0].group(2))

name, version = get_release_information()
productdir = join(builddir, "{}-{}".format(name, version))

def sanity_check():
    if not isdir(productdir):
        print("Product directory doesn't exist.")
        sysexit(1)
    executables = [X for X in listdir(productdir) if match(r"^.*\.exe$", X)]
    if executables == []:
        print("No executables in product directory.")
        sysexit(1)

def deploy_libraries():
    sanity_check()
    run([windeployqt, productdir])

def sign_release():
    sanity_check()
    files_to_sign = []
    for (path, _, files) in walk(productdir):
        files_to_sign += [join(path, X) for X in files
                         if match(r"^.*\.(exe|dll)$", X)]
    for filename in files_to_sign:
        run([signtool, "sign", "/tr", "http://timestamp.digicert.com",
            "/td", "sha256", "/fd", "sha256", "/a", filename])

def make_msi():
    row_rx = regex(r'\s*<File Id="([a-zA-Z0-9]+)"\s.*Source=".*\\' + name + r'\.exe"\s*/>')
    for filename in listdir(productdir):
        if match(r"^.*\.(wxs|wixobj)$", filename):
            remove(join(productdir, filename))
    run([heat, "dir", productdir, "-sreg", "-gg", "-sfrag", "-srd", 
        "-template", "fragment", "-out", join(builddir, "directory.wxs"),
        "-cg", "MainApp", "-dr", "AppDir", "-t",
        join(rootdir, "HeatTransform.xslt")])
    with open(join(builddir, "directory.wxs")) as fh:
        rows = [X.strip() for X in fh.readlines() if row_rx.match(X.strip())]
    if len(rows) != 1:
        print("Either no, or more than one, executable was found.")
        sysexit(1)
    executable_id = row_rx.match(rows[0]).group(1)
    with open(join(builddir, "{}.wxs".format(name))) as fh:
        rows = fh.readlines()
    with open(join(builddir, "{}.wxs".format(name)), "w", encoding="UTF-8") as fh:
        for row in rows:
            row = row.replace("myapplication.exe", executable_id)
            row = row.replace("PUT-GUID-HERE", str(uuid4()))
            fh.write(row)

    copy(join(rootdir, "license.rtf"), productdir)
    for filename in listdir(builddir):
        if match(r"^.*\.wxs$", filename):
            copy(join(builddir, filename), productdir)
    origdir = getcwd()
    chdir(productdir)
    run([candle, "*.wxs"])
    msiname = join(builddir, "{}-{}.msi".format(name, version))
    run([light, "*.wixobj", "-o", msiname, "-ext", "WixUIExtension"])
    chdir(origdir)
    run([signtool, "sign", "/tr", "http://timestamp.digicert.com",
            "/td", "sha256", "/fd", "sha256", "/a", msiname])
    remove(join(productdir, "license.rtf"))
    for filename in listdir(productdir):
        if match(r"^.*\.(wxs|wixobj)$", filename):
            remove(join(productdir, filename))
    

if __name__ == '__main__':
    deploy_libraries()
    sign_release()
    make_msi()