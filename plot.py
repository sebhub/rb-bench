#
# Copyright (c) 2014 embedded brains GmbH.  All rights reserved.
#
#  embedded brains GmbH
#  Dornierstr. 4
#  82178 Puchheim
#  Germany
#  <info@embedded-brains.de>
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#

import libxml2
from libxml2 import xmlNode
import matplotlib.pyplot as plt
doc = libxml2.parseFile("test.xml")
ctx = doc.xpathNewContext()

def getImpl(n):
	return n.prop("implementation");

impls = map(getImpl, ctx.xpathEval("/RBTestCollection/RBTest"))

def getNodeCountAndDuration(n):
	return [n.prop("nodeCount"), n.prop("duration")]

plt.title("red-black tree test runs")
plt.xlabel('node count')
plt.ylabel('test duration [ns]')

for i in impls:
	xy = map(getNodeCountAndDuration, ctx.xpathEval("/RBTestCollection/RBTest[@implementation='" + i + "']/SmallSetLinear/Sample"))
	x = [x[0] for x in xy]
	y = [y[1] for y in xy]
	plt.plot(x, y, label = i)

plt.legend()
plt.show()