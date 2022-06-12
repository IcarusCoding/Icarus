import argparse
import subprocess
import os
import sys
import binascii

parser = argparse.ArgumentParser(description='Generate windows shellcode from asm files')
parser.add_argument('--path', metavar='path', type=str, help='sets the path to an asm file', required=True)
parser.add_argument('--section', metavar='sectionfilter', type=str, help='sets a section filter',
                    default='.text*')
parser.add_argument('--arch', type=str, help='sets the architecture',
                    choices=['x64', 'x86'], required=True)
parser.add_argument('--keep-auxiliary', action='store_true', help='keeps the auxiliary files after completion')
parser.add_argument('--format', type=str, help='sets the output format', choices=['hexstring', 'carray'], required=True)
args = parser.parse_args()

if not os.path.exists(args.path) or not args.path.endswith('.asm'):
    print('Please specify a valid and existing asm file.')
    sys.exit()

dir = os.path.dirname(args.path)
name = os.path.splitext(os.path.basename(args.path))[0]

ret_val = subprocess.call(['ml64.exe' if args.arch == 'x64' else 'ml.exe', '/c', '/nologo', '/Zi',
                           f'/Fo{os.path.join(dir, name + ".obj")}', f'/Fl{os.path.join(dir, name + ".lst")}',
                           '/W3', '/errorReport:prompt', f'/Ta{args.path}'])
if ret_val:
    print('Failed to create object file (is the selected architecture correct?).')
else:
    ret_val = subprocess.call(['objcopy.exe', '-O', 'binary', '-j', args.section, os.path.join(dir, name + ".obj"),
                               os.path.join(dir, name + ".bin")])
    if ret_val:
        print('Failed to create binary file.')
    else:
        with open(os.path.join(dir, name + ".bin"), 'rb') as file:
            print('Generated Shellcode:')
            match args.format:
                case 'hexstring':
                    h = str(binascii.hexlify(file.read()), 'ascii')
                    print(h)
                case 'carray':
                    h = str(binascii.hexlify(file.read(), ' ', 1), 'ascii').split()
                    h = ['{0:#0{1}x}'.format(int(i, 16), 4) for i in h]
                    print('BYTE ShellCode[] = {')
                    for i in range(0, len(h), 15):
                        print('\t', ', '.join(h[i:i + 15]) + ',')
                    print('};')

if not args.keep_auxiliary:
    if os.path.exists(os.path.join(dir, name + ".obj")):
        os.remove(os.path.join(dir, name + ".obj"))
    if os.path.exists(os.path.join(dir, name + ".lst")):
        os.remove(os.path.join(dir, name + ".lst"))
    if os.path.exists(os.path.join(dir, name + ".bin")):
        os.remove(os.path.join(dir, name + ".bin"))
