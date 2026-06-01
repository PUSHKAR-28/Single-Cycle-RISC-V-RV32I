# RV32I Single-Cycle Processor — Complete Instruction Datapath Reference

## Architecture Overview

### Modules in the design
- **Program_Counter** — holds PC_out, updates on posedge clk
- **IMEM** — reads instruction at mem[PC_out[13:2]]
- **Control_Unit** — decodes Instruction[6:0], drives all control signals
- **Immediate_Generator** — assembles and sign-extends immediate from instruction bits
- **Reg_File** — 32×32-bit registers, combinational read, clocked write
- **ALU_Control** — decodes ALUOp + funct3 + funct7[5] into 4-bit ALU_Control
- **ALU** — executes operation, outputs ALU_Out + flags (Zero, Negative, Carry, Overflow)
- **Branch_Condn** — evaluates branch flags using funct3, outputs Taken
- **DMEM** — word-wide memory with byte-lane read/write, combinational read, clocked write
- **PC_4_addr** — computes PC_out + 4
- **PC_Imm_addr** — computes PC_out + Immediate

### Muxes (all in top module)
- **AUIPC mux** → `A = AUIPC ? PC_out : reg_out_data1`
- **ALUSrc mux** → `B = ALUSrc ? Immediate : reg_out_data2`
- **Writeback mux** → `Write_Data` selected by LUI and WBSrc[1:0]
- **PC mux** → `PC_in` selected by Jump, JALR opcode, Branch & Taken

### Control signals
| Signal   | Width | Meaning |
|----------|-------|---------|
| ALUSrc   | 1     | 0=rs2 to ALU-B, 1=Immediate to ALU-B |
| RegWrite | 1     | 1=write Write_Data into Rd on clock edge |
| MemRead  | 1     | 1=DMEM performs a load |
| MemWrite | 1     | 1=DMEM performs a store on clock edge |
| Branch   | 1     | 1=this is a branch instruction |
| Jump     | 1     | 1=this is JAL or JALR |
| LUI      | 1     | 1=bypass writeback mux, write Immediate directly to Rd |
| AUIPC    | 1     | 1=feed PC_out into ALU-A instead of rs1 |
| WBSrc    | 2     | 00=ALU_Out, 01=read_data, 10=PC_plus_4 |
| ALUOp    | 2     | 00=ADD, 01=SUB, 10=decode funct3/funct7 |

### ALU_Control encoding (your design)
| ALU_Control | Operation |
|-------------|-----------|
| 4'b0000     | ADD       |
| 4'b1000     | SUB       |
| 4'b0100     | XOR       |
| 4'b0110     | OR        |
| 4'b0111     | AND       |
| 4'b0001     | SLL       |
| 4'b0101     | SRL       |
| 4'b1101     | SRA       |
| 4'b0010     | SLT       |
| 4'b0011     | SLTU      |

---

## R-Type Instructions
**ADD, SUB, AND, OR, XOR, SLL, SRL, SRA, SLT, SLTU**
Opcode: 0110011 | Format: R | funct3+funct7 distinguish each

### Control signals
ALUSrc=0, RegWrite=1, MemRead=0, MemWrite=0, Branch=0, Jump=0, LUI=0, AUIPC=0, WBSrc=00, ALUOp=10

---

### ADD — rd = rs1 + rs2
**Step 1 — PC and IMEM**
IMEM reads mem[PC_out[13:2]]. Instruction encoding: opcode=0110011, funct3=000, funct7=0000000, rd=[11:7], rs1=[19:15], rs2=[24:20].

**Step 2 — Control Unit**
Sees opcode 0110011. Sets ALUSrc=0, RegWrite=1, MemRead=0, MemWrite=0, Branch=0, Jump=0, LUI=0, AUIPC=0, WBSrc=00, ALUOp=10.

**Step 3 — Immediate Generator**
Opcode is R-type. Outputs 32'b0 (unused).

