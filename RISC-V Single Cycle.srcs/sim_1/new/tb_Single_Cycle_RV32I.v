`timescale 1ns / 1ps

module tb_Single_Cycle_RV32I;

    // 1. Declare Testbench Signals
    reg clk;
    reg reset;

    // 2. Instantiate the Top Module
    Single_Cycle_CPU_Top dut (
        .clk(clk),
        .reset(reset)
    );

    // 3. Clock Generation
    initial begin
        clk = 0;
        forever #10 clk = ~clk; 
    end

    // 4. Memory Initialization & Simulation Flow
    initial begin
        // Load the compiled machine code into IMEM
        // Syntax: $readmemh("filename", hierarchical_path_to_memory_array);
        $readmemh("C:/Users/PUSHKAR/Desktop/PROJECTS/RISC-V Single Cycle/instruction_mem.hex", dut.IMEM_inst.IMEM);
        
        // Load initial data into DMEM (optional, can be left uninitialized or zeroed));
        $readmemh("C:/Users/PUSHKAR/Desktop/PROJECTS/RISC-V Single Cycle/data_mem.hex", dut.DMEM_inst.mem);

        // Apply Reset
        reset = 1;
        #20;
        reset = 0;

        // Let the CPU run for a specific duration
        #900; 

        // End simulation
        $finish;
    end
    
always @(negedge clk) begin
        if (!reset) begin
            $display("Time: %0t | PC: %h | x1: %0d | x2: %0d | x3: %0d", 
                      $time, dut.PC_out, dut.Reg_File_inst.Registers[1], dut.Reg_File_inst.Registers[2], dut.Reg_File_inst.Registers[3]);
        end
    end

endmodule