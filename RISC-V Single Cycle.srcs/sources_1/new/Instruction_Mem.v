`timescale 1ns / 1ps
module IMEM(
    input clk,
    input  [31:0] read_address,
    output reg [31:0] Instruction
);
    reg [31:0] IMEM [4095:0]; 
    always @(*)
        Instruction = IMEM[read_address[13:2]];
        
endmodule