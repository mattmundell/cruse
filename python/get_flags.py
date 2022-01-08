#!/usr/bin/env python2.7
import distutils.sysconfig
print distutils.sysconfig.get_config_var('LINKFORSHARED')
