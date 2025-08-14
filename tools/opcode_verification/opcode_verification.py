"""
A Quicky and Dirty script I made with chatGPT to skim my c++ files to 
visually check that all the opcodes are mapped to the right parameters. 

The output it generates should be very similar to opcode table found at
https://web.archive.org/web/20221112231348if_/http://archive.6502.org/datasheets/rockwell_r650x_r651x.pdf
"""
import re
from PIL import Image, ImageDraw, ImageFont

# Load the C++ source file
with open("../NES_Emulator/opcode_lookup.cpp", "r") as f:
    source = f.read()

# Regex pattern — FIXED to accept 1 or 2 hex digits
pattern = re.compile(
    r'opcode_lookup\[(0x[0-9A-Fa-f]{1,2})\]\s*=\s*Opcode\(\s*"([^"]+)",\s*std::bind\(&cpu::(\w+),\s*this\),\s*std::bind\(&cpu::(\w+),\s*this\),\s*uint8_t\((\d+)\),\s*(true|false)\s*\)',
    re.MULTILINE
)

# Compact the address mode name
def compact_addr_mode(name):
    name = re.sub(r'_addr$', '', name)
    prefix = name[:3]
    capitals = ''.join(c for c in name[3:] if c.isupper())
    return prefix + capitals

# Build the opcode map and track mismatches
opcode_map = {}
mismatched_opcodes = set()

for match in pattern.finditer(source):
    hex_val = int(match.group(1), 16)
    string_mnemonic = match.group(2)  # e.g., "BRK"
    addr_fn_raw = match.group(3)      # e.g., Implicit_addr
    exec_fn = match.group(4)          # e.g., BRK
    cycles = match.group(5)
    flag_val = match.group(6)

    # Check for mismatch between mnemonic string and function name
    if string_mnemonic.upper() != exec_fn.upper():
        print(f"⚠️ Mismatch at opcode {match.group(1)}: string '{string_mnemonic}' != function '{exec_fn}'")
        mismatched_opcodes.add(hex_val)

    addr_mode = compact_addr_mode(addr_fn_raw)

    # Asterisk logic
    if addr_mode == "Rel":
        cycles_flagged = f"{cycles}**"
    elif flag_val == "true":
        cycles_flagged = f"{cycles}*"
    else:
        cycles_flagged = cycles

    # Use the actual function name instead of the string mnemonic
    opcode_map[hex_val] = f"{exec_fn}\n{addr_mode}\n{cycles_flagged}"

# Image settings
cell_size = 160
cols, rows = 16, 16
width = cell_size * cols
height = cell_size * rows
bg_color = "white"
line_color = "black"

# Try to load a larger font
try:
    font = ImageFont.truetype("arial.ttf", 50)
except:
    font = ImageFont.load_default()

# Create the image
img = Image.new("RGB", (width, height), bg_color)
draw = ImageDraw.Draw(img)

# Draw grid and populate with opcode text
for y in range(rows):
    for x in range(cols):
        idx = y * 16 + x
        x0, y0 = x * cell_size, y * cell_size
        draw.rectangle([x0, y0, x0 + cell_size, y0 + cell_size], outline=line_color)

        text = opcode_map.get(idx, "")
        fill_color = "red" if idx in mismatched_opcodes else "black"
        draw.multiline_text((x0 + 6, y0 + 6), text, fill=fill_color, font=font, spacing=4)

# Save image
img.save("opcode_table.png")
print("✅ Created 'opcode_table.png'")
