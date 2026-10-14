#!/usr/bin/env python3
"""Run a command in a pty with a fixed window size, feed timed key input,
capture all output. Usage:
    ptyrun.py ROWS COLS OUTFILE SCRIPT_JSON -- cmd args...
SCRIPT_JSON is a list of [delay_seconds, "python-escaped string"].
"""
import os, sys, time, fcntl, termios, struct, select, json

rows = int(sys.argv[1])
cols = int(sys.argv[2])
outfile = sys.argv[3]
script = json.loads(sys.argv[4])
assert sys.argv[5] == '--', sys.argv[5]
cmd = sys.argv[6:]

winsz = struct.pack('HHHH', rows, cols, 0, 0)

pid, fd = os.forkpty()
if pid == 0:
    # child
    try:
        fcntl.ioctl(0, termios.TIOCSWINSZ, winsz)
    except Exception:
        pass
    os.environ['TERM'] = 'xterm-256color'
    os.environ['COLUMNS'] = str(cols)
    os.environ['LINES'] = str(rows)
    os.execvp(cmd[0], cmd)
    os._exit(127)

fcntl.ioctl(fd, termios.TIOCSWINSZ, winsz)

events = []
for d, s in script:
    try:
        b = s.encode('utf-8').decode('unicode_escape').encode('latin1')
    except Exception:
        b = s.encode('utf-8')
    events.append((float(d), b))

buf = bytearray()
t0 = time.time()
idx = 0
child_done = False
max_t = 40.0
while True:
    now = time.time() - t0
    while idx < len(events) and events[idx][0] <= now:
        try:
            os.write(fd, events[idx][1])
        except OSError:
            pass
        idx += 1
    r, _, _ = select.select([fd], [], [], 0.05)
    if r:
        try:
            data = os.read(fd, 65536)
        except OSError:
            break
        if not data:
            break
        buf += data
    if not child_done:
        wpid, status = os.waitpid(pid, os.WNOHANG)
        if wpid == pid:
            child_done = True
    if child_done and idx >= len(events):
        # drain a little
        time.sleep(0.2)
        try:
            while True:
                r, _, _ = select.select([fd], [], [], 0.1)
                if not r:
                    break
                data = os.read(fd, 65536)
                if not data:
                    break
                buf += data
        except OSError:
            pass
        break
    if now > max_t:
        try:
            os.kill(pid, 9)
        except OSError:
            pass
        break

with open(outfile, 'wb') as f:
    f.write(bytes(buf))
sys.stderr.write(f'\n[ptyrun] captured {len(buf)} bytes, exit_after={time.time()-t0:.1f}s\n')