**Step 4 — Register File**
Reads Rs1=Instruction[19:15] → reg_out_data1. Reads Rs2=Instruction[24:20] → reg_out_data2. Both combinational.

**Step 5 — AUIPC mux**
AUIPC=0 → A = reg_out_data1.

**Step 6 — ALUSrc mux**
ALUSrc=0 → B = reg_out_data2.

**Step 7 — ALU Control**
ALUOp=10 → ALU_Control = {funct7[5], funct3} = {0, 000} = 4'b0000 → ADD.

**Step 8 — ALU**
ALU_Out = A + B.

**Step 9 — DMEM**
MemRead=0, MemWrite=0. DMEM outputs 32'b0. Unused.

**Step 10 — Writeback mux**
LUI=0, WBSrc=00 → Write_Data = ALU_Out.

**Step 11 — Register File write**
RegWrite=1. On posedge clk: Registers[Rd] <= Write_Data = rs1+rs2.

**Step 12 — PC update**
Branch=0, Jump=0 → PC_in = PC_plus_4.

---

### SUB — rd = rs1 - rs2
Identical to ADD except funct7=0100000.

**Step 7 — ALU Control**
ALU_Control = {funct7[5], funct3} = {1, 000} = 4'b1000 → SUB.

**Step 8 — ALU**
is_SUB=1. B_Comp = B ^ 32'hFFFFFFFF. arith_sum = A + B_Comp + 1 (two's complement subtraction). ALU_Out = A - B.

All other steps identical to ADD.

---

### AND — rd = rs1 & rs2
funct3=111, funct7=0000000.

**Step 7 — ALU Control**
ALU_Control = {0, 111} = 4'b0111 → AND.

**Step 8 — ALU**
ALU_Out = A & B.

---

### OR — rd = rs1 | rs2
funct3=110, funct7=0000000.

**Step 7 — ALU Control**
ALU_Control = {0, 110} = 4'b0110 → OR.

**Step 8 — ALU**
ALU_Out = A | B.

---

### XOR — rd = rs1 ^ rs2
funct3=100, funct7=0000000.

**Step 7 — ALU Control**
ALU_Control = {0, 100} = 4'b0100 → XOR.

**Step 8 — ALU**
ALU_Out = A ^ B.

---

### SLL — rd = rs1 << rs2[4:0]
funct3=001, funct7=0000000.

**Step 7 — ALU Control**
ALU_Control = {0, 001} = 4'b0001 → SLL.

**Step 8 — ALU**
ALU_Out = A << B[4:0]. Only bottom 5 bits of rs2 used as shift amount.

---

### SRL — rd = rs1 >> rs2[4:0] (logical, zero-fills)
funct3=101, funct7=0000000.

**Step 7 — ALU Control**
ALU_Control = {0, 101} = 4'b0101 → SRL.

**Step 8 — ALU**
ALU_Out = A >> B[4:0].

---

### SRA — rd = rs1 >>> rs2[4:0] (arithmetic, sign-fills)
funct3=101, funct7=0100000.

**Step 7 — ALU Control**
ALU_Control = {1, 101} = 4'b1101 → SRA.

**Step 8 — ALU**
ALU_Out = $signed(A) >>> B[4:0]. Upper bits filled with sign bit of rs1.

---

### SLT — rd = (rs1 < rs2) ? 1 : 0 (signed)
funct3=010, funct7=0000000.

**Step 7 — ALU Control**
ALU_Control = {0, 010} = 4'b0010 → SLT.

