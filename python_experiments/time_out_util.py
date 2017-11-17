import os, sys, commands
import subprocess, shlex, time
from threading import Timer


def run_with_timeout(cmd, timeout_sec):
    global is_time_out
    is_time_out = False
    proc = subprocess.Popen(shlex.split(cmd), stdout=subprocess.PIPE,
                            stderr=subprocess.PIPE)

    def time_out_call_back(p):
        print 'time out need to kill'
        global is_time_out
        is_time_out = True
        p.kill()

    timer = Timer(timeout_sec, time_out_call_back, [proc])
    try:
        timer.start()
        stdout, stderr = proc.communicate()
        print str(stderr).replace('\n', '')
    finally:
        timer.cancel()
    # print is_time_out
    return is_time_out, str(stderr).replace('\n', ''), str(stdout)


if __name__ == '__main__':
    tle_flag, info = run_with_timeout('sleep 20', timeout_sec=5)
    print tle_flag, info