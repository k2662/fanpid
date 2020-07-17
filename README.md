# fanpid

macOS fan controller. Default macOS has some weaksauce fan control: to prevent
thermal throttling, I wanted to use a more aggressive control scheme but I
didn't want to pay for one of the premium apps and the other open source
projects out there didn't quite work. To debug the other projects, I'd have to
deep dive. In the end I decided to just roll my own.

This project uses my [libsmcpp](https://github.com/cbosoft/libsmcpp) library
(based heaviliy on beltex's excellent `c` version:
[libsmc](https://github.com/beltex/libsmc).

# Building

Install libsmcpp (clone, make, sudo make install)
```bash
clone https://github.com/cbosoft/libsmcpp
cd libsmcpp
make
sudo make install
```

Clone this repo and make:
```bash
git clone https://github.com/cbosoft/fanpid
cd fanpid
make
```

# Usage


Running the program as root with use the
[PID](https://en.wikipedia.org/wiki/PID_controller) algorithm to set the fan speed.

The PID parameters can be set in a config file in whatever directory you run
fanpid from. The config file is very simple: key value pairs separated by
colons.

```
setpoint:50
kp:200
ki:0.0
kd:0.0
```

This will be read by the program on startup, and re-read on receipt of SIGUSR1.