**Step 8 — ALU**
is_SUB=1. ALU_Out = {31'b0, ($signed(A) < $signed(B))}.

---

### SLTU — rd = (rs1 < rs2) ? 1 : 0 (unsigned)
funct3=011, funct7=0000000.

**Step 7 — ALU Control**
ALU_Control = {0, 011} = 4'b0011 → SLTU.

**Step 8 — ALU**
is_SUB=1. ALU_Out = {31'b0, (A < B)}.

---

## I-Type ALU Instructions
**ADDI, SLTI, SLTIU, XORI, ORI, ANDI, SLLI, SRLI, SRAI**
Opcode: 0010011 | Format: I | Immediate in Instruction[31:20]

### Control signals
ALUSrc=1, RegWrite=1, MemRead=0, MemWrite=0, Branch=0, Jump=0, LUI=0, AUIPC=0, WBSrc=00, ALUOp=10

The only difference from R-type: ALUSrc=1 so the immediate goes into ALU-B instead of rs2. No Rs2 is used.

---

### ADDI — rd = rs1 + imm
funct3=000. Immediate: Instruction[31:20] sign-extended.

**Step 3 — Immediate Generator**
Opcode=0010011 (I-type). Immediate = {{20{Instruction[31]}}, Instruction[31:20]}.

**Step 6 — ALUSrc mux**
ALUSrc=1 → B = Immediate.

**Step 7 — ALU Control**
ALUOp=10, funct3=000, funct7[5]=0 → ALU_Control=4'b0000 → ADD.

**Step 8 — ALU**
ALU_Out = rs1 + sign_extended_immediate.

Steps 1,2,4,5,9,10,11,12 same as ADD.

---

### SLTI — rd = ($signed(rs1) < $signed(imm)) ? 1 : 0
funct3=010.

**Step 7 — ALU Control**
ALU_Control = 4'b0010 → SLT.

**Step 8 — ALU**
ALU_Out = {31'b0, ($signed(A) < $signed(B))}.

---

### SLTIU — rd = (rs1 < imm) ? 1 : 0 (unsigned)
funct3=011.

**Step 7 — ALU Control**
ALU_Control = 4'b0011 → SLTU.

**Step 8 — ALU**
ALU_Out = {31'b0, (A < B)}.

---

### XORI — rd = rs1 ^ imm
funct3=100.

**Step 7 — ALU Control**
ALU_Control = 4'b0100 → XOR.

---

### ORI — rd = rs1 | imm
funct3=110.

**Step 7 — ALU Control**
ALU_Control = 4'b0110 → OR.

---

### ANDI — rd = rs1 & imm
funct3=111.

**Step 7 — ALU Control**
ALU_Control = 4'b0111 → AND.

---

### SLLI — rd = rs1 << imm[4:0]
funct3=001, funct7=0000000. Shift amount is Instruction[24:20].

**Step 7 — ALU Control**
ALU_Control = 4'b0001 → SLL.

**Step 8 — ALU**
ALU_Out = A << B[4:0]. ImmGen produces the shift amount in [4:0] of Immediate.

---

### SRLI — rd = rs1 >> imm[4:0] (logical)
funct3=101, funct7=0000000.

**Step 7 — ALU Control**
ALU_Control = 4'b0101 → SRL.

---

### SRAI — rd = $signed(rs1) >>> imm[4:0] (arithmetic)
funct3=101, funct7=0100000.

**Step 7 — ALU Control**
funct7[5]=1 → ALU_Control = {1,101} = 4'b1101 → SRA.

---

## Load Instructions
**LB, LH, LW, LBU, LHU**
Opcode: 0000011 | Format: I

### Control signals
ALUSrc=1, RegWrite=1, MemRead=1, MemWrite=0, Branch=0, Jump=0, LUI=0, AUIPC=0, WBSrc=01, ALUOp=00

---

### LW — rd = M[rs1 + imm][31:0]
funct3=010.

**Step 1 — PC and IMEM**
Instruction: opcode=0000011, funct3=010, rd=[11:7], rs1=[19:15], imm=[31:20].

**Step 2 — Control Unit**
ALUSrc=1, RegWrite=1, MemRead=1, WBSrc=01, ALUOp=00.

**Step 3 — Immediate Generator**
I-type: Immediate = {{20{Instruction[31]}}, Instruction[31:20]}.

**Step 4 — Register File**
Reads Rs1 → reg_out_data1 (base address).

**Step 5 — AUIPC mux**
AUIPC=0 → A = reg_out_data1.

**Step 6 — ALUSrc mux**
ALUSrc=1 → B = Immediate.

**Step 7 — ALU Control**
ALUOp=00 → ALU_Control=4'b0000 → ADD.

**Step 8 — ALU**
ALU_Out = rs1 + sign_extended_offset. This is the byte address of the word in memory.

**Step 9 — DMEM read**
MemRead=1, funct3=010 (LW). word_index = ALU_Out[31:2].
read_data = mem[ALU_Out[31:2]]. Full 32-bit word, no extension needed.

**Step 10 — Writeback mux**
LUI=0, WBSrc=01 → Write_Data = read_data.

**Step 11 — Register File write**
RegWrite=1. Registers[Rd] <= read_data.

**Step 12 — PC update**
PC_in = PC_plus_4.

---

### LH — rd = sign_extend(M[rs1 + imm][15:0])
funct3=001.

Steps 1-8 identical to LW.

**Step 9 — DMEM read**
MemRead=1, funct3=001 (LH). word_index = ALU_Out[31:2].
- ALU_Out[1]=0 → read_data = {{16{mem[word_index][15]}}, mem[word_index][15:0]}
- ALU_Out[1]=1 → read_data = {{16{mem[word_index][31]}}, mem[word_index][31:16]}

Sign bit of the selected halfword fills the upper 16 bits.

Steps 10-12 identical to LW.

---

### LHU — rd = zero_extend(M[rs1 + imm][15:0])
funct3=101. Identical to LH except:

**Step 9 — DMEM read**
- ALU_Out[1]=0 → read_data = {16'b0, mem[word_index][15:0]}
- ALU_Out[1]=1 → read_data = {16'b0, mem[word_index][31:16]}

Upper 16 bits are always zero regardless of the halfword value.

---

### LB — rd = sign_extend(M[rs1 + imm][7:0])
funct3=000.

**Step 9 — DMEM read**
word_index = ALU_Out[31:2]. ALU_Out[1:0] selects which byte:
- 2'b00 → read_data = {{24{mem[word_index][7]}},  mem[word_index][7:0]}
- 2'b01 → read_data = {{24{mem[word_index][15]}}, mem[word_index][15:8]}
- 2'b10 → read_data = {{24{mem[word_index][23]}}, mem[word_index][23:16]}
- 2'b11 → read_data = {{24{mem[word_index][31]}}, mem[word_index][31:24]}

Sign bit of selected byte fills upper 24 bits.

---

### LBU — rd = zero_extend(M[rs1 + imm][7:0])
funct3=100. Identical to LB except upper 24 bits are always 0:
- 2'b00 → read_data = {24'b0, mem[word_index][7:0]}
- 2'b01 → read_data = {24'b0, mem[word_index][15:8]}
- 2'b10 → read_data = {24'b0, mem[word_index][23:16]}
- 2'b11 → read_data = {24'b0, mem[word_index][31:24]}

---

## Store Instructions
**SB, SH, SW**
Opcode: 0100011 | Format: S

### Control signals
ALUSrc=1, RegWrite=0, MemRead=0, MemWrite=1, Branch=0, Jump=0, LUI=0, AUIPC=0, WBSrc=00, ALUOp=00

Nothing is written to the register file. RegWrite=0.

---

### SW — M[rs1 + imm] = rs2[31:0]
funct3=010.

**Step 1 — PC and IMEM**
Instruction: opcode=0100011, funct3=010, rs1=[19:15], rs2=[24:20], imm split: [31:25] and [11:7].

**Step 2 — Control Unit**
ALUSrc=1, RegWrite=0, MemWrite=1, ALUOp=00.

**Step 3 — Immediate Generator**
S-type: Immediate = {{20{Instruction[31]}}, Instruction[31:25], Instruction[11:7]}.

**Step 4 — Register File**
Rs1 → reg_out_data1 (base address). Rs2 → reg_out_data2 (data to store). Both read simultaneously.

**Step 5 — AUIPC mux**
AUIPC=0 → A = reg_out_data1.

**Step 6 — ALUSrc mux**
ALUSrc=1 → B = Immediate (store offset).

**Step 7 — ALU Control**
ALUOp=00 → ALU_Control=4'b0000 → ADD.

**Step 8 — ALU**
ALU_Out = rs1 + sign_extended_offset. Byte address of the store target.

**Step 9 — DMEM write**
MemWrite=1, funct3=010 (SW). On posedge clk:
mem[ALU_Out[31:2]] <= reg_out_data2. Full 32 bits written.

**Step 10 — Writeback mux**
RegWrite=0. Write_Data irrelevant, register file ignores it.

**Step 11 — PC update**
PC_in = PC_plus_4.

---

### SH — M[rs1 + imm][15:0] = rs2[15:0]
funct3=001.

Steps 1-8 identical to SW.

**Step 9 — DMEM write**
MemWrite=1, funct3=001 (SH). word_index = ALU_Out[31:2]. On posedge clk:
- ALU_Out[1]=0 → mem[word_index][15:0]  <= reg_out_data2[15:0]
- ALU_Out[1]=1 → mem[word_index][31:16] <= reg_out_data2[15:0]

Only 16 bits written. Upper 16 bits of rs2 discarded. Other halfword in same word untouched.

---

### SB — M[rs1 + imm][7:0] = rs2[7:0]
funct3=000.

**Step 9 — DMEM write**
MemWrite=1, funct3=000 (SB). word_index = ALU_Out[31:2]. On posedge clk:
- ALU_Out[1:0]=2'b00 → mem[word_index][7:0]   <= reg_out_data2[7:0]
- ALU_Out[1:0]=2'b01 → mem[word_index][15:8]  <= reg_out_data2[7:0]
- ALU_Out[1:0]=2'b10 → mem[word_index][23:16] <= reg_out_data2[7:0]
- ALU_Out[1:0]=2'b11 → mem[word_index][31:24] <= reg_out_data2[7:0]

Only 8 bits written. Upper 24 bits of rs2 discarded. Other 3 bytes in same word untouched.

---

## Branch Instructions
**BEQ, BNE, BLT, BGE, BLTU, BGEU**
Opcode: 1100011 | Format: B

### Control signals
ALUSrc=0, RegWrite=0, MemRead=0, MemWrite=0, Branch=1, Jump=0, LUI=0, AUIPC=0, WBSrc=00, ALUOp=01

Nothing written to register file or memory. Only PC may change.

### How branching works
ALUOp=01 forces ALU_Control=4'b1000 (SUB). The ALU computes rs1 - rs2 and updates all flags. Branch_Condn reads funct3 and the flags to decide Taken. In the PC mux: if Branch=1 AND Taken=1, PC_in = PC_plus_Imm (branch target). Otherwise PC_in = PC_plus_4.

---

### BEQ — branch if rs1 == rs2
funct3=000.

**Step 1 — PC and IMEM**
Instruction: opcode=1100011, funct3=000, rs1=[19:15], rs2=[24:20], imm: Instruction[31], Instruction[7], Instruction[30:25], Instruction[11:8], 1'b0.

**Step 2 — Control Unit**
Branch=1, ALUOp=01. All write signals 0.

**Step 3 — Immediate Generator**
B-type: Immediate = {{19{Instruction[31]}}, Instruction[31], Instruction[7], Instruction[30:25], Instruction[11:8], 1'b0}. This is the signed byte offset to add to PC.

**Step 4 — Register File**
Reads Rs1 → reg_out_data1. Reads Rs2 → reg_out_data2.

**Step 5 — AUIPC mux**
AUIPC=0 → A = reg_out_data1.

**Step 6 — ALUSrc mux**
ALUSrc=0 → B = reg_out_data2.

**Step 7 — ALU Control**
ALUOp=01 → ALU_Control=4'b1000 → SUB.

**Step 8 — ALU**
ALU_Out = rs1 - rs2. Flags updated: Zero=1 if rs1==rs2.

**Step 9 — Branch_Condn**
funct3=000 (BEQ) → Taken = Zero.

**Step 10 — PC_Imm_addr**
PC_plus_Imm = PC_out + Immediate (branch target address).

**Step 11 — PC mux**
Branch=1, Taken=Zero:
- If rs1==rs2: PC_in = PC_plus_Imm (branch taken)
- If rs1!=rs2: PC_in = PC_plus_4 (branch not taken)

**Step 12 — DMEM and Writeback**
MemRead=0, MemWrite=0, RegWrite=0. Nothing written anywhere.

---

### BNE — branch if rs1 != rs2
funct3=001.

**Step 9 — Branch_Condn**
Taken = ~Zero.

---

### BLT — branch if rs1 < rs2 (signed)
funct3=100.

**Step 9 — Branch_Condn**
Taken = Negative ^ Overflow. This is the standard signed less-than condition from subtraction flags.

---

### BGE — branch if rs1 >= rs2 (signed)
funct3=101.

**Step 9 — Branch_Condn**
Taken = ~(Negative ^ Overflow).

---

### BLTU — branch if rs1 < rs2 (unsigned)
funct3=110.

**Step 9 — Branch_Condn**
Taken = ~Carry. When subtracting unsigned, no carry out means a borrow occurred, meaning rs1 < rs2.

---

### BGEU — branch if rs1 >= rs2 (unsigned)
funct3=111.

**Step 9 — Branch_Condn**
Taken = Carry.

---

## U-Type Instructions

### LUI — rd = imm[31:12] << 12
Opcode: 0110111 | Format: U

### Control signals
ALUSrc=0, RegWrite=1, MemRead=0, MemWrite=0, Branch=0, Jump=0, LUI=1, AUIPC=0, WBSrc=00, ALUOp=00

**Step 1 — PC and IMEM**
Instruction: opcode=0110111, rd=[11:7], imm=[31:12].

**Step 2 — Control Unit**
LUI=1, RegWrite=1. All others 0.

**Step 3 — Immediate Generator**
U-type: Immediate = {Instruction[31:12], 12'b0}. Upper 20 bits of instruction placed in upper 20 bits, lower 12 zeroed.

**Steps 4-8 — Register File, Muxes, ALU**
ALU runs but its output is irrelevant. LUI bypasses the writeback mux entirely.

**Step 9 — DMEM**
MemRead=0, MemWrite=0. Unused.

**Step 10 — Writeback mux**
LUI=1 → Write_Data = Immediate. ALU_Out and read_data are ignored.

**Step 11 — Register File write**
RegWrite=1. Registers[Rd] <= {Instruction[31:12], 12'b0}.

**Step 12 — PC update**
PC_in = PC_plus_4.

---

### AUIPC — rd = PC + (imm[31:12] << 12)
Opcode: 0010111 | Format: U

### Control signals
ALUSrc=1, RegWrite=1, MemRead=0, MemWrite=0, Branch=0, Jump=0, LUI=0, AUIPC=1, WBSrc=00, ALUOp=00

**Step 1 — PC and IMEM**
Instruction: opcode=0010111, rd=[11:7], imm=[31:12].

**Step 2 — Control Unit**
AUIPC=1, ALUSrc=1, RegWrite=1, WBSrc=00, ALUOp=00.

**Step 3 — Immediate Generator**
U-type: Immediate = {Instruction[31:12], 12'b0}.

**Step 4 — Register File**
Rs1 is read but unused (AUIPC mux overrides it).

**Step 5 — AUIPC mux**
AUIPC=1 → A = PC_out. PC is fed into ALU input A.

**Step 6 — ALUSrc mux**
ALUSrc=1 → B = Immediate (upper immediate).

**Step 7 — ALU Control**
ALUOp=00 → ALU_Control=4'b0000 → ADD.

**Step 8 — ALU**
ALU_Out = PC_out + {Instruction[31:12], 12'b0}.

**Step 9 — DMEM**
Unused.

**Step 10 — Writeback mux**
LUI=0, WBSrc=00 → Write_Data = ALU_Out = PC + upper_immediate.

**Step 11 — Register File write**
RegWrite=1. Registers[Rd] <= PC + upper_immediate.

**Step 12 — PC update**
PC_in = PC_plus_4.

---

## J-Type Instruction

### JAL — rd = PC+4; PC = PC + imm
Opcode: 1101111 | Format: J

### Control signals
ALUSrc=0, RegWrite=1, MemRead=0, MemWrite=0, Branch=0, Jump=1, LUI=0, AUIPC=0, WBSrc=10, ALUOp=00

**Step 1 — PC and IMEM**
Instruction: opcode=1101111, rd=[11:7], imm scrambled: Instruction[31], Instruction[19:12], Instruction[20], Instruction[30:21], 1'b0.

**Step 2 — Control Unit**
Jump=1, RegWrite=1, WBSrc=10.

**Step 3 — Immediate Generator**
J-type: Immediate = {{11{Instruction[31]}}, Instruction[31], Instruction[19:12], Instruction[20], Instruction[30:21], 1'b0}. Signed 21-bit offset, always even.

**Step 4 — Register File**
Rs1, Rs2 read but unused.

**Steps 5-8 — ALU**
ALU runs but output unused.

**Step 9 — PC_Imm_addr**
PC_plus_Imm = PC_out + Immediate. This is the jump destination.

**Step 10 — Writeback mux**
LUI=0, WBSrc=10 → Write_Data = PC_plus_4. Return address.

**Step 11 — Register File write**
RegWrite=1. Registers[Rd] <= PC_plus_4. Saves return address.

**Step 12 — PC mux**
Jump=1, opcode≠JALR → PC_in = PC_plus_Imm. Processor jumps to PC + J-immediate.

---

## I-Type Jump Instruction

### JALR — rd = PC+4; PC = (rs1 + imm) & ~1
Opcode: 1100111 | Format: I | funct3=000

### Control signals
ALUSrc=1, RegWrite=1, MemRead=0, MemWrite=0, Branch=0, Jump=1, LUI=0, AUIPC=0, WBSrc=10, ALUOp=00

**Step 1 — PC and IMEM**
Instruction: opcode=1100111, funct3=000, rd=[11:7], rs1=[19:15], imm=[31:20].

**Step 2 — Control Unit**
Jump=1, ALUSrc=1, RegWrite=1, WBSrc=10, ALUOp=00.

**Step 3 — Immediate Generator**
I-type: Immediate = {{20{Instruction[31]}}, Instruction[31:20]}.

**Step 4 — Register File**
Rs1 → reg_out_data1. Base register for jump target.

**Step 5 — AUIPC mux**
AUIPC=0 → A = reg_out_data1.

**Step 6 — ALUSrc mux**
ALUSrc=1 → B = Immediate.

**Steps 7-8 — ALU**
ALU_Out = rs1 + Immediate. But ALU_Out is NOT the jump target here — the JALR target is computed separately in the top module as:
JALR_target = (reg_out_data1 + Immediate) & ~32'h1 (LSB cleared per spec).

**Step 9 — DMEM**
Unused.

**Step 10 — Writeback mux**
WBSrc=10 → Write_Data = PC_plus_4.

**Step 11 — Register File write**
RegWrite=1. Registers[Rd] <= PC_plus_4.

**Step 12 — PC mux**
Jump=1 AND Instruction[6:0]==7'b1100111 → PC_in = JALR_target = (rs1 + Immediate) & ~1.

