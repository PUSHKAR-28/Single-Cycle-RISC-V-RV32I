`timescale 1ns / 1ps
module Control_Unit(
    input  [31:0] Instruction,
    output reg        ALUSrc, RegWrite, MemRead, MemWrite, Branch, Jump, LUI, AUIPC,
    output reg [1:0]  WBSrc,   //replaced MemtoReg// 00=ALU, 01=Memory, 10=PC+4, 11=Upper-Imm(unused,LUI bypasses)
    output reg [1:0]  ALUOp
);
    always @(*) begin
        case (Instruction[6:0])
         // opcode       ALUSrc,RegWrite,MemRead,MemWrite,Branch,Jump,LUI,AUIPC, WBSrc, ALUOp
            7'b0110011 : {ALUSrc,RegWrite,MemRead,MemWrite,Branch,Jump,LUI,AUIPC,WBSrc,ALUOp} = 12'b0_1_0_0_0_0_0_0_00_10; // R-type
            7'b0010011 : {ALUSrc,RegWrite,MemRead,MemWrite,Branch,Jump,LUI,AUIPC,WBSrc,ALUOp} = 12'b1_1_0_0_0_0_0_0_00_10; // I-type ALU
            7'b0000011 : {ALUSrc,RegWrite,MemRead,MemWrite,Branch,Jump,LUI,AUIPC,WBSrc,ALUOp} = 12'b1_1_1_0_0_0_0_0_01_00; // Loads
            7'b0100011 : {ALUSrc,RegWrite,MemRead,MemWrite,Branch,Jump,LUI,AUIPC,WBSrc,ALUOp} = 12'b1_0_0_1_0_0_0_0_00_00; // Stores
            7'b1100011 : {ALUSrc,RegWrite,MemRead,MemWrite,Branch,Jump,LUI,AUIPC,WBSrc,ALUOp} = 12'b0_0_0_0_1_0_0_0_00_01; // Branches
            7'b1100111 : {ALUSrc,RegWrite,MemRead,MemWrite,Branch,Jump,LUI,AUIPC,WBSrc,ALUOp} = 12'b1_1_0_0_0_1_0_0_10_00; // JALR
            7'b1101111 : {ALUSrc,RegWrite,MemRead,MemWrite,Branch,Jump,LUI,AUIPC,WBSrc,ALUOp} = 12'b0_1_0_0_0_1_0_0_10_00; // JAL
            7'b0110111 : {ALUSrc,RegWrite,MemRead,MemWrite,Branch,Jump,LUI,AUIPC,WBSrc,ALUOp} = 12'b0_1_0_0_0_0_1_0_00_00; // LUI
            7'b0010111 : {ALUSrc,RegWrite,MemRead,MemWrite,Branch,Jump,LUI,AUIPC,WBSrc,ALUOp} = 12'b1_1_0_0_0_0_0_1_00_00; // AUIPC
            default    : {ALUSrc,RegWrite,MemRead,MemWrite,Branch,Jump,LUI,AUIPC,WBSrc,ALUOp} = 12'b0_0_0_0_0_0_0_0_00_00;
        endcase
    end
endmodule