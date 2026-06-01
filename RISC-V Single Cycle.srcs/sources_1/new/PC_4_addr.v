`timescale 1ns / 1ps
module PC_4_addr(
    input [31:0] PC_curr,
    output [31:0] PC_plus_4
);
    assign PC_plus_4 = PC_curr + 4;
endmodule