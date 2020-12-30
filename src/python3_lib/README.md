# python3-prucam
A python library for interfacing the prucam kernel module.

## Dependencies
- `python3-opencv-headless prucam-dkms`

## Build debian package
- Install dependencies: `$ sudo apt install python3-all python3-stdeb`
- Make deb package: `$ python3 setup.py --command-packages=stdeb.command bdist_deb`
- The deb package will be in `deb_dist/`.