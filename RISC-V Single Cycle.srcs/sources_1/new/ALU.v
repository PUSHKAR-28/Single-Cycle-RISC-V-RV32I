`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////

module ALU(
    input [3:0] ALU_Control,
    input [31:0] A, B, 
    output reg [31:0] ALU_Out, 
    output Zero, Negative, Carry, Overflow
);
    wire [32:0] arith_sum;
    wire [31:0] B_Comp;
    wire is_SUB = (ALU_Control == 4'b1000) || (ALU_Control == 4'b0010) || (ALU_Control == 4'b0011);
    assign B_Comp = B^{32{is_SUB}};
    assign arith_sum = A + B_Comp + {32'b0,is_SUB};
    
    always @(*) begin
        case (ALU_Control)
            4'b0000 : ALU_Out = arith_sum[31:0];                   //ADD
            4'b1000 : ALU_Out = arith_sum[31:0];                   //SUB
            4'b0100 : ALU_Out = A ^ B;                             //XOR
            4'b0110 : ALU_Out = A | B;                             //OR
            4'b0111 : ALU_Out = A & B;                             //AND
            4'b0001 : ALU_Out = A << B[4:0];                       //SLL
            4'b0101 : ALU_Out = A >> B[4:0];                       //SRL
            4'b1101 : ALU_Out = $signed(A) >>> B[4:0];             //SRA
            4'b0010 : ALU_Out = {31'b0,{$signed(A) < $signed(B)}}; //SLT
            4'b0011 : ALU_Out = {31'b0,{A < B}};                   //SLTU            
            default : ALU_Out = 32'b0;
        endcase
    end
    
    assign Zero = (ALU_Out == 32'b0);
    assign Negative = ALU_Out[31];
    assign Carry = arith_sum[32];
    assign Overflow = (A[31] == B_Comp[31]) && (ALU_Out[31] != A[31]);
endmodule
