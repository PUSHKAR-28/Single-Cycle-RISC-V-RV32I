`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 06/01/2026 02:42:17 AM
// Design Name: 
// Module Name: Single_Cycle_CPU_Top
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module Single_Cycle_CPU_Top(
    input clk, reset
    );
    
    //ALL MODULE OUTPUTS ARE WIRES
    //Control Unit
    wire ALUSrc, RegWrite, MemRead, MemWrite, Branch, Jump, LUI, AUIPC;
    wire [1:0] WBSrc, ALUOp;
    //ALU
    wire [31:0] ALU_Out;
    wire Zero, Negative, Carry, Overflow;
    //ALU_Control
    wire [3:0] ALU_Control;
    //Branch_Condn Unit
    wire Taken;
    //DMEM
    wire [31:0] read_data;
    //IMEM
    wire [31:0] Instruction;
    //Immediate Generator
    wire [31:0] Immediate;
    //PC+4
    wire [31:0] PC_plus_4;
    //PC+Imm
    wire [31:0] PC_plus_Imm;
    //Program Counter
    wire [31:0] PC_out;
    //Reg_File
    wire [31:0] reg_out_data1, reg_out_data2;
    
    //SOME EXTRA wires and registers FOR OUTPUTS FROM MUXES
    wire [31:0] A, B;
    reg [31:0] Write_Data, PC_in;
    wire [31:0] JALR_target = (reg_out_data1 + Immediate) & ~32'h1;
    
    //ALL MUXES
    //ALUPC MUX
    assign A = AUIPC? PC_out : reg_out_data1;
    //ALUSrc MUX
    assign B = ALUSrc? Immediate : reg_out_data2;
    //Write Back MUX
    always @(*) begin
        if (LUI) begin
            Write_Data = Immediate;
        end else begin
            case (WBSrc) //replaced MemtoReg// 00=ALU, 01=Memory, 10=PC+4, 11=Upper-Imm(unused,LUI bypasses)
                2'b01 : Write_Data = read_data;
                2'b10 : Write_Data = PC_plus_4;
                default : Write_Data = ALU_Out;              
            endcase
        end
    end
    //PC MUX
    always @(*) begin
        if ((Instruction[6:0] == 7'b1100111) && Jump)
            PC_in = JALR_target;
        else if (Jump)
            PC_in = PC_plus_Imm;
        else if (Branch && Taken)
            PC_in = PC_plus_Imm;
        else
            PC_in = PC_plus_4;
    end
    
    
    
    
    //ALL THE INSTANTIATIONS
    Program_Counter PC_inst (
        .clk         (clk),
        .reset       (reset),
        .PC_in       (PC_in),
        .PC_out      (PC_out)
    );
    ALU ALU_inst (
        .ALU_Control (ALU_Control),
        .A           (A),
        .B           (B), 
        .ALU_Out     (ALU_Out),
        .Zero        (Zero),
        .Negative    (Negative),
        .Overflow    (Overflow),
        .Carry       (Carry)
    );
    ALU_Control ALU_Control_inst (
        .funct3      (Instruction[14:12]),
        .funct7      (Instruction[31:25]),
        .ALUOp       (ALUOp),
        .ALU_Control (ALU_Control)
    );
    Branch_Condn Branch_Condn_inst(
        .funct3      (Instruction[14:12]),
        .Zero        (Zero),
        .Negative    (Negative),
        .Carry       (Carry),
        .Overflow    (Overflow),
        .Taken       (Taken)
    );
    Control_Unit Control_Unit_inst (
        .Instruction (Instruction),
        .ALUSrc      (ALUSrc),
        .RegWrite    (RegWrite),
        .MemRead     (MemRead),
        .MemWrite    (MemWrite),
        .Branch      (Branch),
        .Jump        (Jump),
        .LUI         (LUI),
        .AUIPC       (AUIPC),
        .WBSrc       (WBSrc),
        .ALUOp       (ALUOp)
    );
    DMEM DMEM_inst (
        .clk         (clk),
        .MemRead     (MemRead),
        .MemWrite    (MemWrite),
        .funct3      (Instruction[14:12]),
        .address     (ALU_Out),
        .write_data  (reg_out_data2),
        .read_data   (read_data)
    );
    IMEM IMEM_inst (
        .clk         (clk),
        .read_address(PC_out),
        .Instruction (Instruction)
    );
    Immediate_Generator Immediate_Generator_inst(
        .Instruction (Instruction),
        .Immediate   (Immediate)
    );
    PC_4_addr PC_4_addr_inst(
        .PC_curr     (PC_out),
        .PC_plus_4   (PC_plus_4)
    );
    PC_Imm_addr PC_Imm_addr_inst(
        .PC_curr     (PC_out),
        .Immediate   (Immediate),
        .PC_plus_Imm   (PC_plus_Imm)
    );
    Reg_File Reg_File_inst(
        .clk          (clk), 
        .reset        (reset),
        .RegWrite     (RegWrite),
        .Rs1          (Instruction[19:15]),
        .Rs2          (Instruction[24:20]),
        .Rd           (Instruction[11:7]),
        .Write_Data   (Write_Data),
        .reg_out_data2(reg_out_data2),
        .reg_out_data1(reg_out_data1)        
    );
    
endmodule


