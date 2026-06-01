`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////

module Branch_Condn(
    input [2:0] funct3,
    input Zero, Negative, Carry, Overflow,
    output reg Taken
);
    always @(*) begin
        case(funct3)
            3'b000: Taken = Zero;                  // BEQ
            3'b001: Taken = ~Zero;                 // BNE
            3'b100: Taken = Negative ^ Overflow;   // BLT  (signed)
            3'b101: Taken = ~(Negative ^ Overflow);// BGE  (signed)
            3'b110: Taken = ~Carry;                // BLTU (unsigned)
            3'b111: Taken = Carry;                 // BGEU (unsigned)
            default: Taken = 0;
        endcase
    end
endmodule
