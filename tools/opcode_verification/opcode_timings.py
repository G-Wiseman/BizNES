import pandas as pd

# Map of address modes to desired column names
MODES_MAP = {
    'absolute' :    "Abs", 
    'absolute,X':   "AbsX", 
    'implied':      "Imp",    # fixed from "AbsY" which seemed incorrect
    'immediate':    "Imm", 
    'indirect':     "Ind", 
    '(indirect,X)': "IndX", 
    'absolute,Y':   "AbsY", 
    'accumulator':  "Acc", 
    'relative':     "Rel", 
    'zeropage':     "ZP", 
    '(indirect),Y': "IndY",
    'zeropage,X':   "ZPX",    # capitalized consistently (was "ZpX")
    'zeropage,Y':   "ZPY",    # capitalized consistently (was "ZpY")
}

# Read the CSV
df = pd.read_csv("opcodes.csv")

# Clean up address modes: remove quotes and whitespace
df['AddressMode'] = df['AddressMode'].str.replace('"', '').str.strip()

# Map to friendly names using MODES_MAP
df['AddressMode'] = df['AddressMode'].map(MODES_MAP)

# Extract instruction name from Mnemonic (e.g. 'ADC' from 'ADC #oper')
df['Instruction'] = df['Mnemonic'].str.extract(r'^([A-Z]+)')

# Keep Cycles_Needed as string to preserve asterisks
df['Cycles_Needed'] = df['Cycles_Needed'].astype(str).str.strip()

# Pivot the table
pivot_df = df.pivot_table(
    index='Instruction',
    columns='AddressMode',
    values='Cycles_Needed',
    aggfunc='first'
)

# Alphabetize rows and columns
pivot_df = pivot_df.sort_index(axis=0).sort_index(axis=1)

# Write to CSV
pivot_df.to_csv("output.csv")