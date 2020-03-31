# Copyright (C) 2010-2013 Cuckoo Sandbox Developers.
# This file is part of Cuckoo Sandbox - http://www.cuckoosandbox.org
# See the file 'docs/LICENSE' for copying permission.

from lib.common.abstracts import Package
from lib.api.process import Process
from lib.common.exceptions import CuckooPackageError
from lib.core.config import Config
import os

class Exe(Package):
    """EXE analysis package."""

    def start(self, path):
        pin = "C:\\pin\\pin.exe"
        pindll = os.path.join(os.getcwd(), "dll", "PinVMShield.dll")
        if not pindll:
            raise CuckooPackageError("Unable to find any DBA available")

        free = self.options.get("free", False)
        args = self.options.get("arguments", None)
        dbi = self.options.get("dbi", None)
        if dbi == "true":
            isdbi = True
        else:
            isdbi = False
        
        suspended = True
        if free:
            suspended = False

        p = Process()
        if not isdbi:
            if not p.execute(path=path, args=args, suspended=suspended):
                raise CuckooPackageError("Unable to execute initial process, analysis aborted")
        else:
            if not p.execute(path=pin, args="-t \"%s\" -- \"%s\" %s" % (pindll,path,args), suspended=suspended):
                raise CuckooPackageError("Unable to execute initial process, analysis aborted")
        if not free and suspended:
            p.inject()
            p.resume()
            p.close()
            return p.pid
        else:
            return None

    def check(self):
        return True

    def finish(self):
        if self.options.get("procmemdump", False):
            for pid in self.pids:
                p = Process(pid=pid)
                p.dump_memory()

        return True
