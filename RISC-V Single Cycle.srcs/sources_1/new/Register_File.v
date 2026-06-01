`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
module Reg_File(
    input clk, reset, RegWrite,
    input [4:0] Rs1, Rs2, Rd,
    input [31:0] Write_Data,
    output [31:0] reg_out_data1, reg_out_data2
);
    reg [31:0] Registers [31:0];
    integer k =0;
    
    always @(posedge clk or posedge reset) begin
        if (reset) begin
            for (k = 0; k < 32; k=k+1) begin
                Registers[k] <= 32'b0;
            end
        end else if (RegWrite && (Rd != 5'b0)) begin
            Registers[Rd] <= Write_Data;
        end
    end
    
    assign reg_out_data1 = (Rs1 == 5'b0) ? 32'b0 : Registers[Rs1];
    assign reg_out_data2 = (Rs2 == 5'b0) ? 32'b0 : Registers[Rs2];
    
endmodule