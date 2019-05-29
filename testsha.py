# -*- coding: utf-8 -*-
"""
Created on Tue May 28 22:40:03 2019

@author: andrewsim
"""
import os
import os.path as path
import sys
import subprocess

testdir = "testsuite"


def runtest(files):
    failures = []
    for f in files:
        fpath = path.join(testdir,f)
        result_shasum = subprocess.run(["sha256sum", fpath], stdout=subprocess.PIPE)
        shasum_hash = result_shasum.stdout.decode("utf-8")
        result_csha = subprocess.run(["./csha256", fpath], stdout=subprocess.PIPE)
        csha_hash = result_csha.stdout.decode("utf-8")
        
        print("{:s}:  size: {:d}\nShasum Hash:\n{:s}".format(f,path.getsize(fpath), shasum_hash))
        print("CSha Hash:\n{:s}".format(csha_hash))
        compare = csha_hash == shasum_hash
        if not compare:
            failures.append(f)
        print(compare)
        print()
        
    return failures
        