import pandas as pd
from jinja2 import Environment, FileSystemLoader

MODES_MAP = {
    'absolute' :    "Absolute_addr", 
    'absolute,X':   "AbsoluteX_addr", 
    'implied':      "Implicit_addr", 
    'immediate':    "Immediate_addr", 
    'indirect':     "Indirect_addr", 
    '(indirect,X)': "IndirectX_addr", 
    'absolute,Y':   "AbsoluteY_addr", 
    'accumulator':  "Accumulator_addr", 
    'relative':     "Relative_addr", 
    'zeropage':     "ZeroPage_addr", 
    '(indirect),Y': "IndirectY_addr",
    'zeropage,X':   "ZeroPageX_addr", 
    'zeropage,Y':   "ZeroPageY_addr", 
}

# This csv was created by cutting info directly from 
# https://www.masswerk.at/6502/6502_instruction_set.html
with open("../opcodes.csv", "r") as f: 
    dat_frame = pd.read_csv(f, header=0)

opcodes = []
for row in dat_frame.itertuples():
    full_code = {
        "address_mode": MODES_MAP[row[1]], 
        "mnemonic": row[2][0:3],
        "opcode": ("0x" + row[3]), 
        "cycles_needed": row[5][0], 
        "extra_cycle": (row[5][1:] != "")
    }
    opcodes.append(full_code)
opcodes = sorted(opcodes, key=lambda x: x["opcode"])

# Set up the Jinja2 environment to load templates from the current directory
env = Environment(loader=FileSystemLoader('.'))
template = env.get_template('opcode_template.jinja2')

# Define the data to be used in the template
data_dict = {
    'opcodes': opcodes
}

# Render the template with the provided data
generated_code = template.render(data_dict)

# Save or print the generated code
with open('opcode_lookup.cpp', 'w') as f:
    f.write(generated_code)
