`timescale 1ns / 1ps
/////////////////////////////////////////////////////////
module ALU_Control (
    input [6:0] funct7,
    input [2:0] funct3,
    input [1:0] ALUOp,
    output reg [3:0] ALU_Control
);
   always @(*) begin
    case (ALUOp)
        2'b00 : ALU_Control = 4'b0000; 
        2'b01 : ALU_Control = 4'b1000;
        2'b10 : ALU_Control = {funct7[5], funct3};
//        2'b11 
        default: ALU_Control = 4'b0000;
    endcase
   end
endmodule
