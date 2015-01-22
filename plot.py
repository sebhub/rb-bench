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

import sys
import libxml2
from libxml2 import xmlNode
import matplotlib.pyplot as plt
doc = libxml2.parseFile(sys.argv[1])
root = doc.children
ctx = doc.xpathNewContext()

cmPerInch = 2.54

def getImpl(n):
	return n.prop("implementation");

impls = map(getImpl, ctx.xpathEval("/RBTestCollection/RBTest"))

def getNodeCountAndDuration(n):
	return [n.prop("nodeCount"), n.prop("duration")]

def plotTest(test):
	plt.title("Red-Black Tree Benchmark: " + test + "\nPlatform: " + root.prop("platform") + ", " + root.prop("compiler"))
	plt.xlabel('Node Count')
	plt.ylabel('Test Duration [s]')
	for i in impls:
		xy = map(getNodeCountAndDuration, ctx.xpathEval("/RBTestCollection/RBTest[@implementation='" + i + "']/" + test + "/Sample"))
		x = [int(x[0]) for x in xy]
		y = [int(y[1]) / 1e9 for y in xy]
		plt.plot(x, y, label = i)
	plt.legend(fontsize = 10, bbox_to_anchor=(0.3, 0.9))

plt.figure(figsize = (30.0 / cmPerInch, 2 * 30.0 / 1.618 / cmPerInch))
plt.title('x')
plt.subplot(211)
plotTest('SmallSetLinear');
plt.subplot(212)
plotTest('SmallSetRandomOps');
plt.savefig(sys.argv[2])
