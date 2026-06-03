import sys

with open("raw.hex", "r") as f:
    lines = f.readlines()

IMEM = []
DMEM = []
curr_addr = 0
is_DMEM = False

for line in lines:
    line = line.strip()
    if line.startswith('@'):
        curr_addr = int(line[1:],16)
        continue
    if curr_addr >= 0x2000:
        is_DMEM = True

    bytes_in_line = line.split()
    for i in range(0, len(bytes_in_line), 4):
        chunk = bytes_in_line[i:i+4]
        if (len(chunk)!=4):
            break
        word = int(chunk[3],16)<<24 | int(chunk[2],16)<<16 | int(chunk[1],16)<<8 | int(chunk[0],16)
        if is_DMEM:
            DMEM.append(f"{word:08X}")
        else: 
            IMEM.append(f"{word:08X}")
            
with open("IMEM.hex", "w") as f:
    f.write("\n".join(IMEM) + "\n")
        
with open("DMEM.hex", "w") as f:
    f.write("\n".join(DMEM) + "\n")


    