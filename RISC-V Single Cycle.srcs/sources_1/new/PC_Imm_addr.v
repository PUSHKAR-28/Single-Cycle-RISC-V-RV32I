`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
module PC_Imm_addr(
    input [31:0] PC_curr, Immediate, 
    output [31:0] PC_plus_Imm
);
    assign PC_plus_Imm = PC_curr + Immediate;
endmodule