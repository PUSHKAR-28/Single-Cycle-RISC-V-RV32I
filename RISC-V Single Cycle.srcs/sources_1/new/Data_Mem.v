`timescale 1ns / 1ps
module DMEM(
    input clk,
    input MemRead, MemWrite,
    input [2:0] funct3,
    input [31:0] address, write_data,
    output reg [31:0] read_data
);
    reg [31:0] mem [0:4095]; // 16KB byte addressable

    // Read (combinational)
    always @(*) begin
        if(MemRead)
            case(funct3)
                3'b000: begin
                    case (address[1:0])
                        2'b00: read_data = {{24{mem[address[31:2]][7]}},mem[address[31:2]][7:0]};
                        2'b01: read_data = {{24{mem[address[31:2]][15]}},mem[address[31:2]][15:8]};
                        2'b10: read_data = {{24{mem[address[31:2]][23]}},mem[address[31:2]][23:16]};
                        2'b11: read_data = {{24{mem[address[31:2]][31]}},mem[address[31:2]][31:24]}; 
                    endcase
                end// LB
                3'b001: begin         
                    case (address[1:0])
                    2'b00 : read_data = {{16{mem[address[31:2]][15]}},mem[address[31:2]][15:0]};
                    2'b10 : read_data = {{16{mem[address[31:2]][31]}},mem[address[31:2]][31:16]}; 
                    default : read_data = 32'b0;
                    endcase
                end  // LH               
                3'b010: read_data = mem[address[31:2]];                                                   // LW
                3'b100: begin
                    case (address[1:0])
                        2'b00: read_data = {24'b0,mem[address[31:2]][7:0]};
                        2'b01: read_data = {24'b0,mem[address[31:2]][15:8]};
                        2'b10: read_data = {24'b0,mem[address[31:2]][23:16]};
                        2'b11: read_data = {24'b0,mem[address[31:2]][31:24]}; 
                    endcase
                end          // LBU
                3'b101: begin         
                    case (address[1:0])
                    2'b00 : read_data = {16'b0,mem[address[31:2]][15:0]};
                    2'b10 : read_data = {16'b0,mem[address[31:2]][31:16]}; 
                    default : read_data = 32'b0;
                    endcase
                end  // LHU
                default: read_data = 32'b0;
            endcase
        else read_data = 32'b0;
    end

    // Write (clocked)
    always @(posedge clk) begin
        if(MemWrite)
            case(funct3)
                3'b000: begin                     //SB
                    case(address[1:0])
                        2'b00: mem[address[31:2]][7:0] <= write_data[7:0];
                        2'b01: mem[address[31:2]][15:8] <= write_data[7:0];
                        2'b10: mem[address[31:2]][23:16] <= write_data[7:0];
                        2'b11: mem[address[31:2]][31:24] <= write_data[7:0];
                    endcase
                end
                3'b001: begin 
                //mem[address] <= write_data[7:0]; mem[address+1] <= write_data[15:8]; 
                    case (address[1:0])
                        2'b00: mem[address[31:2]][15:0] <= write_data[15:0];
                        2'b10: mem[address[31:2]][31:16] <= write_data[15:0]; 
                        default: mem[address[31:2]] <= 32'b0;
                    endcase
                end      // SH
                3'b010: mem[address[31:2]] <= write_data;     
                default: ;                                            // SW
            endcase
    end
endmodule